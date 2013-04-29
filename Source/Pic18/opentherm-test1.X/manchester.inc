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


;Transmit state machine:
; 
;       +-----------+
;       |Idle       |
;       |-----------|
;       |output=0   |
; +---->|BitTxBusy=0|
; |     +-----+-----+
; |           |
; |           |BitTxStart
; |           |
; |     +-----v-----+
; |     |SendStartA |
; |     |-----------|
; |     |output=0   |
; |     |BitTxBusy=1|
; |     +-----+-----+
; |           |
; |           |500us
; |           |
; |     +-----v-----+
; |     |SendStartB |
; |     |-----------|
; |     |output=1   |
; |     |           |
; |     +-----+-----+
; |           |
; |           |500us
; |           |
; |     +-----v-----+
; |     |SendBitA   |
; |     |-----------|
; |     |output=!Bit|
; |     |           |<---+
; |     +-----+-----+    |
; |           |          |
; |           |500us     |
; |           |          |
; |     +-----v-----+    |
; |     |SendBitB   |    |
; |     |-----------|    |
; |     |output=Bit |    |
; |     |           |    |
; |     +-----+-----+    |
; |           |500us     |
; |           |          |
; |           |  <32 bits|
; |           +--------->+
; |           |32 bits   
; |           |          
; |     +-----v-----+    
; |     |SendStopA  |
; |     |-----------|
; |     |output=1   |
; |     |           |
; |     +-----+-----+
; |           |
; |           |500us
; |           |
; |     +-----v-----+
; |     |SendStopB  |
; |     |-----------|
; |     |output=0   |
; |     |           |
; |     +-----+-----+
; |           |
; |<----------+


;Receive state machine
; 
;       +-----------+
;       |Idle       |
;       |-----------|
;       |           |
; +---->|BitRxBusy=0|<---------+
; |     +-----+-----+          |
; |           |BitStartRx      |
; |     +-----v-----+          |
; |     |WaitStart  |          |
; |     |-----------|          |
; |     |           |          |
; |     |BitRxBusy=1|          |
; |     +-----------+          |
; |BitRxAbort |                |
; |<----------+                |
; |           |Edge            |
; |           |<---------------|------+
; |     +-----v-----+          |      |
; |     |Sample 1   |          |      |
; |     |-----------|          |      |
; |32bit|           |edge      |      |
; <-----+           +---->Error+      |
;       +-----+-----+          |      |
;             |100us           |      |
;       +-----v-----+          |      |
;       |Sample 2   |          |      |
;       |-----------|          |      |
;       |           |edge      |      |
;       |           +---->Error+      |
;       +-----+-----+          |      |
;             |100us           |      |
;       +-----v-----+          |      |
;       |Sample 3   |          |      |
;       |-----------|          |      |
;       |           |Edge      |      |
;       |           +---->Error+      |
;       +-----+-----+                 |
;             |100us                  |
;       +-----v-----+                 |
;       |Sample 4   |                 |
;       |-----------|                 |
;       |           |                 |
;       |           |                 |
;       +-----------+                 |
;             |100us                  |
;       +-----v-----+                 |
;       |Sample 5   |                 |
;       |-----------|                 |
;       |           |                 |
;       |           |                 |
;       +-----+-----+                 |
;             |100us                  |
;       +-----v-----+                 |
;       |Sample 6   |                 |
;       |-----------|                 |
;       |           |                 |
;       |           |                 |
;       +-----+-----+                 |
;             |100us                  |
;       +-----v-----+                 +
;       |Sample 7   |                 |
;       |-----------|                 |
;       |           |                 |
;       |           |                 |
;       +-----+-----+                 |
;             |100us                  |
;       +-----v-----+                 |
;       |Sample 8   |                 |
;       |-----------|                 |
;       |           |Edge             |
;       |           +---->StoreBit+-->|
;       +-----+-----+                 |
;             |100us                  |
;       +-----v-----+                 |
;       |Sample 9   |                 |
;       |-----------|                 |
;       |           |Edge             |
;       |           +---->StoreBit+-->|
;       +-----+-----+                 |
;             |                       |
;       +-----v-----+                 |
;       |Sample 10  |                 |
;       |-----------|                 |
;       |           |Edge             |
;       |           +---->Storebit+-->|
;       +-----+-----+                 |
;             |100us                  |
;       +-----v-----+                 |
;       |Sample 11  |                 |
;       |-----------|                 |
;       |           |Edge             |
;       |           +---->Storebit+-->|
;       +-----+-----+                 |
;             |100us                  |
;       +-----v-----+                 |
;       |Sample 12  |                 |
;       |-----------|                 |
;       |           |Edge             |
;       |           +---->StoreBit+-->+
;       +-----+-----+
;             |No Edge
;       +-----v-----+
;       |Error      |
;       |State=idle |
;       +-----------+

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
        rlncf   WREG
        addwf   TOSL
        movlw   0
        addwfc  TOSH
        addwfc  TOSU            ; for >64K bytes memory
        return 

