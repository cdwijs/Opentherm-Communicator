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

;On the PCB, the input is inverted. <9.5V is seen by the uC as a 1.
;Idle level is >9.5 
;At the uC pin, the following bitstream is seen. Each char represents 500uSec.
;For clarity, a space is added after each msec

;1111111 01 01 10 10 10 10 10 10 10 10 10 10 01 01 10 10 01 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 01 1111111
;logical:1  1  0  0  0  0  0  0  0  0  0  0  1  1  0  0  1  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  1  
;           D31   D29   D27   D25   D23   D21   D19   D17   D15   D13   D11   D9    D7    D5    D3    D1      
;        Start D30   D28   D26   D24   D22   D20   D18   D16   D14   D12   D10   D8    D6    D4    D2    D0 Stop

;we store the bit after we have seen the transition in the middle. A 10 is stored as a 0, a 01 is stored as a 1 

;correct below comments:
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
;For example, the sequence of bits 110100110 is represented as:
;           ___     ______        ______     ___        ___     ______       
;          |   |   |      |      |      |   |   |      |   |   |      |    
;__________|   |___|      |______|      |___|   |______|   |___|      |____
;
;       |      |       |      |      |      |      |       |       |      |
;       |      |       |      |      |      |      |       |       |      |
;          1      1      0       1       0     0       1       1       0   
;Idle    Start   D0      D1      D2      D3    D4

;The bitstream is sampled every 100uSec.
;Manchester code has always a transition in the middle of the transmitted bit.
;In order to compensate for clock drift the code syncronizes at the middle of 
;the received bits.
;
;A perfectly clocked 1000 usec bit looks like this:
;           0                   1
;        -1 0 1 2 3 4 5 6 7 8 9 0
;           _________           __    
;          |         |         |    
;__________|         |_________|
;
;A 800usc bit (25% too fast) input signal looks like this:
;           0                   1
;        -1 0 1 2 3 4 5 6 7 8 9 0
;           _______         __    
;          |       |       |    
;__________|       |_______|
;Samplepoints 0,1,2,3 are stable
;At samplepoint 8, the mid-bit point is found.
;
;A 1200 usec bit (20% too slow) looks like this 
;           0                   1 1 1
;        -1 0 1 2 3 4 5 6 7 8 9 0 1 2
;           ___________             __    
;          |           |           |    
;__________|           |___________|
;Samplepoints 0,1,2,3,4,5 are stable
;At samplepoint 12, the mid-bit point is found. 

;At samplepoint 0, the first rising edge is found. This is the mid-bit point.
;The input is not allowed to change at samplepoint 0,1,2,3
;At samplepoint 4,5,6,7 the input is allowed to change.
;Samplepoint 7 is taken as reference, the next edge in the input signal denotes the mid-bit point.
;If no midbit is found at samplepoint 13, the input data is invalid.

;The state machine syncronizes with the tramsitions 
;in the middle of the received bits

#define NUM_TX_BITS	d'32'
#define NUM_RX_BITS d'32'

SwitchCase2:
		andlw	h'01'
SwitchCase4:
		andlw	h'03'
Switchcase8:
		andlw	h'07'
Switchcase16:
		andlw	h'0F'
        rlncf   WREG ;cdw, does not work when access or banked is specified?
        addwf   TOSL
        movlw   0
        addwfc  TOSH
        addwfc  TOSU            ; for >64K bytes memory
        return 

;jumptable StateTx1
int100usec:
	bcf	PIR1,TMR2IF,ACCESS
	;sample input, edge detect
#ifdef INPUTS_ON_PORTA
	#ifdef DEBUG_LOOPBACK_TEST
		movff	LATB,myPortaNow
	#else
		movff	PORTA,myPortaNow
	#endif
	movf	myPortaNow,w,BANKED
	xorwf	myPortaPrevious,w,BANKED
	movwf	myPortaEdge,BANKED
	movff	myPortaNow,myPortaPrevious
#endif
#ifdef INPUTS_ON_PORTB
	movff	PORTB,myPortbNow
	movf	myPortbNow,w,BANKED
	xorwf	myPortbPrevious,w,BANKED
	movwf	myPortbEdge,BANKED
	movff	myPortbNow,myPortbPrevious
