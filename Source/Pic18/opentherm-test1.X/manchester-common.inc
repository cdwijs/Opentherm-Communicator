;*******************************************************************************
;    manchester-common.inc                                                     *
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

#define txStateIdle		txState0
#define TX_STATE_IDLE		d'0'
#define txStateSendStartA	txState1
#define txStateSendStartB	txState2
#define txStateSendBitA	txState3
#define TX_STATE_SENDBIT_A	d'3'
#define txStateSendBitB	txState4
#define txStateSendStopA	txState5
#define txStateSendStopB	txState6
#define txStateUnused7		txState7		
	
	movf	myTxState,w,ACCESS
	call	Switchcase8
	bra		txState0
	bra		txState1
	bra		txState2
	bra		txState3
	bra		txState4
	bra		txState5
	bra		txState6
	bra		txState7

txStateIdle:
	bcf		bitTxBusy
	;debug
	movlw	h'EF'
	movwf	myTxBuffer+0
	movlw	h'BE'
	movwf	myTxBuffer+1
	movlw	h'AD'
	movwf	myTxBuffer+2
	movlw	h'DE'
	movwf	myTxBuffer+3
	;end debug

	btfss	bitTxStart
	bra		txDone
	bra		txNextState

txStateSendStartA:
#ifdef OUTPUT_ACTIVE_HIGH
	bcf		pinOutTx
#else
	bsf		pinOutTx
#endif
	movlw	NUM_TX_BITS
	movwf	myTxCounter,ACCESS
	bsf		bitTxBusy
	bra		txNextState

txStateSendStartB:
#ifdef OUTPUT_ACTIVE_HIGH
	bsf		pinOutTx	
#else
	bcf		pinOutTx
#endif
	bra		txNextState

txStateSendBitA
#ifdef OUTPUT_ACTIVE_HIGH
	btfss	myTxBuffer+3,7
	bsf		pinOutTx	;invert highest bit to output pin
	btfsc	myTxBuffer+3,7
	bcf		pinOutTx
#else
	btfss	myTxBuffer+3,7
	bcf		pinOutTx
	btfsc	myTxBuffer+3,7
	bsf		pinOutTx
#endif
	bra		txNextState

txStateSendBitB
#ifdef OUTPUT_ACTIVE_HIGH
	btfss	myTxBuffer+3,7
	bcf		pinOutTx	;copy highest bit to output pin
	btfsc	myTxBuffer+3,7
	bsf		pinOutTx
#else
	btfss	myTxBuffer+3,7
	bsf		pinOutTx
	btfsc	myTxBuffer+3,7
	bcf		pinOutTx	
#endif
	dcfsnz	myTxCounter,f,ACCESS
	bra		txNextState

	bcf		Carry
	rlcf	myTxBuffer+0,f,ACCESS
	rlcf	myTxBuffer+1,f,ACCESS
	rlcf	myTxBuffer+2,f,ACCESS
	rlcf	myTxBuffer+3,f,ACCESS 
	movlw	TX_STATE_SENDBIT_A
	movwf	myTxState,ACCESS
	bra		txDone

txStateSendStopA:
#ifdef OUTPUT_ACTIVE_HIGH
	bsf		pinOutTx
#else
	bcf		pinOutTx	
#endif
	bra		txNextState

txStateSendStopB:
#ifdef OUTPUT_ACTIVE_HIGH
	bcf		pinOutTx
#else
	bsf		pinOutTx
#endif
	movlw	TX_STATE_IDLE
	movwf	myTxState,ACCESS
	bra		txDone

txStateUnused7:
	clrf 	myTxState,ACCESS ;should never happen, set an error bit here?
	bra txDone

txNextState:
	incf	myTxState,f,ACCESS
txDone:

;labels
#undefine txStateIdle
#undefine TX_STATE_IDLE
#undefine txStateSendStartA
#undefine txStateSendStartB
#undefine txStateSendBitA
#undefine TX_STATE_SENDBIT_A
#undefine txStateSendBitB
#undefine txStateSendStopA
#undefine txStateSendStopB
#undefine txStateUnused7
#undefine txState0
#undefine txState1
#undefine txState2
#undefine txState3
#undefine txState4
#undefine txState5
#undefine txState6
#undefine txState7
#undefine txNextState
#undefine txDone
;data
#undefine myTxState
#undefine myTxBuffer
#undefine myTxCounter
;bits
#undefine bitTxBusy
#undefine bitTxStart
#undefine pinOutTx


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
#define rxStateSample6		rxState8
#define rxStateSample7		rxState9
#define rxStateSample8		rxState10
#define rxStateSample9		rxState11
#define rxStateSample10		rxState12
#define rxStateSample11		rxState13
#define rxStateSample12		rxState14
#define rxStateUnused15		rxState15	
	
	movf	myRxState,w,ACCESS
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
#ifdef INPUT_ACTIVE_HIGH
	btfss	pinInRx
#else
	btfsc	pinInRx	
#endif
	bra		rxDone
	movlw	NUM_RX_BITS+1 ;first bit is startbit
	movwf	myRxCounter,ACCESS


	bra		rxNextState

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
	dcfsnz	myRxCounter,f,ACCESS
	bra		rxSetIdleState

	bcf		Carry
#ifdef INPUT_ACTIVE_HIGH
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
	;bra	rxSetIdleState
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
