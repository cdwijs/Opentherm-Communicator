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
#define MAX_THERMOSTAT				d'7'

#define RS232_SMD_SET_TEMP			'G'
#define RS232_CMD_SET_SYM_FLAME		'F'
#define RS232_CMD_SET_SYM_BOILER	'B'

#define RS232_CR					h'0D'
#define RS232_LF					h'0A'

#define cmdStateIdle				cmdState0
#define CMD_STATE_IDLE				d'0'
#define cmdStateCommand				cmdState1
#define CMD_STATE_COMMAND			d'1'
#define cmdSetpointD3				cmdState2
#define CMD_STATE_D3				d'2'
#define cmdSetpointD2				cmdState3
#define cmdSetpointD1				cmdState4
#define cmdSetpointD0				cmdState5
#define CMD_STATE_D0				d'5'
#define cmdSetpointCr				cmdState6
#define cmdSetpointLf				cmdState7

CmdJump:
;only execute the state machine when a char has been received.
	#define rxproducer 	reg0
	#define rxbyte		reg1
	movf	myRs232RxProducer,w,BANKED	;myRs232Producer can increase under interrupt,
	movwf	rxproducer,BANKED			;so store in more stable place
	xorwf	myRs232RxConsumer,w,BANKED
	btfsc	Zero
	goto	cmdDone						;producer = consumer -> nothing received, nothing to do here
	
	lfsr	1,myRs232RxBuf				;fsr1 is not used in interrupts, so can be used here
	movf	myRs232RxConsumer,w,BANKED
	movff	PLUSW1,rxbyte
	addlw	d'1'
	andlw	SERIAL_BUF_MASK
	movwf	myRs232RxConsumer,BANKED

	movf	myCmdState,w,BANKED
	call	Switchcase8
	bra		cmdState0
	bra		cmdState1
	bra		cmdState2
	bra		cmdState3
	bra		cmdState4
	bra		cmdState5
	bra		cmdState6
	bra		cmdState7

cmdStateIdle:
	;check first byte of command, can be 'a' (keepalive) of 'S'..'Z' (Thermostat 0..7)
	movf	rxbyte,w,BANKED

	xorlw	RS232_CMD_KEEPALIVE
	bz		cmdKeepAlive
	movlw	RS232_CMD_SELECT_THERM_0
	subwf	rxbyte,w,BANKED	;w how holds the thermostat channel, 0..7
	movwf	myCmdThermostat,BANKED
	sublw	MAX_THERMOSTAT ;carry set when thermostat 0..7
	bnc		cmdResetState		   ;command unknown, ignore until a valid command has been received	
	movlw	CMD_STATE_COMMAND
	bra		cmdSetState

cmdKeepAlive:
;	movlw	CMD_STATE_KEEP_ALIVE
;	bra		cmdSetState
	bra		cmdResetState ;;;;keepalive for now disabled

cmdStateCommand:
	;command can be 'G'(Setpoint), 'F'(Symbol Flame), 'B'(Symbol Boiler)
	clrf	myCmdData+HIGH_BYTE,BANKED
	clrf	myCmdData+LOW_BYTE,BANKED
	movf	rxbyte,w,BANKED
	movwf	myCmd,BANKED
	
	xorlw	RS232_SMD_SET_TEMP
	movlw	CMD_STATE_D0;other commands have only 1 data nibble
	btfsc	Zero	;set temp command has 4 data nibbles
	movlw	CMD_STATE_D3	
	bra		cmdSetState

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

cmdSetpointLf: ;check for lf
	movf	rxbyte,w
	xorlw	RS232_LF
	bnz		cmdResetState
	
	;execute command here
	movf	myCmd,w,BANKED
	xorlw	RS232_SMD_SET_TEMP
	bz		cmdSetTemp
	xorlw	RS232_CMD_SET_SYM_FLAME^RS232_SMD_SET_TEMP
	bz		cmdSetFlame
	xorlw	RS232_CMD_SET_SYM_BOILER^RS232_CMD_SET_SYM_FLAME
	bz		cmdSetBoiler
	bra		cmdResetState ;unknown command
	
cmdSetTemp:
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

cmdSetFlame:
	movf	myCmdThermostat,w,BANKED
	bz		cmdFlame0
	decf	WREG,f,ACCESS
	bz		cmdFlame1
	decf	WREG,f,ACCESS
	bz		cmdFlame2
	decf	WREG,f,ACCESS
	bz		cmdFlame3
	decf	WREG,f,ACCESS
	bz		cmdFlame4
	decf	WREG,f,ACCESS
	bz		cmdFlame5
	decf	WREG,f,ACCESS
	bz		cmdFlame6
	decf	WREG,f,ACCESS
	bz		cmdFlame7
	