#endif
#ifdef INPUTS_ON_PORTC
	movff	PORTC,myPortcNow
	movf	myPortcNow,w,BANKED
	xorwf	myPortcPrevious,w,BANKED
	movwf	myPortcEdge,BANKED
	movff	myPortcNow,myPortcPrevious
#endif
#ifdef INPUTS_ON_PORTD
	movff	PORTD,myPortdNow
	movf	myPortdNow,w,BANKED
	xorwf	myPortdPrevious,w,BANKED
	movwf	myPortdEdge,BANKED
	movff	myPortdNow,myPortdPrevious
#endif
#ifdef INPUTS_ON_PORTE
	movff	PORTE,myPorteNow
	movf	myPorteNow,w,BANKED
	xorwf	myPortePrevious,w,BANKED
	movwf	myPorteEdge,BANKED
	movff	myPorteNow,myPortePrevious
#endif

	;debug verify interrupt timing
	movlw	PRESCALE_1MS
	decfsz	myPrescale1msec,f,BANKED
	bra		timerDone	
	movwf	myPrescale1msec,BANKED
	;here 1msec has passed
;cdw, move timers to better place
#ifdef USE_CHANNEL_0
	movf	myComm0Timer,f,BANKED
	bz		timercomm0done
	decf	myComm0Timer,f,BANKED
timercomm0done:
#endif
#ifdef USE_CHANNEL_1
	movf	myComm1Timer,f,BANKED
	bz		timercomm1done
	decf	myComm1Timer,f,BANKED
timercomm1done:
#endif
#ifdef USE_CHANNEL_2
	movf	myComm2Timer,f,BANKED
	bz		timercomm2done
	decf	myComm2Timer,f,BANKED
timercomm2done:
#endif
#ifdef USE_CHANNEL_3
	movf	myComm3Timer,f,BANKED
	bz		timercomm3done
	decf	myComm3Timer,f,BANKED
timercomm3done:
#endif
#ifdef USE_CHANNEL_4
	movf	myComm4Timer,f,BANKED
	bz		timercomm4done
	decf	myComm4Timer,f,BANKED
timercomm4done:
#endif
#ifdef USE_CHANNEL_5
	movf	myComm5Timer,f,BANKED
	bz		timercomm5done
	decf	myComm5Timer,f,BANKED
timercomm5done:
#endif
#ifdef USE_CHANNEL_6
	movf	myComm6Timer,f,BANKED
	bz		timercomm6done
	decf	myComm6Timer,f,BANKED
timercomm6done:
#endif
#ifdef USE_CHANNEL_7
	movf	myComm7Timer,f,BANKED
	bz		timercomm7done
	decf	myComm7Timer,f,BANKED
timercomm7done:
#endif
	movlw	PRESCALE_125MS
	decfsz	myPrescale125msec,f,BANKED
	bra		timerDone
	movwf	myPrescale125msec,BANKED		
	;here 125msec has passed

	movlw	PRESCALE_500MS
	decfsz	myPrescale500msec,f,BANKED
	bra		timerDone
	movwf	myPrescale500msec,BANKED		
	;here 500msec has passed
timerDone:
	;end debug
	;run each transmitter @500usec
	movf	myPrescaleTx,w,BANKED
	call	Switchcase8
	bra		tx0Jump				;0	run 0&1
	bra		tx2Jump				;1	run 2&3
	bra		tx4Jump				;2	run 4&5
	bra		tx6Jump				;3	run 6&7
	clrf	myPrescaleTx,BANKED	;4	run nothing
	clrf	myPrescaleTx,BANKED	;5	run nothing
	clrf	myPrescaleTx,BANKED	;6	run nothing
	clrf	myPrescaleTx,BANKED	;7	run nothing
	bra		txJumpsNoInc

