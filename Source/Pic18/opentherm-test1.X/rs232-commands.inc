;*******************************************************************************
;    ser-comm.inc                                                              *
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
;This state machine handles commands from the serial port.
;The following commands are implemented:
;Update room setpoint.
;Set flame symbol
;Set boiler symbol
;Set Home signal
;Heartbeat

#define RS232_CMD_KEEPALIVE			'a'
#define RS232_CMD_SELECT_THERM_0	'S'
#define RS232_CMD_SELECT_THERM_1	'T'
#define RS232_CMD_SELECT_THERM_2	'U'
#define RS232_CMD_SELECT_THERM_3	'V'
#define RS232_CMD_SELECT_THERM_4	'W'
#define RS232_CMD_SELECT_THERM_5	'X'
#define RS232_CMD_SELECT_THERM_6	'Y'
#define RS232_CMD_SELECT_THERM_7	'Z'

#define RS232_SMD_SET_TEMP			'G'
#define RS232_CMD_SET_SYM_FLAME		'F'
#define RS232_CMD_SET_SYM_BOILER	'B'

#define RS232_CR					h'0D'
#define RS232_LF					h'0A'

#define cmdStateIdle				cmdState0
#define CMD_STATE_IDLE				d'0'
#define cmdStateThermostat			cmdState1
#define cmdStateCommand				cmdState2
#define CMD_STATE_COMMAND			d'2'
#define cmdSetpoint					cmdState3
#define cmdSetpointD3				cmdState4
#define CMD_STATE_SETPOINT			d'4'
#define cmdSetpointD2				cmdState5
#define cmdSetpointD1				cmdState6
#define cmdSetpointD0				cmdState7
#define cmdSetpointCr				cmdState8
#define cmdSetpointLf				cmdState9
#define cmdSym						cmdState10
#define cmdSymD0					cmdState11
#define CMD_STATE_SYM				d'11'
#define cmdSymCr					cmdState12
#define cmdSymLf					cmdState13
#define cmdAliveCr					cmdState14
#define CMD_STATE_KEEP_ALIVE		d'14'
#define cmdAliveLf					cmdState15


CmdJump:
;only execute the state machine when a char has been received.
	#define rxproducer 	reg0
	#define rxbyte		reg1
	movf	myRs232RxProducer,w,BANKED	;myRs232Producer can increase under interrupt,
	movwf	rxproducer,BANKED			;so store in more stable place
	xorwf	myRs232RxConsumer,w,BANKED
	bz		cmdDone						;producer = consumer -> nothing received, nothing to do here
	
	lfsr	1,myRs232RxBuf				;fsr1 is not used in interrupts, so can be used here
	movf	myRs232RxConsumer,w,BANKED
	movff	PLUSW1,rxbyte
	addlw	d'1'
	andlw	SERIAL_BUF_MASK
	movwf	myRs232RxConsumer,BANKED

	movf	myCmdState,w,BANKED
	call	Switchcase16
	bra		cmdState0
	bra		cmdState1
	bra		cmdState2
	bra		cmdState3
	bra		cmdState4
	bra		cmdState5
	bra		cmdState6
	bra		cmdState7
	bra		cmdState8
	bra		cmdState9
	bra		cmdState10
	bra		cmdState11
	bra		cmdState12
	bra		cmdState13
	bra		cmdState14
	bra		cmdState15

cmdStateIdle:
	;check first byte of command, can be 'a' (keepalive) of 'S'..'Z' (Thermostat 0..7)
	movf	rxbyte,w,BANKED

	xorlw	RS232_CMD_KEEPALIVE
	bz		cmdKeepAlive
	xorlw	RS232_CMD_SELECT_THERM_0^RS232_CMD_KEEPALIVE
	bz		cmdSelect0
	xorlw	RS232_CMD_SELECT_THERM_1^RS232_CMD_SELECT_THERM_0
	bz		cmdSelect1
	xorlw	RS232_CMD_SELECT_THERM_2^RS232_CMD_SELECT_THERM_1
	bz		cmdSelect2
	xorlw	RS232_CMD_SELECT_THERM_3^RS232_CMD_SELECT_THERM_2
	bz		cmdSelect3
	xorlw	RS232_CMD_SELECT_THERM_4^RS232_CMD_SELECT_THERM_3
	bz		cmdSelect4
	xorlw	RS232_CMD_SELECT_THERM_5^RS232_CMD_SELECT_THERM_4
	bz		cmdSelect5
	xorlw	RS232_CMD_SELECT_THERM_6^RS232_CMD_SELECT_THERM_5
	bz		cmdSelect6
	xorlw	RS232_CMD_SELECT_THERM_7^RS232_CMD_SELECT_THERM_6
	bz		cmdSelect7
	;command unknown, ignore until a valid command has been received
	bra		cmdDone

