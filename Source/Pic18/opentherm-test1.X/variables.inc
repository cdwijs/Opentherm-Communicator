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
#define RAMSTART_ACCESS	h'00'
#define SERIAL_BUF_SIZE h'20' ;must be power of 2
#define SERIAL_BUF_MASK (SERIAL_BUF_SIZE-1)

	cblock	RAMSTART_ACCESS
		myPrescale1msec:1		;timing
		myPrescale125msec:1
		myPrescale500msec:1
		myPrescaleTx
	endc

	cblock
		myRs232TxBuf:SERIAL_BUF_SIZE	;serial port
		myRs232TxProducer
		myRs232TxConsumer
	endc


	cblock
		reg0:1	
		reg1:1
	endc
	
	#ifdef INPUTS_ON_PORTA
		cblock
		myPortaEdge:1			;edge detector
		myPortaNow:1		
		myPortaPrevious:1
		endc
	#endif
	#ifdef INPUTS_ON_PORTB
		cblock
		myPortbEdge:1			;edge detector
		myPortbNow:1
		myPortbPrevious:1
		endc		
	#endif
	#ifdef INPUTS_ON_PORTC
		cblock
		myPortcNow:1			;edge detector
		myPortcEdge:1
		myPortcPrevious:1
		endc
	#endif
	#ifdef INPUTS_ON_PORTD
		cblock
		myPortdEdge:1			;edge detector
		myPortdNow:1
		myPortdPrevious:1
		endc
	#endif
	#ifdef INPUTS_ON_PORTE
		cblock
		myPorteEdge:1			;edge detector
		myPorteNow:1
		myPortePrevious:1
		endc
	#endif
	
	#ifdef USE_CHANNEL_0
		#define bitTx0Start 		my0Flags,0
		#define bitTx0Busy			my0Flags,1
		#define bitRx0Start			my0Flags,2
		#define bitRx0Busy			my0Flags,3
		#define bitComm0StartMsg	my0Flags,4
		#define bitComm0Busy		my0Flags,5
		cblock
			my0Flags:1
			myTx0State:1
			myTx0Counter:1
			myTx0Buffer:4
			myRx0State:1
			myRx0Counter:1
			myRx0Buffer:4
			myComm0State:1
			myComm0Timer:1
		endc
	#endif
	#ifdef USE_CHANNEL_1
		#define bitTx1Start 		my1Flags,0
		#define bitTx1Busy			my1Flags,1
		#define bitRx1Start			my1Flags,2
		#define bitRx1Busy			my1Flags,3
		#define bitComm1StartMsg	my1Flags,4
		#define bitComm1Busy		my1Flags,5
		cblock
			my1Flags:1
			myTx1State:1
			myTx1Counter:1
			myTx1Buffer:4
			myRx1State:1
			myRx1Counter:1
			myRx1Buffer:4
			myComm1State:1
			myComm1Timer:1
		endc
	#endif
	#ifdef USE_CHANNEL_2
		#define bitTx2Start 		my2Flags,0
		#define bitTx2Busy			my2Flags,1
		#define bitRx2Start			my2Flags,2
		#define bitRx2Busy			my2Flags,3
		#define bitComm2StartMsg	my2Flags,4
		#define bitComm2Busy		my2Flags,5
		cblock
			my2Flags:1
			myTx2State:1
			myTx2Counter:1
			myTx2Buffer:4
			myRx2State:1
			myRx2Counter:1
			myRx2Buffer:4
			myComm2State:1
			myComm2Timer:1
		endc
	#endif





#define RAMSTART_BANK0 h'0000'
variablesInit:
	lfsr	FSR0, RAMSTART_BANK0
varLoop:
	clrf	POSTINC0
	btfss	FSR0H,1		;stop at the begin of bank2 (0x0200)
	bra		varLoop		;writes to uminplemented locations are ignored
	return

