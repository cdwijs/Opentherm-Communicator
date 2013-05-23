;*******************************************************************************
;    ot-comm-states.inc                                                        *
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
;This state machine handles the opentherm conversation.
;It sends the request to the thermostat, handles timeout and parity check

;       +-------------+
;       |Idle         |
;       |-------------|
;       |BitRxStart=1 |
; +---->|bitTxStart=0 |
; |     +------+------+
; |            |bitRxBusy
; |     +------v------+
; |     |WaitAnswer   |
; |     |-------------|
; |     |bitRxStart=0 |
; |     |             |
; |     |             |
; |     +------+------+
; |            |!bitRxBusy
; |     +------v------+
; |     |CheckMessageA|
; |     |-------------|
; |     |Timer = 20 ms|
; |     |             |
; |     +------+------+
; |     Error  |
; |<-----------|
; |            |
; |     +------v------+
; |     |CheckmessageB|
; |     |-------------|
; |     |             |
; |     |             |
; |     +------+------+
; |            |
; |     +------v------+
; |     |SetupAnswer  |
; |     |-------------|
; |     |             |
; |     |             |
; |     +------+------+
; |            |
; |     +------v------+
; |     |WaitSend     |
; |     |-------------|
; |     |             |
; |     |             |
; |     +------+------+
; |            |Timer
; |     +------v------+
; |     |SendAnswer   |
; |     |-------------|
; |     |             |
; |     |             |
; |     +------+------+
; |            |!bitTxbusy
; +<-----------+
#define commStateIdle			commState0
#define COMM_STATE_IDLE			d'0'
#define commStateWaitAnswer		commState1
#define commStateCheckMessageA	commState2
#define commStateCheckMessageID	commState3
#define commStateSetupAnswer	commState4
#define commStateReport			commState5
#define commStateWaitSend		commState6
#define commStateSendAnswer		commState7

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
	bsf		bitRxStart
	btfss	bitRxBusy
	bra		commDone
	bra		commNextState

;cdw, add timeout?
commStateWaitAnswer:
	bcf		bitRxStart
	btfsc	bitRxBusy
	bra		commDone
	bra		commNextState

#define regparity	reg0		
commStateCheckMessageA:
	movlw	TIME_COMM_RESPONSE
	movwf	myCommTimer
	
	;first check Rx errors (late and early edges)
	
	;now check parity
	movf	myRxBuffer+3,w,ACCESS
	xorwf	myRxBuffer+2,w,ACCESS
	xorwf	myRxBuffer+1,w,ACCESS
	xorwf	myRxBuffer+0,w,ACCESS		
	movwf	regparity,ACCESS

	swapf	regparity,w,ACCESS 
    xorwf   regparity,f,ACCESS 
	bcf		Carry
    rrcf	regparity,w,ACCESS
    xorwf   regparity,f,ACCESS
    btfsc   regparity,2 
    incf	regparity,f,ACCESS 
    ;parity is now in lowest bit of reg0
	;0 means even, 1 means error
	btfss	reg0,0
	bra		commNextState

	;set error bit here
	movlw	COMM_STATE_IDLE
	movwf	myCommState,ACCESS
	bra		commDone
#undefine regparity
		
commStateCheckMessageID:
	movf	myRxBuffer+OFFSET_MSG_ID,w
	xorlw	ID_MASTER_SLAVE_FLAGS
	bz		commIdSlaveFlags
	xorlw	ID_CH_WATER_TEMP_SETPOINT^ID_MASTER_SLAVE_FLAGS
	bz		commIdTempSetpoint
	xorlw	ID_REMOTE_OVERWRITE_ROOM_SETP^ID_CH_WATER_TEMP_SETPOINT
	bz		commIdRemoteOverwrite
	xorlw	ID_MAX_REL_MOD_LEVEL^ID_REMOTE_OVERWRITE_ROOM_SETP
	bz		commIdMaxRelMod
	xorlw	ID_MAX_BOILER_CAPACITY^ID_MAX_REL_MOD_LEVEL	
	bz		commIdMaxBoilerCapacity
	xorlw	ID_ROOM_SETPOINT^ID_MAX_BOILER_CAPACITY
	bz		commIdRoomSetpoint
	xorlw	ID_RELATIVE_MODULATION_LEVEL^ID_ROOM_SETPOINT
	bz		commIdRelModLevel
	xorlw	ID_WATER_PRESSURE_IN_CH_CIRCUIT^ID_RELATIVE_MODULATION_LEVEL
	bz		commIdWaterPressInCh
	xorlw	ID_DAY_OF_WEEK_AND_TIME_OF_DAY^ID_WATER_PRESSURE_IN_CH_CIRCUIT
	bz		commIdDayOfWeek
	xorlw	ID_ROOM_TEMPERATURE^ID_DAY_OF_WEEK_AND_TIME_OF_DAY
	bz		commIdRoomTemp
	xorlw	ID_BOILER_FLOW_WATER_TEMPERATURE^ID_ROOM_TEMPERATURE
	bz		commIdFlowWaterTemp
	xorlw	ID_DHW_TEMPERATURE^ID_BOILER_FLOW_WATER_TEMPERATURE
	bz		commIdDwhTemp
	xorlw	ID_OUTSIDE_TEMPERATURE^ID_DHW_TEMPERATURE
	bz		commIdOutsideTemp
	xorlw	ID_RETURN_WATER_TEMPERATURE^ID_OUTSIDE_TEMPERATURE
	bz		commIdReturnWaterTemp
	xorlw	ID_DHW_SETPOINT^ID_RETURN_WATER_TEMPERATURE
	bz		commIdDhwSetpoint
	xorlw	ID_MAX_CH_WATER_SETPOINT^ID_DHW_SETPOINT
	bz		commIdMaxChWaterSetpoint
	xorlw	ID_REMOTE_OVERRIDE_FUNCTION^ID_MAX_CH_WATER_SETPOINT
	bz		commIdRemoteOverRide

	;here the ID is not known
	;prepare answer: "ID not known"
	movlw	(MSG_TYPE_UNKNOWN_DATA_ID << 4)
	movwf	myTxBuffer+OFFSET_MSG_TYPE,ACCESS
	movff	myRxBuffer+OFFSET_MSG_ID,myTxBuffer+OFFSET_MSG_ID
	clrf	myTxBuffer+OFFSET_DATA_HIGH,ACCESS
	clrf	myTxBuffer+OFFSET_DATA_LOW,ACCESS
	bra		commNextState
	
