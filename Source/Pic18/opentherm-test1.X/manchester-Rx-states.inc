;*******************************************************************************
;    manchester-rx-states.inc                                                  *
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
; |           |                |
; |<----------+                |
; |           |Edge            | ;cdw, on edge code goes to Sample5
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

#define rxStateIdle			rxState0
#define RX_STATE_IDLE		d'0'
#define rxStateWaitStart	rxState1
#define rxStateSample0		rxState2
#define rxStateSample1		rxState3
#define RX_SAMPLE_1			d'3'
#define rxStateSample2		rxState4
#define rxStateSample3		rxState5
#define rxStateSample4		rxState6
#define rxStateSample5		rxState7
#define RX_SAMPLE_5			d'7'
#define rxStateSample6		rxState8
#define rxStateSample7		rxState9
#define rxStateSample8		rxState10
#define rxStateSample9		rxState11
#define rxStateSample10		rxState12
#define rxStateSample11		rxState13
#define rxStateSample12		rxState14
#define rxStateUnused15		rxState15	
	
	movf	myRxState,w,BANKED
	call	Switchcase16
	bra		rxState0
	bra		rxState1
	bra		rxState2
	bra		rxState3
	bra		rxState4
	bra		rxState5
	bra		rxState6
	bra		rxState7
	bra		rxState8
	bra		rxState9
	bra		rxState10
	bra		rxState11
	bra		rxState12
	bra		rxState13
	bra		rxState14
	bra		rxState15

rxStateIdle:
	bcf		bitRxBusy
	btfss	bitRxStart
	bra		rxDone
	bra		rxNextState	

rxStateWaitStart:
	bsf		bitRxBusy
	btfss	edgeRx		;only react to an edge, not a stuck-at-active signal...
	bra		rxDone
#ifdef INPUT_INVERTED
	btfsc	pinInRx
#else
	btfss	pinInRx
#endif
	bra		rxDone
	movlw	NUM_RX_BITS+1+1 ;first bit is startbit, and first decrement, and then check
	movwf	myRxCounter,BANKED
	;first edge is at start of bit, so skip half a bittime
	movlw	RX_SAMPLE_5
	movwf	myRxState,BANKED
	bra		rxDone

rxStateSample0:
rxStateSample1:
rxStateSample2:	
rxStateSample3:
	btfss	edgeRx
	bra		rxNextState
	bra		rxErrorFastEdge

rxStateSample4:
rxStateSample5:
rxStateSample6:
rxStateSample7:
	bra		rxNextState

rxStateSample8:
rxStateSample9:
rxStateSample10:
rxStateSample11:
	btfss	edgeRx
	bra		rxNextState
	bra		rxStoreBit

rxStateSample12:
	btfss	edgeRx
	bra		rxErrorLateEdge
	bra		rxStoreBit

rxStateUnused15:
	;set error bit here?	
	bra		rxSetIdleState


rxStoreBit:
	;opentherm sends and receives data MSB first
	;store bit here
	dcfsnz	myRxCounter,f,BANKED
	bra		rxSetIdleState	;cdw, this code does not check for stopbit
	
	bcf		Carry
#ifdef INPUT_INVERTED
	btfsc	pinInRx
#else
	btfss	pinInRx
#endif
	bsf		Carry
	rlcf	myRxBuffer+0,f,ACCESS
	rlcf	myRxBuffer+1,f,ACCESS
	rlcf	myRxBuffer+2,f,ACCESS
	rlcf	myRxBuffer+3,f,ACCESS

	;not all bits received
	movlw	RX_SAMPLE_1
	movwf	myRxState,ACCESS
	bra		rxDone

rxErrorLateEdge:
	;set error bit here
	bra		rxSetIdleState

rxErrorFastEdge:
	;set error bit here
	bra	rxSetIdleState
rxSetIdleState:
	movlw	RX_STATE_IDLE
	movwf	myRxState,ACCESS
	bra		rxDone

rxNextState:
	incf	myRxState,f,ACCESS
rxDone:

;labels
#undefine rxStateIdle
#undefine RX_STATE_IDLE
#undefine rxStateWaitStart
#undefine rxStateSample0
#undefine rxStateSample1
#undefine RX_SAMPLE_1
#undefine rxStateSample2
#undefine rxStateSample3
#undefine rxStateSample4
#undefine rxStateSample5
#undefine RX_SAMPLE_5
#undefine rxStateSample6
#undefine rxStateSample7
#undefine rxStateSample8
#undefine rxStateSample9
#undefine rxStateSample10
#undefine rxStateSample11
#undefine rxStateSample12
#undefine rxStateUnused15
#undefine rxState0
#undefine rxState1
#undefine rxState2
#undefine rxState3
#undefine rxState4
#undefine rxState5
#undefine rxState6
#undefine rxState7
#undefine rxState8
#undefine rxState9
#undefine rxState10
#undefine rxState11
#undefine rxState12
#undefine rxState13
#undefine rxState14
#undefine rxState15
#undefine rxStoreBit
#undefine rxErrorLateEdge
#undefine rxErrorFastEdge
#undefine rxSetIdleState
#undefine rxNextState
#undefine rxDone
;data
#undefine myRxState
#undefine myRxCounter
#undefine myRxBuffer
;bits
#undefine bitRxBusy
#undefine bitRxStart
#undefine pinInRx
#undefine edgeRx
