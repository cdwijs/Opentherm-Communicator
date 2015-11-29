;*******************************************************************************
;    variables.inc                                                             *
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
#define RAMSTART_ACCESS	h'00'
#define SERIAL_BUF_SIZE h'20' ;must be power of 2
#define SERIAL_BUF_MASK (SERIAL_BUF_SIZE-1)
#define HIGH_BYTE	d'1' ;for constructions like: 
#define LOW_BYTE	d'0' ;movf	myCmdSetpoint+LOW_BYTE,w,BANKED


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
		myRs232RxBuf:SERIAL_BUF_SIZE	;serial port
		myRs232RxProducer
		myRs232RxConsumer
	endc
	cblock
		myCmdState:1	;serial command handler
		myCmdThermostat
		myCmdData:2
		myCmd
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
		#define bit0SymBoil		my0Flags,6
		#define bit0SymFlame		my0Flags,7
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
			myCmd0Setpoint:2
		endc
	#endif
	#ifdef USE_CHANNEL_1
		#define bitTx1Start 		my1Flags,0
		#define bitTx1Busy			my1Flags,1
		#define bitRx1Start			my1Flags,2
		#define bitRx1Busy			my1Flags,3
		#define bitComm1StartMsg	my1Flags,4
		#define bitComm1Busy		my1Flags,5
		#define bit1SymBoil		my1Flags,6
		#define bit1SymFlame		my1Flags,7
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
			myCmd1Setpoint:2
		endc
	#endif
	#ifdef USE_CHANNEL_2
		#define bitTx2Start 		my2Flags,0
		#define bitTx2Busy			my2Flags,1
		#define bitRx2Start			my2Flags,2
		#define bitRx2Busy			my2Flags,3
		#define bitComm2StartMsg	my2Flags,4
		#define bitComm2Busy		my2Flags,5
		#define bit2SymBoil		my2Flags,6
		#define bit2SymFlame		my2Flags,7
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
			myCmd2Setpoint:2
		endc
	#endif
	#ifdef USE_CHANNEL_3
		#define bitTx3Start 		my3Flags,0
		#define bitTx3Busy			my3Flags,1
		#define bitRx3Start			my3Flags,2
		#define bitRx3Busy			my3Flags,3
		#define bitComm3StartMsg	my3Flags,4
		#define bitComm3Busy		my3Flags,5
		#define bit3SymBoil		my3Flags,6
		#define bit3SymFlame		my3Flags,7
		cblock
			my3Flags:1
			myTx3State:1
			myTx3Counter:1
			myTx3Buffer:4
			myRx3State:1
			myRx3Counter:1
			myRx3Buffer:4
			myComm3State:1
			myComm3Timer:1
			myCmd3Setpoint:2
		endc
	#endif
	#ifdef USE_CHANNEL_4
		#define bitTx4Start 		my4Flags,0
		#define bitTx4Busy			my4Flags,1
		#define bitRx4Start			my4Flags,2
		#define bitRx4Busy			my4Flags,3
		#define bitComm4StartMsg	my4Flags,4
		#define bitComm4Busy		my4Flags,5
		#define bit4SymBoil		my4Flags,6
		#define bit4SymFlame		my4Flags,7
		cblock
			my4Flags:1
			myTx4State:1
			myTx4Counter:1
			myTx4Buffer:4
			myRx4State:1
			myRx4Counter:1
			myRx4Buffer:4
			myComm4State:1
			myComm4Timer:1
			myCmd4Setpoint:2
		endc
	#endif
	#ifdef USE_CHANNEL_5
		#define bitTx5Start 		my5Flags,0
		#define bitTx5Busy			my5Flags,1
		#define bitRx5Start			my5Flags,2
		#define bitRx5Busy			my5Flags,3
		#define bitComm5StartMsg	my5Flags,4
		#define bitComm5Busy		my5Flags,5
		#define bit5SymBoil		my5Flags,6
		#define bit5SymFlame		my5Flags,7
		cblock
			my5Flags:1
			myTx5State:1
			myTx5Counter:1
			myTx5Buffer:4
			myRx5State:1
			myRx5Counter:1
			myRx5Buffer:4
			myComm5State:1
			myComm5Timer:1
			myCmd5Setpoint:2
		endc
	#endif
	#ifdef USE_CHANNEL_6
		#define bitTx6Start 		my6Flags,0
		#define bitTx6Busy			my6Flags,1
		#define bitRx6Start			my6Flags,2
		#define bitRx6Busy			my6Flags,3
		#define bitComm6StartMsg	my6Flags,4
		#define bitComm6Busy		my6Flags,5
		#define bit6SymBoil		my6Flags,6
		#define bit6SymFlame		my6Flags,7
		cblock
			my6Flags:1
			myTx6State:1
			myTx6Counter:1
			myTx6Buffer:4
			myRx6State:1
			myRx6Counter:1
			myRx6Buffer:4
			myComm6State:1
			myComm6Timer:1
			myCmd6Setpoint:2
		endc
	#endif
	#ifdef USE_CHANNEL_7
		#define bitTx7Start 		my7Flags,0
		#define bitTx7Busy			my7Flags,1
		#define bitRx7Start			my7Flags,2
		#define bitRx7Busy			my7Flags,3
		#define bitComm7StartMsg	my7Flags,4
		#define bitComm7Busy		my7Flags,5
		#define bit7SymBoil		my7Flags,6
		#define bit7SymFlame		my7Flags,7
		cblock
			my7Flags:1
			myTx7State:1
			myTx7Counter:1
			myTx7Buffer:4
			myRx7State:1
			myRx7Counter:1
			myRx7Buffer:4
			myComm7State:1
			myComm7Timer:1
			myCmd7Setpoint:2
		endc
	#endif

	#ifdef TIMING_CHECK
		cblock
			myPr2Max
		endc
	#endif

#define RAMSTART_BANK0 h'0000'
variablesInit:
	lfsr	FSR0, RAMSTART_BANK0
varLoop:
	clrf	POSTINC0
	btfss	FSR0H,1		;stop at the begin of bank2 (0x0200)
	bra		varLoop		;writes to uminplemented locations are ignored

#ifdef DEBUG_LOOPBACK_TEST 
	movlw	d'4' ;ot-comm will now add parity and then send it.
	movwf	myComm0State
	movwf	myComm1State
	movwf	myComm2State
	movwf	myComm3State
	movwf	myComm4State
	movwf	myComm5State	
	movwf	myComm6State
	movwf	myComm7State
#endif
	return

