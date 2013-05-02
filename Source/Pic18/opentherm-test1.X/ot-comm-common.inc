;*******************************************************************************
;    ot-comm-re-use.inc                                                               *
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
#define commStateIdle			commState0
#define COMM_STATE_IDLE			d'0'
#define commStateStartA			commState1
#define commStateStartB			commState2
#define commStateWaitAnswer		commState3
#define commStateTimeout		commState4
#define commStateAborted		commState5
#define commStateReportMsg		commState6
#define commStateUnused7		commState7
	movf	myCommState,w,ACCESS
	call	Switchcase8
	bra		commState0
	bra		commState1
	bra		commState2
	bra		commState3
	bra		commState4
	bra		commState5
	bra		commState6
	bra		commState7

commStateIdle:
	bcf		bitCommBusy
	btfss	bitCommStartMsg
	bra		commDone
	bra		commNextState


commStateStartA:
	bsf		bitCommBusy
	bsf		bitRxStart
	bsf		bitTxStart
	btfss	bitRxBusy
	bra		commDone
	bra		commNextState
		
commStateStartB:
	bcf		bitRxStart
	btfsc	bitTxBusy
	bra		commDone
	bra		commNextState
		
commStateWaitAnswer:
	btfss	bitRxBusy
	bra		commNextState

	movf	myCommTimer,f,ACCESS	
	bnz		commDone
;	movlw	
	
commStateTimeout:		
commStateAborted:		
commStateReportMsg:		
commStateUnused7:
	bra		commDone

commNextState:
	incf	myCommState,f,ACCESS
commDone:

;labels
#undefine commStateIdle
#undefine COMM_STATE_IDLE
#undefine commStateStartA
#undefine commStateStartB	
#undefine commStateWaitAnswer
#undefine commStateTimeout
#undefine commStateAborted
#undefine commStateReportMsg
#undefine commStateUnused7
#undefine commState0			
#undefine commState1			
#undefine commState2			
#undefine commState3			
#undefine commState4			
#undefine commState5			
#undefine commState6			
#undefine commState7
#undefine commNextState		
#undefine commDone			
;data
#undefine myCommState			
#undefine myCommTimer			
;bits
#undefine bitCommBusy			
#undefine bitCommStartMsg		
#undefine bitRxStart			
#undefine bitRxBusy			
#undefine bitTxStart			
#undefine bitTxBusy			