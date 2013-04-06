;*******************************************************************************
;    manchester.inc                                                            *
;    Copyright 2013 Cedric de Wijs.                                            *
;                                                                              *
;    This file is part of Opentherm Communicator                               *
;                                                                              *
;    Opentherm Communicator is free software: you can redistribute it and/or   *
;    modify it under the terms of the GNU General Public License as published  *
;    by the Free Software Foundation, either version 3 of the License,         *
;    or (at your option) any later version.                                    *
;                                                                              *
;    You should have received a copy of the GNU General Public License         *
;    along with this software. If not, see <http://www.gnu.org/licenses/>.     *
;                                                                              *
;    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS   * 
;    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF                *
;    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.    *
;    IN NO EVENT SHALL THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR      *
;    ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  *
;    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE         *
;    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                    *
;                                                                              *
;    Disclaimer: I did this project entirely to satisfy my own curiosity,      * 
;    using data available for free on the internet and electronics and         *
;    software I built myself. You may use the information in this software     *
;    for your own purposes, and at your own risk. I will not accept any        *
;    responsibility for whatever damages might occur from using or applying    *
;    the information in this software, or from following directions therein.   *
;                                                                              *
;    OpenTherm,OpenTherm/Plus,OpenTherm/Lite and the OpenTherm logo are        * 
;    registered trademarks of The OpenTherm Association.                       *
;*******************************************************************************
;Manchester code has always a transition in the middle of the transmitted bit.
;In order to compensate for clock drift the code syncronizes at the middle of 
;the received bits.
;- "Logic one"
;     ___
;    |
;    |     <--- transition from low to high
; ___|
;
;- "Logic zero"
;___
;    |
;    |     <--- transition from high to low
;    |___
;
;For example, the sequence of bits 11010011010 is represented as:
;           ___     ______        ______     ___        ___     ______        ______
;          |   |   |      |      |      |   |   |      |   |   |      |      |      |
;__________|   |___|      |______|      |___|   |______|   |___|      |______|      |___
;
;       |      |       |      |      |      |      |       |       |      |      |      |
;       |      |       |      |      |      |      |       |       |      |      |      |
;          1      1      0       1       0     0       1       1       0      1      0
;Idle    Start   D0      D1      D2      D3    D4

;The bitstream is sampled every 100uSec.
;
;A perfectly clocked 1000 usec bit looks like this:
;           0					1
;        -1 0 1 2 3 4 5 6 7 8 9 0
;           _________           __    
;          |         |         |    
;__________|         |_________|
;
;A 800usc bit (25% too fast) input signal looks like this:
;           0					1
;        -1 0 1 2 3 4 5 6 7 8 9 0
;           _______         __    
;          |       |       |    
;__________|       |_______|
;Samplepoints 0,1,2,3 are stable
;At samplepoint 8, the mid-bit point is found.
;
;A 1200 usec bit (20% too slow) looks like this 
;           0					1 1 1
;        -1 0 1 2 3 4 5 6 7 8 9 0 1 2
;           ___________             __    
;          |           |           |    
;__________|           |___________|
;Samplepoints 0,1,2,3,4,5 are stable
;At samplepoint 12, the mid-bit point is found. 

;At samplepoint 0, the first rising edge is found. This is the mid-bit point.
;The input is not allowed to change at samplepoint 0,1,2,3
;At samplepoint 4,5,6 the input is allowed to change.
;Samplepoint 7 is taken as reference, the next edge in the input signal denotes the mid-bit point.
;If no midbit is found at samplepoint 13, the input data is invalid.

;The state machine syncronizes with the tramsitions 
;in the middle of the received bits


