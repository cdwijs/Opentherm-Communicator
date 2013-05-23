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

#define txStateIdle			txState0
#define TX_STATE_IDLE		d'0'
#define txStateSendStartA	txState1
#define txStateSendStartB	txState2
#define txStateSendBitA		txState3
#define TX_STATE_SENDBIT_A	d'3'
#define txStateSendBitB		txState4
#define txStateSendStopA	txState5
#define txStateSendStopB	txState6
#define txStateUnused7		txState7		
	movf	myTxState,w,BANKED
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
	bcf		bitTxBusy,BANKED
	btfss	bitTxStart,BANKED
	bra		txDone
	bra		txNextState

txStateSendStartA:
#ifdef OUTPUT_INVERTED
	bcf		pinOutTx,ACCESS
#else
	bsf		pinOutTx,ACCESS
#endif
	movlw	NUM_TX_BITS
	movwf	myTxCounter,BANKED
	bsf		bitTxBusy,BANKED
	bra		txNextState

txStateSendStartB:
#ifdef OUTPUT_INVERTED
	bsf		pinOutTx,ACCESS	
#else
	bcf		pinOutTx,ACCESS
#endif
	bra		txNextState

txStateSendBitA
#ifdef OUTPUT_INVERTED
	btfss	myTxBuffer+3,7,BANKED
	bsf		pinOutTx,ACCESS	;invert highest bit to output pin
	btfsc	myTxBuffer+3,7,BANKED
	bcf		pinOutTx,ACCESS
#else
	btfss	myTxBuffer+3,7,BANKED
	bcf		pinOutTx,ACCESS
	btfsc	myTxBuffer+3,7,BANKED
	bsf		pinOutTx,ACCESS
#endif
	bra		txNextState

txStateSendBitB
#ifdef OUTPUT_INVERTED
	btfss	myTxBuffer+3,7,BANKED
	bcf		pinOutTx,ACCESS	;copy highest bit to output pin
	btfsc	myTxBuffer+3,7,BANKED
	bsf		pinOutTx,ACCESS
#else
	btfss	myTxBuffer+3,7,BANKED
	bsf		pinOutTx,ACCESS
	btfsc	myTxBuffer+3,7,BANKED
	bcf		pinOutTx,ACCESS	
#endif
	dcfsnz	myTxCounter,f,BANKED
	bra		txNextState

	bcf		Carry
	rlcf	myTxBuffer+0,f,BANKED
	rlcf	myTxBuffer+1,f,BANKED
	rlcf	myTxBuffer+2,f,BANKED
	rlcf	myTxBuffer+3,f,BANKED
	movlw	TX_STATE_SENDBIT_A
	movwf	myTxState,BANKED
	bra		txDone

txStateSendStopA:
#ifdef OUTPUT_INVERTED
	bcf		pinOutTx,ACCESS
#else
	bsf		pinOutTx,ACCESS
#endif
	bra		txNextState

txStateSendStopB:
#ifdef OUTPUT_INVERTED
	bsf		pinOutTx,ACCESS
#else
	bcf		pinOutTx,ACCESS
#endif
	movlw	TX_STATE_IDLE
	movwf	myTxState,BANKED
	bra		txDone

txStateUnused7:
	clrf 	myTxState,BANKED ;should never happen, set an error bit here?
	bra txDone

txNextState:
	incf	myTxState,f,BANKED
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