;jumptable StateTx1
int100usec:
	bcf	PIR1,TMR2IF
	;sample input, edge detect
	;create #ifdef construction for unused ports
#define DEBUG_LOOPBACK_TEST
#ifdef DEBUG_LOOPBACK_TEST
	movff	LATB,myPortaNow
#else
	movff	PORTA,myPortaNow
#endif
	movf	myPortaNow,w,ACCESS
	xorwf	myPortaPrevious,w,ACCESS
	movwf	myPortaEdge
	movff	myPortaNow,myPortaPrevious

	movff	PORTB,myPortbNow
	movf	myPortbNow,w,ACCESS
	xorwf	myPortbPrevious,w,ACCESS
	movwf	myPortbEdge
	movff	myPortbNow,myPortbPrevious

	movff	PORTC,myPortcNow
	movf	myPortcNow,w,ACCESS
	xorwf	myPortcPrevious,w,ACCESS
	movwf	myPortcEdge
	movff	myPortcNow,myPortcPrevious

	movff	PORTD,myPortdNow
	movf	myPortdNow,w,ACCESS
	xorwf	myPortdPrevious,w,ACCESS
	movwf	myPortdEdge
	movff	myPortdNow,myPortdPrevious

	movff	PORTE,myPorteNow
	movf	myPorteNow,w,ACCESS
	xorwf	myPortePrevious,w,ACCESS
	movwf	myPorteEdge
	movff	myPorteNow,myPortePrevious

	;debug verify interrupt timing
	btfsc	bitTx0Busy
	bcf		bitTx0Start

	movlw	PRESCALE_1MS
	decfsz	myPrescale1msec,f,ACCESS
	bra		timerDone	
	movwf	myPrescale1msec
	;here 1msec has passed
	movlw	PRESCALE_125MS
	decfsz	myPrescale125msec,f,ACCESS
	bra		timerDone
	movwf	myPrescale125msec		
	;here 125msec has passed
	movlw	PRESCALE_500MS
	decfsz	myPrescale500msec,f,ACCESS
	bra		timerDone
	movwf	myPrescale500msec		
	;here 500msec has passed
	bsf		bitTx0Start
	bsf		bitRx0Start
timerDone:
	;end debug

tx1Jump:
	decfsz	myPrescale500usec,f,ACCESS
	bra		tx0Done
	movlw	PRESCALE_500US
	movwf	myPrescale500usec

#define tx0StateIdle		tx0State0
#define TX_STATE_IDLE		d'0'
#define tx0StateSendStartA	tx0State1
#define tx0StateSendStartB	tx0State2
#define tx0StateSendBitA	tx0State3
#define TX_STATE_SENDBIT_A	d'3'
#define tx0StateSendBitB	tx0State4
#define tx0StateSendStopA	tx0State5
#define tx0StateSendStopB	tx0State6
#define tx0StateUnused7		tx0State7		
	
	movf	myTx0State,w,ACCESS
	call	Switchcase8
	bra		tx0State0
	bra		tx0State1
	bra		tx0State2
	bra		tx0State3
	bra		tx0State4
	bra		tx0State5
	bra		tx0State6
	bra		tx0State7

tx0StateIdle:
	bcf		bitTx0Busy
	;debug
	movlw	h'EF'
	movwf	myTx0Buffer+0
	movlw	h'BE'
	movwf	myTx0Buffer+1
	movlw	h'AD'
	movwf	myTx0Buffer+2
	movlw	h'DE'
	movwf	myTx0Buffer+3
	;end debug

	btfss	bitTx0Start
	bra		tx0Done
	bra		tx0NextState

tx0StateSendStartA:
#ifdef OUTPUT_ACTIVE_HIGH
	bcf		pinOutTx0
#else
	bsf		pinOutTx0
#endif
	movlw	NUM_TX_BITS
	movwf	myTx0Counter,ACCESS
	bsf		bitTx0Busy
	bra		tx0NextState

tx0StateSendStartB:
#ifdef OUTPUT_ACTIVE_HIGH
	bsf		pinOutTx0	
#else
	bcf		pinOutTx0
#endif
	bra		tx0NextState

tx0StateSendBitA
#ifdef OUTPUT_ACTIVE_HIGH
	btfss	myTx0Buffer+3,7
	bsf		pinOutTx0	;invert highest bit to output pin
	btfsc	myTx0Buffer+0,0
	bcf		pinOutTx0
#else
	btfss	myTx0Buffer+3,7
	bcf		pinOutTx0
	btfsc	myTx0Buffer+3,7
	bsf		pinOutTx0
#endif
	bra		tx0NextState

tx0StateSendBitB
#ifdef OUTPUT_ACTIVE_HIGH
	btfss	myTx0Buffer+3,7
	bcf		pinOutTx0	;copy highest bit to output pin
	btfsc	myTx0Buffer+3,7
	bsf		pinOutTx0