cmdKeepAlive:
	movlw	CMD_STATE_KEEP_ALIVE
	bra		cmdSetState

cmdSelect0:
cmdSelect1:
cmdSelect2:
cmdSelect3:
cmdSelect4:
cmdSelect5:
cmdSelect6:
cmdSelect7:
	movlw	RS232_CMD_SELECT_THERM_0
	subwf	rxbyte,w,BANKED	;w how holds the thermostat channel, 0..7
	movwf	myCmdThermostat,BANKED
	movlw	CMD_STATE_COMMAND
	bra		cmdSetState

	
cmdStateThermostat:	;state unused, should be removed
	bra		cmdResetState

cmdStateCommand:
	;command can be 'G'(Setpoint), 'F'(Symbol Flame), 'B'(Symbol Boiler)
	movf	rxbyte,w,BANKED
	xorlw	RS232_SMD_SET_TEMP
	bz		cmdSetTemp
	xorlw	RS232_CMD_SET_SYM_FLAME^RS232_SMD_SET_TEMP
	bz		cmdSetFlame
	xorlw	RS232_CMD_SET_SYM_BOILER^RS232_CMD_SET_SYM_FLAME
	bz		cmdSetBoiler
	bra		cmdResetState ;command unknown, reset the command handler

cmdSetTemp:
	movlw	CMD_STATE_SETPOINT
	bra		cmdSetState
cmdSetFlame:
	bsf		bitCmdSymFlame
	movlw	CMD_STATE_SYM
	bra		cmdSetState
cmdSetBoiler:
	bsf		bitCmdSymBoil
	movlw	CMD_STATE_SYM
	bra		cmdSetState
	
cmdSetpoint:	;state unused, should be removed
	bra		cmdResetState

cmdSetpointD3:
	movf	rxbyte,w,BANKED
	call	ascii2nibble
	swapf	WREG,f,ACCESS
	movwf	myCmdData+HIGH_BYTE,BANKED
	bra		cmdNextState

cmdSetpointD2:
	movf	rxbyte,w,BANKED
	call	ascii2nibble
	iorwf	myCmdData+HIGH_BYTE,f,BANKED	
	bra		cmdNextState

cmdSetpointD1:
	movf	rxbyte,w,BANKED
	call	ascii2nibble
	swapf	WREG,f,ACCESS
	movwf	myCmdData+LOW_BYTE,BANKED
	bra		cmdNextState

cmdSetpointD0:
	movf	rxbyte,w,BANKED
	call	ascii2nibble
	iorwf	myCmdData+LOW_BYTE,f,BANKED	
	bra		cmdNextState
			
cmdSetpointCr:
	movf	rxbyte,w
	xorlw	RS232_CR
	bz		cmdNextState
	bra		cmdResetState

cmdSetpointLf: ;check for lf and execute
	movf	rxbyte,w
	xorlw	RS232_LF
	bnz		cmdResetState
	;;execute command here
	movf	myCmdThermostat,w,BANKED
	bz		cmdSet0
	decf	WREG,f,ACCESS
	bz		cmdSet1
	decf	WREG,f,ACCESS
	bz		cmdSet2
	decf	WREG,f,ACCESS
	bz		cmdSet3
	decf	WREG,f,ACCESS
	bz		cmdSet4
	decf	WREG,f,ACCESS
	bz		cmdSet5
	decf	WREG,f,ACCESS
	bz		cmdSet6
	decf	WREG,f,ACCESS
	bz		cmdSet7
;;possible fall true when myCmdThermostat is > 7
;in the middle for branch instructions
cmdResetState:
	clrf	myCmdState,BANKED
	bra		cmdDone

cmdSetState:
	movwf	myCmdState,BANKED
	bra		cmdDone
cmdNextState:
	incf	myCmdState,f,BANKED
cmdDone:
	return

cmdSet0:
	movff	myCmdData+LOW_BYTE,myCmd0Setpoint+LOW_BYTE
	movff	myCmdData+HIGH_BYTE,myCmd0Setpoint+HIGH_BYTE
	bra		cmdSetpointDone
cmdSet1:
	movff	myCmdData+LOW_BYTE,myCmd1Setpoint+LOW_BYTE
	movff	myCmdData+HIGH_BYTE,myCmd1Setpoint+HIGH_BYTE
	bra		cmdSetpointDone
