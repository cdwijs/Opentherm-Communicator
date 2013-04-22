;*******************************************************************************
;    io.inc                                                                    *
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
;PORTA	Tris	Port
;7	Rx7	1 (in)	0
;6	Rx6	1 (in)	0
;5	Rx5	1 (in)	0
;4	Rx4	1 (in)	0
;3	Rx3	1 (in)	0
;2	Rx2	1 (in)	0
;1	Rx1	1 (in)	0
;0	Rx0	1 (in)	0
#define INIT_TRISA b'11111111'
#define INIT_PORTA b'00000000'
;PORTB	Tris	Port
;7	Tx7	0 (out)	1 (low-active)
;6	Tx6	0 (out)	1 (low-active)
;5	Tx5	0 (out)	1 (low-active)
;4	Tx4	0 (out)	1 (low-active)
;3	Tx3	0 (out)	1 (low-active)
;2	Tx2	0 (out)	1 (low-active)
;1	Tx1	0 (out)	1 (low-active)
;0	Tx0	0 (out)	1 (low-active)
#define INIT_TRISB	b'00000000'
#define INIT_PORTB	b'11111111'
;PORTC			Tris	Port
;7	RS232-RX	1		0
;6	RS232-TX	1		0
;5	-			1		0
;4	-			1		0
;3	-			1		0
;2	-			1		0
;1	-			1		0
;0	-			1		0
#define INIT_TRISC	b'11111111'
#define INIT_PORTC	b'00000000'
;PORTD			Tris	Port
;7	-			1		0
;6	-			1		0
;5	-			1		0
;4	-			1		0
;3	-			1		0
;2	-			1		0
;1	-			1		0
;0	-			1		0
#define INIT_TRISD	b'11111111'
#define INIT_PORTD	b'00000000'
;PORTE			Tris	Port
;7	-			1		0
;6	-			1		0
;5	-			1		0
;4	-			1		0
;3	-			1		0
;2	-			1		0
;1	-			1		0
;0	-			1		0
#define INIT_TRISE	b'11111111'
#define INIT_PORTE	b'00000000'
#define pinInRx0	LATA.PORTA0
#define pinInRx1	LATA.PORTA1
#define pinInRx2	LATA.PORTA2
#define pinInRx3	LATA.PORTA3
#define pinInRx4	LATA.PORTA4
#define pinInRx5	LATA.PORTA5
#define pinInRx6	LATA.PORTA6
#define pinInRx7	LATA.PORTA7

#define pinOutTx0	LATB.LATB0
#define pinOutTx1	LATB.LATB1
#define pinOutTx2	LATB.LATB2
#define pinOutTx3	LATB.LATB3
#define pinOutTx4	LATB.LATB4
#define pinOutTx5	LATB.LATB5
#define pinOutTx6	LATB.LATB6	;shared with PGC
#define pinOutTx7	LATB.LATB7	;shared with PGD

ioInit:
	movlw	INIT_PORTA
	movwf	PORTA,ACCESS
	movlw	INIT_TRISA
	movwf	TRISA,ACCESS
	movlw	INIT_PORTB
	movwf	PORTB,ACCESS
	movlw	INIT_TRISB
	movwf	TRISB,ACCESS
	movlw	INIT_PORTC
	movwf	PORTC,ACCESS
	movlw	INIT_TRISC
	movwf	TRISC,ACCESS
	movlw	INIT_PORTD
	movwf	PORTD,ACCESS
	movlw	INIT_TRISD
	movwf	TRISD,ACCESS
	movlw	INIT_PORTE
	movwf	PORTE,ACCESS
	movlw	INIT_TRISE
	movwf	TRISE,ACCESS
	return