;*******************************************************************************
;    interrupt.inc                                                             *
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
interruptInit:
	bsf	RCON,IPEN	;1 = Enable priority levels on interrupts
	bsf	IPR1,TMR2IP	;1 = High priority for TMR2
	bsf	IPR1,TX1IP	;1 = High priority for TX1
	bsf	PIE1,TMR2IE	;1 = Enables the TMR2 to PR2 match interrupt 
	bsf	INTCON,GIEH	;1 = Enables all high priority interrupts
	return

interruptHigh:
	btfsc	PIR1,TMR2IF	;1 = TMR2 to PR2 match occurred (must be cleared by software) 
	call	int100usec		;replace by goto construction, to free up 1 stacklevel
	btfsc	PIR1,TX1IF	;1 = The EUSART1 transmit buffer, TXREG1, is empty
	call	serialTxInt
	retfie	FAST		;auto-restore Wreg, Status and BSR registers
	
interruptLow:
	retfie			;no autorestore