tx0Jump:
#ifdef USE_CHANNEL_0
	;labels
	#define txState0		tx0State0	;transmit
	#define txState1		tx0State1
	#define txState2		tx0State2
	#define txState3		tx0State3
	#define txState4		tx0State4
	#define txState5		tx0State5
	#define txState6		tx0State6
	#define txState7		tx0State7
	#define txNextState		tx0NextState
	#define txDone			tx0Done
	;data
	#define myTxState		myTx0State
	#define myTxBuffer		myTx0Buffer
	#define myTxCounter		myTx0Counter
	;bits
	#define bitTxBusy		bitTx0Busy
	#define bitTxStart		bitTx0Start
	#define pinOutTx		pinOutTx0
	#include manchester-Tx-States.inc
#endif

tx1Jump:
#ifdef USE_CHANNEL_1
	;labels
	#define txState0		tx1State0
	#define txState1		tx1State1
	#define txState2		tx1State2
	#define txState3		tx1State3
	#define txState4		tx1State4
	#define txState5		tx1State5
	#define txState6		tx1State6
	#define txState7		tx1State7
	#define txNextState		tx1NextState
	#define txDone			tx1Done
	;data
	#define myTxState		myTx1State
	#define myTxBuffer		myTx1Buffer
	#define myTxCounter		myTx1Counter
	;bits
	#define bitTxBusy		bitTx1Busy
	#define bitTxStart		bitTx1Start
	#define pinOutTx		pinOutTx1
	#include manchester-Tx-States.inc
#endif
	bra	txjumpsDone

tx2Jump:
#ifdef USE_CHANNEL_2
	;labels
	#define txState0		tx2State0
	#define txState1		tx2State1
	#define txState2		tx2State2
	#define txState3		tx2State3
	#define txState4		tx2State4
	#define txState5		tx2State5
	#define txState6		tx2State6
	#define txState7		tx2State7
	#define txNextState		tx2NextState
	#define txDone			tx2Done
	;data
	#define myTxState		myTx2State
	#define myTxBuffer		myTx2Buffer
	#define myTxCounter		myTx2Counter
	;bits
	#define bitTxBusy		bitTx2Busy
	#define bitTxStart		bitTx2Start
	#define pinOutTx		pinOutTx2
	#include manchester-Tx-States.inc
#endif

tx3Jump:
#ifdef USE_CHANNEL_3
	;labels
	#define txState0		tx3State0
	#define txState1		tx3State1
	#define txState2		tx3State2
	#define txState3		tx3State3
	#define txState4		tx3State4
	#define txState5		tx3State5
	#define txState6		tx3State6
	#define txState7		tx3State7
	#define txNextState		tx3NextState
	#define txDone			tx3Done
	;data
	#define myTxState		myTx3State
	#define myTxBuffer		myTx3Buffer
	#define myTxCounter		myTx3Counter
	;bits
	#define bitTxBusy		bitTx3Busy
	#define bitTxStart		bitTx3Start
	#define pinOutTx		pinOutTx3
	#include manchester-Tx-States.inc
#endif
	bra	txjumpsDone
tx4Jump:
#ifdef USE_CHANNEL_4
	;labels
	#define txState0		tx4State0
	#define txState1		tx4State1
	#define txState2		tx4State2
	#define txState3		tx4State3
	#define txState4		tx4State4
	#define txState5		tx4State5
	#define txState6		tx4State6
	#define txState7		tx4State7
	#define txNextState		tx4NextState
	#define txDone			tx4Done
	;data
	#define myTxState		myTx4State
	#define myTxBuffer		myTx4Buffer
	#define myTxCounter		myTx4Counter
	;bits
	#define bitTxBusy		bitTx4Busy
	#define bitTxStart		bitTx4Start
	#define pinOutTx		pinOutTx4
	#include manchester-Tx-States.inc
#endif
tx5Jump:
#ifdef USE_CHANNEL_5
	;labels
	#define txState0		tx5State0
	#define txState1		tx5State1
	#define txState2		tx5State2
	#define txState3		tx5State3
	#define txState4		tx5State4
	#define txState5		tx5State5
	#define txState6		tx5State6
	#define txState7		tx5State7
	#define txNextState		tx5NextState
	#define txDone			tx5Done
	;data
	#define myTxState		myTx5State
	#define myTxBuffer		myTx5Buffer
	#define myTxCounter		myTx5Counter
	;bits
	#define bitTxBusy		bitTx5Busy
	#define bitTxStart		bitTx5Start
	#define pinOutTx		pinOutTx5
	#include manchester-Tx-States.inc
