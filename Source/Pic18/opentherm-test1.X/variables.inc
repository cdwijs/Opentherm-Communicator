;*******************************************************************************
;    variables.inc                                                             *
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
#define bitTx0Start myTx0Flags,0	;move these bits together, not per-transmitter
#define bitTx0Busy	myTx0Flags,1
#define bitRx0Start	myRx0Flags,0
#define bitRx0Busy	myRx0Flags,1

#define RAMSTART_ACCESS	0x00
	cblock	RAMSTART_ACCESS	
		;manchester transmitters
		myTx0State:1
		myTx0Counter:1
		myTx0Flags:1
		myTx0Buffer:4
		;manchester receivers
		myRx0State:1
		myRx0Counter:1
		myRx0Flags:1
		myRx0Buffer:4
		;edge detectors
		myPortaEdge:1		;create #ifdef construction for unused ports
		myPortaNow:1		
		myPortaPrevious:1

		myPortbEdge:1
		myPortbNow:1
		myPortbPrevious:1		

		myPortcNow:1
		myPortcEdge:1
		myPortcPrevious:1

		myPortdEdge:1
		myPortdNow:1
		myPortdPrevious:1

		myPorteEdge:1
		myPorteNow:1
		myPortePrevious:1

		;timing
		myPrescale1msec:1
		myPrescale125msec:1
		myPrescale500msec:1
		myPrescale500usec:1
	endc

#define RAMSTART_BANK0 h'0000'
variablesInit:
	lfsr	FSR0, RAMSTART_BANK0
varLoop:
	clrf	POSTINC0
	btfss	FSR0H,1		;stop at the begin of bank2 (0x0200)
	bra		varLoop		;writes to uminplemented locations are ignored
	return