cmdFlame0:
	bcf		bit0SymFlame
	movf	myCmdData+LOW_BYTE,w,BANKED
	btfss	Zero
	bsf		bit0SymFlame
	bra		cmdFlameDone
cmdFlame1:
	bcf		bit1SymFlame
	movf	myCmdData+LOW_BYTE,w,BANKED
	btfss	Zero
	bsf		bit1SymFlame
	bra		cmdFlameDone
cmdFlame2:
	bcf		bit2SymFlame
	movf	myCmdData+LOW_BYTE,w,BANKED
	btfss	Zero
	bsf		bit2SymFlame
	bra		cmdFlameDone
cmdFlame3:
	bcf		bit3SymFlame
	movf	myCmdData+LOW_BYTE,w,BANKED
	btfss	Zero
	bsf		bit3SymFlame
	bra		cmdFlameDone
cmdFlame4:
	bcf		bit4SymFlame
	movf	myCmdData+LOW_BYTE,w,BANKED
	btfss	Zero
	bsf		bit4SymFlame
	bra		cmdFlameDone
cmdFlame5:
	bcf		bit5SymFlame
	movf	myCmdData+LOW_BYTE,w,BANKED
	btfss	Zero
	bsf		bit5SymFlame
	bra		cmdFlameDone
cmdFlame6:
	bcf		bit6SymFlame
	movf	myCmdData+LOW_BYTE,w,BANKED
	btfss	Zero
	bsf		bit6SymFlame
	bra		cmdFlameDone
cmdFlame7:
	bcf		bit7SymFlame
	movf	myCmdData+LOW_BYTE,w,BANKED
	btfss	Zero
	bsf		bit7SymFlame
	;bra		cmdFlameDone
cmdFlameDone:
	bra		cmdResetState


	
cmdSetBoiler:
	movf	myCmdThermostat,w,BANKED
	bz		cmdBoiler0
	decf	WREG,f,ACCESS
	bz		cmdBoiler1
	decf	WREG,f,ACCESS
	bz		cmdBoiler2
	decf	WREG,f,ACCESS
	bz		cmdBoiler3
	decf	WREG,f,ACCESS
	bz		cmdBoiler4
	decf	WREG,f,ACCESS
	bz		cmdBoiler5
	decf	WREG,f,ACCESS
	bz		cmdBoiler6
	decf	WREG,f,ACCESS
	bz		cmdBoiler7	
	
cmdBoiler0:
	bcf		bit0SymBoil
	movf	myCmdData+LOW_BYTE,w,BANKED
	btfss	Zero
	bsf		bit0SymBoil
	bra		cmdBoilDone
cmdBoiler1:
	bcf		bit1SymBoil
	movf	myCmdData+LOW_BYTE,w,BANKED
	btfss	Zero
	bsf		bit1SymBoil
	bra		cmdBoilDone
cmdBoiler2:
	bcf		bit2SymBoil
	movf	myCmdData+LOW_BYTE,w,BANKED
	btfss	Zero
	bsf		bit2SymBoil
	bra		cmdBoilDone
cmdBoiler3:
	bcf		bit3SymBoil
	movf	myCmdData+LOW_BYTE,w,BANKED
	btfss	Zero
	bsf		bit3SymBoil
	bra		cmdBoilDone
cmdBoiler4:
	bcf		bit4SymBoil
	movf	myCmdData+LOW_BYTE,w,BANKED
	btfss	Zero
	bsf		bit4SymBoil
	bra		cmdBoilDone
cmdBoiler5:
	bcf		bit5SymBoil
	movf	myCmdData+LOW_BYTE,w,BANKED
	btfss	Zero
	bsf		bit5SymBoil
	bra		cmdBoilDone
cmdBoiler6:
	bcf		bit6SymBoil
	movf	myCmdData+LOW_BYTE,w,BANKED
	btfss	Zero
	bsf		bit6SymBoil
	bra		cmdBoilDone
cmdBoiler7:
	bcf		bit7SymBoil
	movf	myCmdData+LOW_BYTE,w,BANKED
	btfss	Zero
	bsf		bit7SymBoil
	;bra		cmdBoilDone
cmdBoilDone:
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