#endif
	bra	txjumpsDone
tx6Jump:
#ifdef USE_CHANNEL_6
	;labels
	#define txState0		tx6State0
	#define txState1		tx6State1
	#define txState2		tx6State2
	#define txState3		tx6State3
	#define txState4		tx6State4
	#define txState5		tx6State5
	#define txState6		tx6State6
	#define txState7		tx6State7
	#define txNextState		tx6NextState
	#define txDone			tx6Done
	;data
	#define myTxState		myTx6State
	#define myTxBuffer		myTx6Buffer
	#define myTxCounter		myTx6Counter
	;bits
	#define bitTxBusy		bitTx6Busy
	#define bitTxStart		bitTx6Start
	#define pinOutTx		pinOutTx6
	#include manchester-Tx-States.inc
#endif
tx7Jump:
#ifdef USE_CHANNEL_7
	;labels
	#define txState0		tx7State0
	#define txState1		tx7State1
	#define txState2		tx7State2
	#define txState3		tx7State3
	#define txState4		tx7State4
	#define txState5		tx7State5
	#define txState6		tx7State6
	#define txState7		tx7State7
	#define txNextState		tx7NextState
	#define txDone			tx7Done
	;data
	#define myTxState		myTx7State
	#define myTxBuffer		myTx7Buffer
	#define myTxCounter		myTx7Counter
	;bits
	#define bitTxBusy		bitTx7Busy
	#define bitTxStart		bitTx7Start
	#define pinOutTx		pinOutTx7
	#include manchester-Tx-States.inc
#endif
	bra	txjumpsDone

txjumpsDone:
	incf	myPrescaleTx,f,BANKED
txJumpsNoInc:
	

#ifdef USE_CHANNEL_0
	;labels
	#define rxState0			rx0State0	;receive
	#define rxState1			rx0State1
	#define rxState2			rx0State2
	#define rxState3			rx0State3
	#define rxState4			rx0State4
	#define rxState5			rx0State5
	#define rxState6			rx0State6
	#define rxState7			rx0State7
	#define rxState8			rx0State8
	#define rxState9			rx0State9
	#define rxState10			rx0State10
	#define rxState11			rx0State11
	#define rxState12			rx0State12
	#define rxState13			rx0State13
	#define rxState14			rx0State14
	#define rxState15			rx0State15
	#define rxStoreBit			rx0StoreBit
	#define rxErrorLateEdge		rx0ErrorLateEdge
	#define rxErrorFastEdge		rx0ErrorFastEdge
	#define rxSetIdleState		rx0SetIdleState
	#define rxNextState			rx0NextState
	#define rxDone				rx0Done
	;data
	#define myRxState			myRx0State
	#define myRxCounter			myRx0Counter
	#define myRxBuffer			myRx0Buffer
	;bits
	#define bitRxBusy			bitRx0Busy
	#define bitRxStart			bitRx0Start
	#define pinInRx				pinInRx0
	#define edgeRx				edgeRx0
	#include manchester-Rx-States.inc
#endif

#ifdef USE_CHANNEL_1
	;labels
	#define rxState0			rx1State0	;receive
	#define rxState1			rx1State1
	#define rxState2			rx1State2
	#define rxState3			rx1State3
	#define rxState4			rx1State4
	#define rxState5			rx1State5
	#define rxState6			rx1State6
	#define rxState7			rx1State7
	#define rxState8			rx1State8
	#define rxState9			rx1State9
	#define rxState10			rx1State10
	#define rxState11			rx1State11
	#define rxState12			rx1State12
	#define rxState13			rx1State13
	#define rxState14			rx1State14
	#define rxState15			rx1State15
	#define rxStoreBit			rx1StoreBit
	#define rxErrorLateEdge		rx1ErrorLateEdge
	#define rxErrorFastEdge		rx1ErrorFastEdge
	#define rxSetIdleState		rx1SetIdleState
	#define rxNextState			rx1NextState
	#define rxDone				rx1Done
	;data
	#define myRxState			myRx1State
	#define myRxCounter			myRx1Counter
	#define myRxBuffer			myRx1Buffer
	;bits
	#define bitRxBusy			bitRx1Busy
	#define bitRxStart			bitRx1Start
	#define pinInRx				pinInRx1
	#define edgeRx				edgeRx1
	#include manchester-Rx-States.inc