commIdSlaveFlags:
	clrf	myTxBuffer+OFFSET_DATA_LOW,ACCESS
	clrf	myTxBuffer+OFFSET_DATA_HIGH,ACCESS
	bsf		myTxBuffer+OFFSET_DATA_LOW,1 ;enable CH mode
	bsf		myTxBuffer+OFFSET_DATA_LOW,3 ;enable flame (seems to only work in combination with bit1)
;	bsf		myTxBuffer+OFFSET_DATA_LOW,5 ;enable CH2 mode
	bra		commStateCheckKnown
commIdTempSetpoint:
	bra		commStateCheckKnown
commIdRemoteOverwrite:
	bra		commStateCheckKnown
commIdMaxRelMod:
	bra		commStateCheckKnown
commIdMaxBoilerCapacity:
	bra		commStateCheckKnown
commIdRoomSetpoint:
	bra		commStateCheckKnown
commIdRelModLevel:
	bra		commStateCheckKnown
commIdWaterPressInCh:
	bra		commStateCheckKnown
commIdDayOfWeek:
	bra		commStateCheckKnown
commIdRoomTemp:
	bra		commStateCheckKnown
commIdFlowWaterTemp:
	bra		commStateCheckKnown
commIdDwhTemp:
	bra		commStateCheckKnown
commIdOutsideTemp:
	bra		commStateCheckKnown
commIdReturnWaterTemp:
	bra		commStateCheckKnown
commIdDhwSetpoint:
	bra		commStateCheckKnown
commIdMaxChWaterSetpoint:
	bra		commStateCheckKnown
commIdRemoteOverRide:
	bra		commStateCheckKnown






commStateCheckKnown:
#define messagetype reg0
	;figure out message type (bit6,5,4 of MSB)
	swapf	myRxBuffer+3,w,ACCESS
	andlw	h'07' ;w now holds message type, 0-7
	btfsc	WREG,2
	bra		commStateCheckWrongMsgtype
	
	bz		commReadData  ;reg=00
	decf	WREG,f,ACCESS
	bz		commWriteData ;reg=01
	decf	WREG,f,ACCESS
	bz		commWriteInvalid ;reg=10
	;reg=11 is reserved, drop packet
	bra		commStateCheckWrongMsgtype
#undefine messagetype
	
commReadData: ;thermostat is reading from us
	;for now send always 0, wich is invalid :-)
	movlw	(MSG_TYPE_READ_ACK << 4)
;	movlw	(MSG_TYPE_DATA_INVALID << 4)
	movwf	myTxBuffer+OFFSET_MSG_TYPE,ACCESS
	movff	myRxBuffer+OFFSET_MSG_ID,myTxBuffer+OFFSET_MSG_ID
;	clrf	myTxBuffer+OFFSET_DATA_HIGH,ACCESS ;remove this, and fill in the data above
;	clrf	myTxBuffer+OFFSET_DATA_LOW,ACCESS

	;debug, replay 0x40192F80 to question 0x80190000
;	movlw	h'2F'
;	movwf	myTxBuffer+OFFSET_DATA_HIGH,ACCESS
;	movlw	h'80'
;	movwf	myTxBuffer+OFFSET_DATA_LOW,ACCESS
	;end debug


	bra		commNextState

commWriteData: 
	;for now send always 0
	movlw	(MSG_TYPE_WRITE_ACK << 4)
	movwf	myTxBuffer+OFFSET_MSG_TYPE,ACCESS
	movff	myRxBuffer+OFFSET_MSG_ID,myTxBuffer+OFFSET_MSG_ID
	clrf	myTxBuffer+OFFSET_DATA_HIGH,ACCESS
	clrf	myTxBuffer+OFFSET_DATA_LOW,ACCESS
	bra		commNextState

