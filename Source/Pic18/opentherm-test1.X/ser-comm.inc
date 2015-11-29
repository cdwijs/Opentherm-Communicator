;*******************************************************************************
;    ser-comm.inc                                                              *
;    Copyright 2013-2015 Cedric de Wijs.                                       *
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
;This state machine handles commands from the serial port.
;The following commands are implemented:
;Update room setpoint.
;Set flame symbol
;Set boiler symbol
;Set Home signal
;Heartbeat


#define commStateIdle			commState0
#define COMM_STATE_IDLE			d'0'
#define commStateWaitAnswer		commState1
#define commStateCheckMessageA	commState2
#define commStateCheckMessageID	commState3
#define commStateSetupAnswer	commState4
#define commStateReport			commState5
#define commStateWaitSend		commState6
#define commStateSendAnswer		commState7

	movf	myCommState,w,BANKED
	call	Switchcase8
	bra		commState0
	bra		commState1
	bra		commState2
	bra		commState3
	bra		commState4
	bra		commState5
	bra		commState6
	bra		commState7