#endif

#ifdef USE_CHANNEL_2
	;labels
	#define rxState0			rx2State0	;receive
	#define rxState1			rx2State1
	#define rxState2			rx2State2
	#define rxState3			rx2State3
	#define rxState4			rx2State4
	#define rxState5			rx2State5
	#define rxState6			rx2State6
	#define rxState7			rx2State7
	#define rxState8			rx2State8
	#define rxState9			rx2State9
	#define rxState10			rx2State10
	#define rxState11			rx2State11
	#define rxState12			rx2State12
	#define rxState13			rx2State13
	#define rxState14			rx2State14
	#define rxState15			rx2State15
	#define rxStoreBit			rx2StoreBit
	#define rxErrorLateEdge		rx2ErrorLateEdge
	#define rxErrorFastEdge		rx2ErrorFastEdge
	#define rxSetIdleState		rx2SetIdleState
	#define rxNextState			rx2NextState
	#define rxDone				rx2Done
	;data
	#define myRxState			myRx2State
	#define myRxCounter			myRx2Counter
	#define myRxBuffer			myRx2Buffer
	;bits
	#define bitRxBusy			bitRx2Busy
	#define bitRxStart			bitRx2Start
	#define pinInRx				pinInRx2
	#define edgeRx				edgeRx2	
	#include manchester-Rx-States.inc
#endif

#ifdef USE_CHANNEL_3
	;labels
	#define rxState0			rx3State0	;receive
	#define rxState1			rx3State1
	#define rxState2			rx3State2
	#define rxState3			rx3State3
	#define rxState4			rx3State4
	#define rxState5			rx3State5
	#define rxState6			rx3State6
	#define rxState7			rx3State7
	#define rxState8			rx3State8
	#define rxState9			rx3State9
	#define rxState10			rx3State10
	#define rxState11			rx3State11
	#define rxState12			rx3State12
	#define rxState13			rx3State13
	#define rxState14			rx3State14
	#define rxState15			rx3State15
	#define rxStoreBit			rx3StoreBit
	#define rxErrorLateEdge		rx3ErrorLateEdge
	#define rxErrorFastEdge		rx3ErrorFastEdge
	#define rxSetIdleState		rx3SetIdleState
	#define rxNextState			rx3NextState
	#define rxDone				rx3Done
	;data
	#define myRxState			myRx3State
	#define myRxCounter			myRx3Counter
	#define myRxBuffer			myRx3Buffer
	;bits
	#define bitRxBusy			bitRx3Busy
	#define bitRxStart			bitRx3Start
	#define pinInRx				pinInRx3
	#define edgeRx				edgeRx3
	#include manchester-Rx-States.inc
#endif

#ifdef USE_CHANNEL_4
	;labels
	#define rxState0			rx4State0	;receive
	#define rxState1			rx4State1
	#define rxState2			rx4State2
	#define rxState3			rx4State3
	#define rxState4			rx4State4
	#define rxState5			rx4State5
	#define rxState6			rx4State6
	#define rxState7			rx4State7
	#define rxState8			rx4State8
	#define rxState9			rx4State9
	#define rxState10			rx4State10
	#define rxState11			rx4State11
	#define rxState12			rx4State12
	#define rxState13			rx4State13
	#define rxState14			rx4State14
	#define rxState15			rx4State15
	#define rxStoreBit			rx4StoreBit
	#define rxErrorLateEdge		rx4ErrorLateEdge
	#define rxErrorFastEdge		rx4ErrorFastEdge
	#define rxSetIdleState		rx4SetIdleState
	#define rxNextState			rx4NextState
	#define rxDone				rx4Done
	;data
	#define myRxState			myRx4State
	#define myRxCounter			myRx4Counter
	#define myRxBuffer			myRx4Buffer
	;bits
	#define bitRxBusy			bitRx4Busy
	#define bitRxStart			bitRx4Start
	#define pinInRx				pinInRx4
	#define edgeRx				edgeRx4
	#include manchester-Rx-States.inc
#endif