commWriteInvalid:
	movlw	(MSG_TYPE_DATA_INVALID << 4)
	movwf	myTxBuffer+OFFSET_MSG_TYPE,ACCESS
	movff	myRxBuffer+OFFSET_MSG_ID,myTxBuffer+OFFSET_MSG_ID
	clrf	myTxBuffer+OFFSET_DATA_HIGH,ACCESS
	clrf	myTxBuffer+OFFSET_DATA_LOW,ACCESS
	bra		commNextState

commStateCheckWrongMsgtype:
	movlw	COMM_STATE_IDLE
	movwf	myCommState,ACCESS
	bra		commDone	

#define regparity	reg0
commStateSetupAnswer:
	;Fill in the parity bit, next state sends it.
	movf	myTxBuffer+3,w,ACCESS
	xorwf	myTxBuffer+2,w,ACCESS
	xorwf	myTxBuffer+1,w,ACCESS
	xorwf	myTxBuffer+0,w,ACCESS		
	movwf	regparity,ACCESS

	swapf	regparity,w,ACCESS 
    xorwf   regparity,f,ACCESS 
	bcf		Carry
    rrcf	regparity,w,ACCESS
    xorwf   regparity,f,ACCESS
    btfsc   regparity,2 
    incf	regparity,f,ACCESS 
    ;parity is now in lowest bit of reg0
	movlw	h'80'		
	btfsc	regparity,0 	;0 means even, 1 means odd
	xorwf	myTxBuffer+3	;invert parity bit if odd to make even
	bra		commNextState

#undefine regparity

commStateReport:
	movf	myRs232TxConsumer,w,ACCESS ;wait for serial buffer to be empty
	cpfseq	myRs232TxProducer,ACCESS
	bra		commDone
#ifdef DEBUG_RS232_SPACES
	movlw	' '
	call	serialPutByte
#endif
	movlw	'T'
	call	serialPutByte
	movf	myRxBuffer+3,w,ACCESS
	call	serialPutByteHex
	movf	myRxBuffer+2,w,ACCESS
	call	serialPutByteHex
	movf	myRxBuffer+1,w,ACCESS
	call	serialPutByteHex
	movf	myRxBuffer+0,w,ACCESS
	call	serialPutByteHex
	movlw	h'0d' ;cr + lf
	call	serialPutByte
	movlw	h'0a'
	call	serialPutByte
#ifdef DEBUG_RS232_SPACES
	movlw	' '
	call	serialPutByte
#endif
	movlw	'B'
	call	serialPutByte
	movf	myTxBuffer+3,w,ACCESS
	call	serialPutByteHex
	movf	myTxBuffer+2,w,ACCESS
	call	serialPutByteHex
	movf	myTxBuffer+1,w,ACCESS
	call	serialPutByteHex
	movf	myTxBuffer+0,w,ACCESS
	call	serialPutByteHex
	movlw	h'0d' ;cr + lf
	call	serialPutByte
	movlw	h'0a'
	call	serialPutByte
;end report code
	bra		commNextState

commStateWaitSend:
	movf	myCommTimer,f,ACCESS
	bnz		commDone
	bsf		bitTxStart
	bra		commNextState
 
commStateSendAnswer:		
	btfss	bitTxBusy
	bra		commDone	
	bcf		bitTxStart
	movlw	COMM_STATE_IDLE
	movwf	myCommState,ACCESS
	bra		commDone

commNextState:
	incf	myCommState,f,ACCESS
commDone:

;labels
#undefine commStateIdle
#undefine COMM_STATE_IDLE
#undefine commStateWaitAnswer
#undefine commStateCheckMessageA
#undefine commStateCheckMessageID
#undefine commStateSetupAnswer
#undefine commStateReport
#undefine commStateWaitSend
#undefine commStateSendAnswer
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

#undefine commIdSlaveFlags
#undefine commIdTempSetpoint			
#undefine commIdRemoteOverwrite		
#undefine commIdMaxRelMod				
#undefine commIdMaxBoilerCapacity		
#undefine commIdRoomSetpoint			
#undefine commIdRelModLevel			
#undefine commIdWaterPressInCh		
#undefine commIdDayOfWeek				
#undefine commIdRoomTemp				
#undefine commIdFlowWaterTemp			
#undefine commIdDwhTemp				
#undefine commIdOutsideTemp			
#undefine commIdReturnWaterTemp		
#undefine commIdDhwSetpoint			
#undefine commIdMaxChWaterSetpoint	
#undefine commIdRemoteOverRide		
#undefine commStateCheckKnown			
#undefine commReadData				
#undefine commWriteData				
#undefine commWriteInvalid			
#undefine commStateCheckWrongMsgtype	
			
;data
#undefine myCommState			
#undefine myCommTimer
#undefine myRxBuffer
#undefine myTxBuffer			
;bits
#undefine bitCommBusy			
#undefine bitCommStartMsg		
#undefine bitRxStart			
#undefine bitRxBusy			
#undefine bitTxStart			
#undefine bitTxBusy			