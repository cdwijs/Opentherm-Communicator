;*******************************************************************************
;    serial.inc                                                                *
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



; Serial command format:
; "SG1400"+cr+lf
; 
;
;
;




#ifdef CPU_16MHZ
	;Baudrate 19K2
	;BRGH=1, BRG16=1
	;              16MHz/4
	;(SPBRG) = ---------= 208.33
	;              19200
	#define INIT_SPBRG	d'416'	;9600 = 416  
	;define INIT_SPBRG	d'207'	;19K2 = 207
	;define INIT_SPBRG	d'34'	;115k2 = 34
#endif

#ifdef CPU_8MHZ
	;Baudrate 19K2
	;BRGH=1, BRG16=1
	;              16MHz/4
	;(SPBRG) = ---------= 208.33
	;              19200
	#define INIT_SPBRG	d'207'	;9600 = 207
	;define INIT_SPBRG	d'103'	;19K2 = 103
	;define INIT_SPBRG	d'16'	;115k2 = 16
#endif

serialInit:
	movlb	BANK_RAM_0
#ifdef P18F2x4xK22
	movlw	high(INIT_SPBRG)
	movwf	SPBRGH1,ACCESS
	movlw	low(INIT_SPBRG)
	movwf	SPBRG1,ACCESS

	bsf		TXSTA1,BRGH,ACCESS
	bsf		BAUDCON1,BRG16
	bcf		TXSTA1,SYNC,ACCESS
	bsf		TRISC,6,ACCESS
	bsf		RCSTA1,SPEN,ACCESS
	bsf		TXSTA1,TXEN,ACCESS
	bsf		RCSTA1,CREN,ACCESS
#endif
#ifdef P18F2455_2550_4455_4550
	movlw	high(INIT_SPBRG)
	movwf	SPBRGH,ACCESS
	movlw	low(INIT_SPBRG)
	movwf	SPBRG,ACCESS

	bsf		TXSTA,BRGH,ACCESS
	bsf		BAUDCON,BRG16
	bcf		TXSTA,SYNC,ACCESS
	bsf		TRISC,6,ACCESS
	bsf		RCSTA,SPEN,ACCESS
	bsf		RCSTA,RCEN,ACCESS ;*untested*
	bsf		TXSTA,TXEN,ACCESS
#endif
#ifdef RS232_GREETING
	movlw	'H'
	call	serialPutByte
	movlw	'o'
	call	serialPutByte
	movlw	'i'
	call	serialPutByte
	movlw	'!'
	call	serialPutByte
#endif
	return

#define regserialbyte	reg1
serialPutByteHex
	movwf	regserialbyte,BANKED
	swapf	regserialbyte,w,BANKED	;get highnibble
	andlw	h'0f'
	addlw	-d'10'
	btfsc	Carry
	addlw	'A' - '9'  -1
	addlw	'0' + d'10'
	call	serialPutByte
	movf	regserialbyte,w,BANKED	;get lownibble
	andlw	h'0f'
	addlw	-d'10'
	btfsc	Carry
	addlw	'A' - '9'  -1
	addlw	'0' + d'10'
	call	serialPutByte
	return
#undefine regserialbyte
#define regserialbyte	reg0
serialPutByte
	;place byte in wreg into buffer, it does not check for space in the buffer
	movwf	regserialbyte,BANKED
	lfsr	0,myRs232TxBuf
	incf	myRs232TxProducer,w,BANKED
	andlw	SERIAL_BUF_MASK
	movwf	myRs232TxProducer,BANKED
	movff	regserialbyte,PLUSW0
	#ifdef P18F2455_2550_4455_4550
		bsf		PIE1,TXIE ;enable TX interrupt here
	#endif
	#ifdef P18F2x4xK22
		bsf		PIE1,TX1IE ;enable TX interrupt here
	#endif
	return
#undefine regserialbyte

serialRxInt:
#ifdef P18F2455_2550_4455_4550 ;*untested*
#endif
#ifdef P18F2x4xK22
	btfsc	RCSTA1,OERR,ACCESS
	bra		serialRxIntOERR
	btfsc	RCSTA1,FERR,ACCESS
	bra		serialRxIntFERR
	;on no error, store the received byte in ringbuffer.
	;fsr0 is reserved for interrupt
	lfsr	0,myRs232RxBuf
	movf	myRs232RxProducer,w,BANKED
	andlw	SERIAL_BUF_MASK
	movff	RCREG1,PLUSW0
	incf	myRs232RxProducer,w,BANKED
	andlw	SERIAL_BUF_MASK
	movwf	myRs232RxProducer,BANKED
	bra		serialRxIntDone
#endif
serialRxIntOERR: ;on overrun error, clear and set CREN bit, also read and ignore RCREG
	bcf		RCSTA1,CREN,ACCESS
	bsf		RCSTA1,CREN,ACCESS
serialRxIntFERR: ;on framing error, read byte from RCREG, but ignore
	movf	RCREG1,w,ACCESS
	bra		serialRxIntDone
serialRxIntDone:
	return

serialTxInt:
	;fsr0 is reserved for interrupt
	;if producer != consumer, there's data to be send
	lfsr	0,myRs232TxBuf
	movf	myRs232TxConsumer,w,BANKED
	cpfseq	myRs232TxProducer,BANKED
	bra		serialTxIntBusy
	#ifdef P18F2455_2550_4455_4550
		bcf		PIE1,TXIE ;disable interrupt
	#endif
	#ifdef P18F2x4xK22
		bcf		PIE1,TX1IE ;disable interrupt
	#endif
	bra		serialTxIntDone						
serialTxIntBusy:
	incf	myRs232TxConsumer,w,BANKED
	andlw	SERIAL_BUF_MASK
	movwf	myRs232TxConsumer,BANKED
	#ifdef P18F2455_2550_4455_4550
		movff	PLUSW0,TXREG
	#endif
	#ifdef P18F2x4xK22
		movff	PLUSW0,TXREG1
	#endif
serialTxIntDone:
	return