#ifdef USE_CHANNEL_5
	;labels
	#define rxState0			rx5State0	;receive
	#define rxState1			rx5State1
	#define rxState2			rx5State2
	#define rxState3			rx5State3
	#define rxState4			rx5State4
	#define rxState5			rx5State5
	#define rxState6			rx5State6
	#define rxState7			rx5State7
	#define rxState8			rx5State8
	#define rxState9			rx5State9
	#define rxState10			rx5State10
	#define rxState11			rx5State11
	#define rxState12			rx5State12
	#define rxState13			rx5State13
	#define rxState14			rx5State14
	#define rxState15			rx5State15
	#define rxStoreBit			rx5StoreBit
	#define rxErrorLateEdge		rx5ErrorLateEdge
	#define rxErrorFastEdge		rx5ErrorFastEdge
	#define rxSetIdleState		rx5SetIdleState
	#define rxNextState			rx5NextState
	#define rxDone				rx5Done
	;data
	#define myRxState			myRx5State
	#define myRxCounter			myRx5Counter
	#define myRxBuffer			myRx5Buffer
	;bits
	#define bitRxBusy			bitRx5Busy
	#define bitRxStart			bitRx5Start
	#define pinInRx				pinInRx5
	#define edgeRx				edgeRx5
	#include manchester-Rx-States.inc
#endif

#ifdef USE_CHANNEL_6
	;labels
	#define rxState0			rx6State0	;receive
	#define rxState1			rx6State1
	#define rxState2			rx6State2
	#define rxState3			rx6State3
	#define rxState4			rx6State4
	#define rxState5			rx6State5
	#define rxState6			rx6State6
	#define rxState7			rx6State7
	#define rxState8			rx6State8
	#define rxState9			rx6State9
	#define rxState10			rx6State10
	#define rxState11			rx6State11
	#define rxState12			rx6State12
	#define rxState13			rx6State13
	#define rxState14			rx6State14
	#define rxState15			rx6State15
	#define rxStoreBit			rx6StoreBit
	#define rxErrorLateEdge		rx6ErrorLateEdge
	#define rxErrorFastEdge		rx6ErrorFastEdge
	#define rxSetIdleState		rx6SetIdleState
	#define rxNextState			rx6NextState
	#define rxDone				rx6Done
	;data
	#define myRxState			myRx6State
	#define myRxCounter			myRx6Counter
	#define myRxBuffer			myRx6Buffer
	;bits
	#define bitRxBusy			bitRx6Busy
	#define bitRxStart			bitRx6Start
	#define pinInRx				pinInRx6
	#define edgeRx				edgeRx6
	#include manchester-Rx-States.inc
#endif

#ifdef USE_CHANNEL_7
	;labels
	#define rxState0			rx7State0	;receive
	#define rxState1			rx7State1
	#define rxState2			rx7State2
	#define rxState3			rx7State3
	#define rxState4			rx7State4
	#define rxState5			rx7State5
	#define rxState6			rx7State6
	#define rxState7			rx7State7
	#define rxState8			rx7State8
	#define rxState9			rx7State9
	#define rxState10			rx7State10
	#define rxState11			rx7State11
	#define rxState12			rx7State12
	#define rxState13			rx7State13
	#define rxState14			rx7State14
	#define rxState15			rx7State15
	#define rxStoreBit			rx7StoreBit
	#define rxErrorLateEdge		rx7ErrorLateEdge
	#define rxErrorFastEdge		rx7ErrorFastEdge
	#define rxSetIdleState		rx7SetIdleState
	#define rxNextState			rx7NextState
	#define rxDone				rx7Done
	;data
	#define myRxState			myRx7State
	#define myRxCounter			myRx7Counter
	#define myRxBuffer			myRx7Buffer
	;bits
	#define bitRxBusy			bitRx7Busy
	#define bitRxStart			bitRx7Start
	#define pinInRx				pinInRx7
	#define edgeRx				edgeRx7
	#include manchester-Rx-States.inc
#endif

#ifdef TIMING_CHECK
	movf	TMR2,w,ACCESS
	cpfsgt	myPr2Max,BANKED
	movwf	myPr2Max,BANKED	
#endif
	return