#else
	btfss	myTx0Buffer+3,7
	bsf		pinOutTx0
	btfsc	myTx0Buffer+3,7
	bcf		pinOutTx0	
#endif
	dcfsnz	myTx0Counter,f,ACCESS
	bra		tx0NextState

	bcf		Carry
	rlcf	myTx0Buffer+0,f,ACCESS
	rlcf	myTx0Buffer+1,f,ACCESS
	rlcf	myTx0Buffer+2,f,ACCESS
	rlcf	myTx0Buffer+3,f,ACCESS 
	movlw	TX_STATE_SENDBIT_A
	movwf	myTx0State,ACCESS
	bra		tx0Done

tx0StateSendStopA:
#ifdef OUTPUT_ACTIVE_HIGH
	bsf		pinOutTx0
#else
	bcf		pinOutTx0	
#endif
	bra		tx0NextState

tx0StateSendStopB:
#ifdef OUTPUT_ACTIVE_HIGH
	bcf		pinOutTx0
#else
	bsf		pinOutTx0
#endif
	movlw	TX_STATE_IDLE
	movwf	myTx0State,ACCESS
	bra		tx0Done

tx0StateUnused7:
	clrf 	myTx0State,ACCESS ;should never happen, set an error bit here?
	bra tx0Done

tx0NextState:
	incf	myTx0State,f,ACCESS
tx0Done:
	;return



#define rx0StateIdle		rx0State0
#define RX_STATE_IDLE		d'0'
#define rx0StateWaitStart	rx0State1
#define rx0StateSample0		rx0State2
#define rx0StateSample1		rx0State3
#define RX_SAMPLE_1			d'3'
#define rx0StateSample2		rx0State4
#define rx0StateSample3		rx0State5
#define rx0StateSample4		rx0State6
#define rx0StateSample5		rx0State7
#define rx0StateSample6		rx0State8
#define rx0StateSample7		rx0State9
#define rx0StateSample8		rx0State10
#define rx0StateSample9		rx0State11
#define rx0StateSample10	rx0State12
#define rx0StateSample11	rx0State13
#define rx0StateSample12	rx0State14
#define rx0StateUnused15	rx0State15	
	
	movf	myRx0State,w,ACCESS
	call	Switchcase16
	bra		rx0State0
	bra		rx0State1
	bra		rx0State2
	bra		rx0State3
	bra		rx0State4
	bra		rx0State5
	bra		rx0State6
	bra		rx0State7
	bra		rx0State8
	bra		rx0State9
	bra		rx0State10
	bra		rx0State11
	bra		rx0State12
	bra		rx0State13
	bra		rx0State14
	bra		rx0State15

rx0StateIdle:
	bcf		bitRx0Busy
	btfss	bitRx0Start
	bra		rx0Done
	bra		rx0NextState	

rx0StateWaitStart:
	bsf		bitRx0Busy
#ifdef INPUT_ACTIVE_HIGH
	btfss	pinInRx0
#else
	btfsc	pinInRx0	
#endif
	bra		rx0Done
	movlw	NUM_RX_BITS+1 ;first bit is startbit
	movwf	myRx0Counter,ACCESS


	bra		rx0NextState

rx0StateSample0:
rx0StateSample1:
rx0StateSample2:	
rx0StateSample3:
	btfss	edgeRx0
	bra		rx0NextState
	bra		rx0ErrorFastEdge

rx0StateSample4:
rx0StateSample5:
rx0StateSample6:
rx0StateSample7:
	bra		rx0NextState

rx0StateSample8:
rx0StateSample9:
rx0StateSample10:
rx0StateSample11:
	btfss	edgeRx0
	bra		rx0NextState
	bra		rx0StoreBit

rx0StateSample12:
	btfss	edgeRx0
	bra		rx0ErrorLateEdge
	bra		rx0StoreBit

rx0StateUnused15:
	;set error bit here?	
	bra		rx0SetIdleState


rx0StoreBit
	;opentherm sends and receives data MSB first
	;store bit here
	dcfsnz	myRx0Counter,f,ACCESS
	bra		rx0SetIdleState

	bcf		Carry
#ifdef INPUT_ACTIVE_HIGH
	btfsc	pinInRx0
#else
	btfss	pinInRx0
#endif
	bsf		Carry
	rlcf	myRx0Buffer+0,f,ACCESS
	rlcf	myRx0Buffer+1,f,ACCESS
	rlcf	myRx0Buffer+2,f,ACCESS
	rlcf	myRx0Buffer+3,f,ACCESS

	;not all bits received
	movlw	RX_SAMPLE_1
	movwf	myRx0State,ACCESS
	bra		rx0Done


rx0ErrorLateEdge
	;set error bit here
	bra		rx0SetIdleState

rx0ErrorFastEdge:
	;set error bit here
	;bra	rx0SetIdleState
rx0SetIdleState:
	movlw	RX_STATE_IDLE
	movwf	myRx0State,ACCESS
	bra		rx0Done

rx0NextState:
	incf	myRx0State,f,ACCESS
rx0Done:
	return