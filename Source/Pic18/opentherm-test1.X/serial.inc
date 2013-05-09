;*******************************************************************************
;    serial.inc                                                                *
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
;Baudrate 19K2
;BRGH=1, BRG16=1
;              16MHz/4
;(SPBRG) = ---------= 208.33
;              19200
#define INIT_SPBRG	d'416'	;9600 = 416  
;define INIT_SPBRG	d'207'	;19K2 = 207 ;reports only 2 chars on 19K2 or 9600 baud?
;define INIT_SPBRG	d'34'	;115k2 = 34

serialInit:
	movlb	BANK_RAM_0

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
	movlw	'H'
	call	serialPutByte
	movlw	'o'
	call	serialPutByte
	movlw	'i'
	call	serialPutByte
	movlw	'!'
	call	serialPutByte
	return

#define regserialbyte	reg1
serialPutByteHex
	movwf	regserialbyte,ACCESS
	swapf	regserialbyte,w,ACCESS	;get highnibble
	andlw	h'0f'
	addlw	-d'10'
	btfsc	Carry
	addlw	'A' - '9'  -1
	addlw	'0' + d'10'
	call	serialPutByte
	movf	regserialbyte,w,ACCESS	;get lownibble
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
	movwf	regserialbyte,ACCESS
	lfsr	0,myRs232TxBuf
	incf	myRs232TxProducer,w,ACCESS
	andlw	SERIAL_BUF_MASK
	movwf	myRs232TxProducer,ACCESS	
	movff	regserialbyte,PLUSW0
	bsf		PIE1,TX1IE ;enable TX interrupt here
	return
#undefine regserialbyte

serialTxInt:
;need to save / restore FSR0 here, or only use fsr0 in interrupt
	;if producer != consumer, there's data to be send
	lfsr	0,myRs232TxBuf
	movf	myRs232TxConsumer,w,ACCESS
	cpfseq	myRs232TxProducer,ACCESS
	bra		serialTxIntBusy
	bcf		PIE1,TX1IE ;disable interrupt
	bra		serialTxIntDone						
serialTxIntBusy:
	incf	myRs232TxConsumer,w,ACCESS
	andlw	SERIAL_BUF_MASK
	movwf	myRs232TxConsumer,ACCESS
	movff	PLUSW0,TXREG1
	
	
serialTxIntDone:
	return