cmdSet2:
	movff	myCmdData+LOW_BYTE,myCmd2Setpoint+LOW_BYTE
	movff	myCmdData+HIGH_BYTE,myCmd2Setpoint+HIGH_BYTE
	bra		cmdSetpointDone
cmdSet3:
	movff	myCmdData+LOW_BYTE,myCmd3Setpoint+LOW_BYTE
	movff	myCmdData+HIGH_BYTE,myCmd3Setpoint+HIGH_BYTE
	bra		cmdSetpointDone
cmdSet4:
	movff	myCmdData+LOW_BYTE,myCmd4Setpoint+LOW_BYTE
	movff	myCmdData+HIGH_BYTE,myCmd4Setpoint+HIGH_BYTE
	bra		cmdSetpointDone
cmdSet5:
	movff	myCmdData+LOW_BYTE,myCmd5Setpoint+LOW_BYTE
	movff	myCmdData+HIGH_BYTE,myCmd5Setpoint+HIGH_BYTE
	bra		cmdSetpointDone
cmdSet6:
	movff	myCmdData+LOW_BYTE,myCmd6Setpoint+LOW_BYTE
	movff	myCmdData+HIGH_BYTE,myCmd6Setpoint+HIGH_BYTE
	bra		cmdSetpointDone
cmdSet7:
	movff	myCmdData+LOW_BYTE,myCmd7Setpoint+LOW_BYTE
	movff	myCmdData+HIGH_BYTE,myCmd7Setpoint+HIGH_BYTE
	bra		cmdSetpointDone
cmdSetpointDone:
	bra		cmdResetState

cmdSym: ;state unused, remove
	bra		cmdResetState
			
cmdSymD0:
	movff	rxbyte,myCmdData+LOW_BYTE
	bra		cmdNextState				
cmdSymCr:
	movf	rxbyte,w
	xorlw	RS232_CR
	bz		cmdNextState
	bra		cmdResetState				
cmdSymLf:		;check for lf and execute
	movf	rxbyte,w
	xorlw	RS232_LF
	bnz		cmdResetState
	;;execute command here
	movf	myCmdThermostat,w,BANKED
	bz		cmdSym0
	decf	WREG,f,ACCESS
	bz		cmdSym1
	decf	WREG,f,ACCESS
	bz		cmdSym2
	decf	WREG,f,ACCESS
	bz		cmdSym3
	decf	WREG,f,ACCESS
	bz		cmdSym4
	decf	WREG,f,ACCESS
	bz		cmdSym5
	decf	WREG,f,ACCESS
	bz		cmdSym6
	decf	WREG,f,ACCESS
	bz		cmdSym7

cmdSym0:
	movf	myCmdData+LOW_BYTE,w,BANKED
	xorlw	'0'
	bz		

	bcf		bit0SymFlame
	btfsc	bitCmdSymFlame
	bsf 	bit0SymFlame
	bra		cmdSymDone
cmdSym1:
	bcf		bit1SymFlame
	btfsc	bitCmdSymFlame
	bsf 	bit1SymFlame
	bra		cmdSymDone
cmdSym2:
	bcf		bit2SymFlame
	btfsc	bitCmdSymFlame
	bsf 	bit2SymFlame
	bra		cmdSymDone
cmdSym3:
	bcf		bit3SymFlame
	btfsc	bitCmdSymFlame
	bsf 	bit3SymFlame
	bra		cmdSymDone
cmdSym4:
	bcf		bit4SymFlame
	btfsc	bitCmdSymFlame
	bsf 	bit4SymFlame
	bra		cmdSymDone
cmdSym5:
	bcf		bit5SymFlame
	btfsc	bitCmdSymFlame
	bsf 	bit5SymFlame
	bra		cmdSymDone
cmdSym6:
	bcf		bit6SymFlame
	btfsc	bitCmdSymFlame
	bsf 	bit6SymFlame
	bra		cmdSymDone
cmdSym7:
	bcf		bit7SymFlame
	btfsc	bitCmdSymFlame
	bsf 	bit7SymFlame
	bra		cmdSymDone
cmdSymDone:
	bra		cmdResetState
		
cmdAliveCr:
	movf	rxbyte,w
	xorlw	RS232_CR
	bz		cmdNextState
	bra		cmdResetState
cmdAliveLf:
	movf	rxbyte,w
	xorlw	RS232_LF
	bnz		cmdResetState
	;;execute command here
	bra		cmdResetState


