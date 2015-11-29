;*******************************************************************************
;    ot-comm.inc                                                               *
;    Copyright 2013-2014 Cedric de Wijs.                                       *
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


#define TIME_COMM_RESPONSE	d'20' ;*msec


#define ID_MASTER_SLAVE_FLAGS				d'0'
#define ID_CH_WATER_TEMP_SETPOINT			d'1'
#define ID_REMOTE_OVERWRITE_ROOM_SETP		d'9'
#define ID_MAX_REL_MOD_LEVEL				d'14'
#define ID_MAX_BOILER_CAPACITY				d'15'
#define ID_ROOM_SETPOINT					d'16'
#define ID_RELATIVE_MODULATION_LEVEL		d'17'
#define ID_WATER_PRESSURE_IN_CH_CIRCUIT		d'18'
#define ID_DAY_OF_WEEK_AND_TIME_OF_DAY		d'20'
#define ID_ROOM_TEMPERATURE					d'24'
#define ID_BOILER_FLOW_WATER_TEMPERATURE	d'25'
#define ID_DHW_TEMPERATURE					d'26'
#define ID_OUTSIDE_TEMPERATURE				d'27'
#define ID_RETURN_WATER_TEMPERATURE			d'28'
#define ID_DHW_SETPOINT						d'56'
#define ID_MAX_CH_WATER_SETPOINT			d'57'
#define ID_REMOTE_OVERRIDE_FUNCTION			d'100' 

#define OFFSET_MSG_TYPE						d'3'
#define OFFSET_MSG_ID						d'2'
#define OFFSET_DATA_HIGH					d'1'
#define OFFSET_DATA_LOW						d'0'

#define MSG_TYPE_READ_DATA					d'0'
#define MSG_TYPE_WRITE_DATA					d'1'
#define MSG_TYPE_INVALID_DATA				d'2'
#define MSG_TYPE_RESERVED					d'3'
#define MSG_TYPE_READ_ACK					d'4'
#define MSG_TYPE_WRITE_ACK					d'5'
#define MSG_TYPE_DATA_INVALID				d'6'
#define MSG_TYPE_UNKNOWN_DATA_ID			d'7'

commJumps:
#ifdef USE_CHANNEL_0
	#define COMM_CHANNEL		0
	;labels
	#define commState0			comm0State0
	#define commState1			comm0State1
	#define commState2			comm0State2
	#define commState3			comm0State3
	#define commState4			comm0State4
	#define commState5			comm0State5
	#define commState6			comm0State6
	#define commState7			comm0State7
	#define commNextState		comm0NextState
	#define commDone			comm0Done
	#define commIdSlaveFlags			comm0IdSlaveFlags
	#define commIdTempSetpoint			comm0IdTempSetpoint
	#define commIdRemoteOverwrite		comm0IdRemoteOverwrite
	#define commIdMaxRelMod				comm0IdMaxRelMod
	#define commIdMaxBoilerCapacity		comm0IdMaxBoilerCapacity
	#define commIdRoomSetpoint			comm0IdRoomSetpoint
	#define commIdRoomSetpointDiff		comm0IdRoomSetpointDiff
	#define commIdRelModLevel			comm0IdRelModLevel
	#define commIdWaterPressInCh		comm0IdWaterPressInCh
	#define commIdDayOfWeek				comm0IdDayOfWeek
	#define commIdRoomTemp				comm0IdRoomTemp
	#define commIdFlowWaterTemp			comm0IdFlowWaterTemp
	#define commIdDwhTemp				comm0IdDwhTemp
	#define commIdOutsideTemp			comm0IdOutsideTemp
	#define commIdReturnWaterTemp		comm0IdReturnWaterTemp
	#define commIdDhwSetpoint			comm0IdDhwSetpoint
	#define commIdMaxChWaterSetpoint	comm0IdMaxChWaterSetpoint
	#define commIdRemoteOverRide		comm0IdRemoteOverRide
	#define commStateCheckKnown			comm0StateCheckKnown
	#define commReadData				comm0ReadData
	#define commWriteData				comm0WriteData
	#define commWriteInvalid			comm0WriteInvalid
	#define commStateCheckWrongMsgtype	comm0StateCheckWrongMsgtype
	;data
	#define myCommState			myComm0State
	#define myCommTimer			myComm0Timer
	#define myRxBuffer			myRx0Buffer
	#define myTxBuffer			myTx0Buffer
	#define myCmdSetpoint		myCmd0Setpoint
	;bits
	#define bitCommBusy			bitComm0Busy
	#define bitCommStartMsg		bitComm0StartMsg
	#define bitRxStart			bitRx0Start
	#define bitRxBusy			bitRx0Busy
	#define bitTxStart			bitTx0Start
	#define bitTxBusy			bitTx0Busy
	#include ot-comm-states.inc
#endif

#ifdef USE_CHANNEL_1
	#define COMM_CHANNEL		1
	;labels
	#define commState0			comm1State0
	#define commState1			comm1State1
	#define commState2			comm1State2
	#define commState3			comm1State3
	#define commState4			comm1State4
	#define commState5			comm1State5
	#define commState6			comm1State6
	#define commState7			comm1State7
	#define commNextState		comm1NextState
	#define commDone			comm1Done
	#define commIdSlaveFlags			comm1IdSlaveFlags
	#define commIdTempSetpoint			comm1IdTempSetpoint
	#define commIdRemoteOverwrite		comm1IdRemoteOverwrite
	#define commIdMaxRelMod				comm1IdMaxRelMod
	#define commIdMaxBoilerCapacity		comm1IdMaxBoilerCapacity
	#define commIdRoomSetpoint			comm1IdRoomSetpoint
	#define commIdRoomSetpointDiff		comm1IdRoomSetpointDiff
	#define commIdRelModLevel			comm1IdRelModLevel
	#define commIdWaterPressInCh		comm1IdWaterPressInCh
	#define commIdDayOfWeek				comm1IdDayOfWeek
	#define commIdRoomTemp				comm1IdRoomTemp
	#define commIdFlowWaterTemp			comm1IdFlowWaterTemp
	#define commIdDwhTemp				comm1IdDwhTemp
	#define commIdOutsideTemp			comm1IdOutsideTemp
	#define commIdReturnWaterTemp		comm1IdReturnWaterTemp
	#define commIdDhwSetpoint			comm1IdDhwSetpoint
	#define commIdMaxChWaterSetpoint	comm1IdMaxChWaterSetpoint
	#define commIdRemoteOverRide		comm1IdRemoteOverRide
	#define commStateCheckKnown			comm1StateCheckKnown
	#define commReadData				comm1ReadData
	#define commWriteData				comm1WriteData
	#define commWriteInvalid			comm1WriteInvalid
	#define commStateCheckWrongMsgtype	comm1StateCheckWrongMsgtype
	;data
	#define myCommState			myComm1State
	#define myCommTimer			myComm1Timer
	#define myRxBuffer			myRx1Buffer
	#define myTxBuffer			myTx1Buffer
	#define myCmdSetpoint		myCmd1Setpoint
	;bits
	#define bitCommBusy			bitComm1Busy
	#define bitCommStartMsg		bitComm1StartMsg
	#define bitRxStart			bitRx1Start
	#define bitRxBusy			bitRx1Busy
	#define bitTxStart			bitTx1Start
	#define bitTxBusy			bitTx1Busy
	#include ot-comm-states.inc
#endif

#ifdef USE_CHANNEL_2
	#define COMM_CHANNEL		2
	;labels
	#define commState0			comm2state0
	#define commState1			comm2State1
	#define commState2			comm2State2
	#define commState3			comm2State3
	#define commState4			comm2State4
	#define commState5			comm2State5
	#define commState6			comm2State6
	#define commState7			comm2State7
	#define commNextState		comm2NextState
	#define commDone			comm2Done
	#define commIdSlaveFlags			comm2IdSlaveFlags
	#define commIdTempSetpoint			comm2IdTempSetpoint
	#define commIdRemoteOverwrite		comm2IdRemoteOverwrite
	#define commIdMaxRelMod				comm2IdMaxRelMod
	#define commIdMaxBoilerCapacity		comm2IdMaxBoilerCapacity
	#define commIdRoomSetpoint			comm2IdRoomSetpoint
	#define commIdRoomSetpointDiff		comm2IdRoomSetpointDiff
	#define commIdRelModLevel			comm2IdRelModLevel
	#define commIdWaterPressInCh		comm2IdWaterPressInCh
	#define commIdDayOfWeek				comm2IdDayOfWeek
	#define commIdRoomTemp				comm2IdRoomTemp
	#define commIdFlowWaterTemp			comm2IdFlowWaterTemp
	#define commIdDwhTemp				comm2IdDwhTemp
	#define commIdOutsideTemp			comm2IdOutsideTemp
	#define commIdReturnWaterTemp		comm2IdReturnWaterTemp
	#define commIdDhwSetpoint			comm2IdDhwSetpoint
	#define commIdMaxChWaterSetpoint	comm2IdMaxChWaterSetpoint
	#define commIdRemoteOverRide		comm2IdRemoteOverRide
	#define commStateCheckKnown			comm2StateCheckKnown
	#define commReadData				comm2ReadData
	#define commWriteData				comm2WriteData
	#define commWriteInvalid			comm2WriteInvalid
	#define commStateCheckWrongMsgtype	comm2StateCheckWrongMsgtype
	;data
	#define myCommState			myComm2State
	#define myCommTimer			myComm2Timer
	#define myRxBuffer			myRx2Buffer
	#define myTxBuffer			myTx2Buffer
	#define myCmdSetpoint		myCmd2Setpoint
	;bits
	#define bitCommBusy			bitComm2Busy
	#define bitCommStartMsg		bitComm2StartMsg
	#define bitRxStart			bitRx2Start
	#define bitRxBusy			bitRx2Busy
	#define bitTxStart			bitTx2Start
	#define bitTxBusy			bitTx2Busy
	#include ot-comm-states.inc
#endif

#ifdef USE_CHANNEL_3
	#define COMM_CHANNEL		3
	;labels
	#define commState0			comm3State0
	#define commState1			comm3State1
	#define commState2			comm3State2
	#define commState3			comm3State3
	#define commState4			comm3State4
	#define commState5			comm3State5
	#define commState6			comm3State6
	#define commState7			comm3State7
	#define commNextState		comm3NextState
	#define commDone			comm3Done
	#define commIdSlaveFlags			comm3IdSlaveFlags
	#define commIdTempSetpoint			comm3IdTempSetpoint
	#define commIdRemoteOverwrite		comm3IdRemoteOverwrite
	#define commIdMaxRelMod				comm3IdMaxRelMod
	#define commIdMaxBoilerCapacity		comm3IdMaxBoilerCapacity
	#define commIdRoomSetpoint			comm3IdRoomSetpoint
	#define commIdRoomSetpointDiff		comm3IdRoomSetpointDiff
	#define commIdRelModLevel			comm3IdRelModLevel
	#define commIdWaterPressInCh		comm3IdWaterPressInCh
	#define commIdDayOfWeek				comm3IdDayOfWeek
	#define commIdRoomTemp				comm3IdRoomTemp
	#define commIdFlowWaterTemp			comm3IdFlowWaterTemp
	#define commIdDwhTemp				comm3IdDwhTemp
	#define commIdOutsideTemp			comm3IdOutsideTemp
	#define commIdReturnWaterTemp		comm3IdReturnWaterTemp
	#define commIdDhwSetpoint			comm3IdDhwSetpoint
	#define commIdMaxChWaterSetpoint	comm3IdMaxChWaterSetpoint
	#define commIdRemoteOverRide		comm3IdRemoteOverRide
	#define commStateCheckKnown			comm3StateCheckKnown
	#define commReadData				comm3ReadData
	#define commWriteData				comm3WriteData
	#define commWriteInvalid			comm3WriteInvalid
	#define commStateCheckWrongMsgtype	comm3StateCheckWrongMsgtype
	;data
	#define myCommState			myComm3State
	#define myCommTimer			myComm3Timer
	#define myRxBuffer			myRx3Buffer
	#define myTxBuffer			myTx3Buffer
	#define myCmdSetpoint		myCmd3Setpoint
	;bits
	#define bitCommBusy			bitComm3Busy
	#define bitCommStartMsg		bitComm3StartMsg
	#define bitRxStart			bitRx3Start
	#define bitRxBusy			bitRx3Busy
	#define bitTxStart			bitTx3Start
	#define bitTxBusy			bitTx3Busy
	#include ot-comm-states.inc
#endif

#ifdef USE_CHANNEL_4
	#define COMM_CHANNEL		4
	;labels
	#define commState0			comm4State0
	#define commState1			comm4State1
	#define commState2			comm4State2
	#define commState3			comm4State3
	#define commState4			comm4State4
	#define commState5			comm4State5
	#define commState6			comm4State6
	#define commState7			comm4State7
	#define commNextState		comm4NextState
	#define commDone			comm4Done
	#define commIdSlaveFlags			comm4IdSlaveFlags
	#define commIdTempSetpoint			comm4IdTempSetpoint
	#define commIdRemoteOverwrite		comm4IdRemoteOverwrite
	#define commIdMaxRelMod				comm4IdMaxRelMod
	#define commIdMaxBoilerCapacity		comm4IdMaxBoilerCapacity
	#define commIdRoomSetpoint			comm4IdRoomSetpoint
	#define commIdRoomSetpointDiff		comm4IdRoomSetpointDiff
	#define commIdRelModLevel			comm4IdRelModLevel
	#define commIdWaterPressInCh		comm4IdWaterPressInCh
	#define commIdDayOfWeek				comm4IdDayOfWeek
	#define commIdRoomTemp				comm4IdRoomTemp
	#define commIdFlowWaterTemp			comm4IdFlowWaterTemp
	#define commIdDwhTemp				comm4IdDwhTemp
	#define commIdOutsideTemp			comm4IdOutsideTemp
	#define commIdReturnWaterTemp		comm4IdReturnWaterTemp
	#define commIdDhwSetpoint			comm4IdDhwSetpoint
	#define commIdMaxChWaterSetpoint	comm4IdMaxChWaterSetpoint
	#define commIdRemoteOverRide		comm4IdRemoteOverRide
	#define commStateCheckKnown			comm4StateCheckKnown
	#define commReadData				comm4ReadData
	#define commWriteData				comm4WriteData
	#define commWriteInvalid			comm4WriteInvalid
	#define commStateCheckWrongMsgtype	comm4StateCheckWrongMsgtype
	;data
	#define myCommState			myComm4State
	#define myCommTimer			myComm4Timer
	#define myRxBuffer			myRx4Buffer
	#define myTxBuffer			myTx4Buffer
	#define myCmdSetpoint		myCmd4Setpoint
	;bits
	#define bitCommBusy			bitComm4Busy
	#define bitCommStartMsg		bitComm4StartMsg
	#define bitRxStart			bitRx4Start
	#define bitRxBusy			bitRx4Busy
	#define bitTxStart			bitTx4Start
	#define bitTxBusy			bitTx4Busy
	#include ot-comm-states.inc
#endif

#ifdef USE_CHANNEL_5
	#define COMM_CHANNEL		5
	;labels
	#define commState0			comm5State0
	#define commState1			comm5State1
	#define commState2			comm5State2
	#define commState3			comm5State3
	#define commState4			comm5State4
	#define commState5			comm5State5
	#define commState6			comm5State6
	#define commState7			comm5State7
	#define commNextState		comm5NextState
	#define commDone			comm5Done
	#define commIdSlaveFlags			comm5IdSlaveFlags
	#define commIdTempSetpoint			comm5IdTempSetpoint
	#define commIdRemoteOverwrite		comm5IdRemoteOverwrite
	#define commIdMaxRelMod				comm5IdMaxRelMod
	#define commIdMaxBoilerCapacity		comm5IdMaxBoilerCapacity
	#define commIdRoomSetpoint			comm5IdRoomSetpoint
	#define commIdRoomSetpointDiff		comm5IdRoomSetpointDiff
	#define commIdRelModLevel			comm5IdRelModLevel
	#define commIdWaterPressInCh		comm5IdWaterPressInCh
	#define commIdDayOfWeek				comm5IdDayOfWeek
	#define commIdRoomTemp				comm5IdRoomTemp
	#define commIdFlowWaterTemp			comm5IdFlowWaterTemp
	#define commIdDwhTemp				comm5IdDwhTemp
	#define commIdOutsideTemp			comm5IdOutsideTemp
	#define commIdReturnWaterTemp		comm5IdReturnWaterTemp
	#define commIdDhwSetpoint			comm5IdDhwSetpoint
	#define commIdMaxChWaterSetpoint	comm5IdMaxChWaterSetpoint
	#define commIdRemoteOverRide		comm5IdRemoteOverRide
	#define commStateCheckKnown			comm5StateCheckKnown
	#define commReadData				comm5ReadData
	#define commWriteData				comm5WriteData
	#define commWriteInvalid			comm5WriteInvalid
	#define commStateCheckWrongMsgtype	comm5StateCheckWrongMsgtype
	;data
	#define myCommState			myComm5State
	#define myCommTimer			myComm5Timer
	#define myRxBuffer			myRx5Buffer
	#define myTxBuffer			myTx5Buffer
	#define myCmdSetpoint		myCmd5Setpoint
	;bits
	#define bitCommBusy			bitComm5Busy
	#define bitCommStartMsg		bitComm5StartMsg
	#define bitRxStart			bitRx5Start
	#define bitRxBusy			bitRx5Busy
	#define bitTxStart			bitTx5Start
	#define bitTxBusy			bitTx5Busy
	#include ot-comm-states.inc
#endif

#ifdef USE_CHANNEL_6
	#define COMM_CHANNEL		6
	;labels
	#define commState0			comm6State0
	#define commState1			comm6State1
	#define commState2			comm6State2
	#define commState3			comm6State3
	#define commState4			comm6State4
	#define commState5			comm6State5
	#define commState6			comm6State6
	#define commState7			comm6State7
	#define commNextState		comm6NextState
	#define commDone			comm6Done
	#define commIdSlaveFlags			comm6IdSlaveFlags
	#define commIdTempSetpoint			comm6IdTempSetpoint
	#define commIdRemoteOverwrite		comm6IdRemoteOverwrite
	#define commIdMaxRelMod				comm6IdMaxRelMod
	#define commIdMaxBoilerCapacity		comm6IdMaxBoilerCapacity
	#define commIdRoomSetpoint			comm6IdRoomSetpoint
	#define commIdRoomSetpointDiff		comm6IdRoomSetpointDiff
	#define commIdRelModLevel			comm6IdRelModLevel
	#define commIdWaterPressInCh		comm6IdWaterPressInCh
	#define commIdDayOfWeek				comm6IdDayOfWeek
	#define commIdRoomTemp				comm6IdRoomTemp
	#define commIdFlowWaterTemp			comm6IdFlowWaterTemp
	#define commIdDwhTemp				comm6IdDwhTemp
	#define commIdOutsideTemp			comm6IdOutsideTemp
	#define commIdReturnWaterTemp		comm6IdReturnWaterTemp
	#define commIdDhwSetpoint			comm6IdDhwSetpoint
	#define commIdMaxChWaterSetpoint	comm6IdMaxChWaterSetpoint
	#define commIdRemoteOverRide		comm6IdRemoteOverRide
	#define commStateCheckKnown			comm6StateCheckKnown
	#define commReadData				comm6ReadData
	#define commWriteData				comm6WriteData
	#define commWriteInvalid			comm6WriteInvalid
	#define commStateCheckWrongMsgtype	comm6StateCheckWrongMsgtype
	;data
	#define myCommState			myComm6State
	#define myCommTimer			myComm6Timer
	#define myRxBuffer			myRx6Buffer
	#define myTxBuffer			myTx6Buffer
	#define myCmdSetpoint		myCmd6Setpoint
	;bits
	#define bitCommBusy			bitComm6Busy
	#define bitCommStartMsg		bitComm6StartMsg
	#define bitRxStart			bitRx6Start
	#define bitRxBusy			bitRx6Busy
	#define bitTxStart			bitTx6Start
	#define bitTxBusy			bitTx6Busy
	#include ot-comm-states.inc
#endif

#ifdef USE_CHANNEL_7
	#define COMM_CHANNEL		7
	;labels
	#define commState0			comm7State0
	#define commState1			comm7State1
	#define commState2			comm7State2
	#define commState3			comm7State3
	#define commState4			comm7State4
	#define commState5			comm7State5
	#define commState6			comm7State6
	#define commState7			comm7State7
	#define commNextState		comm7NextState
	#define commDone			comm7Done
	#define commIdSlaveFlags			comm7IdSlaveFlags
	#define commIdTempSetpoint			comm7IdTempSetpoint
	#define commIdRemoteOverwrite		comm7IdRemoteOverwrite
	#define commIdMaxRelMod				comm7IdMaxRelMod
	#define commIdMaxBoilerCapacity		comm7IdMaxBoilerCapacity
	#define commIdRoomSetpoint			comm7IdRoomSetpoint
	#define commIdRoomSetpointDiff		comm7IdRoomSetpointDiff
	#define commIdRelModLevel			comm7IdRelModLevel
	#define commIdWaterPressInCh		comm7IdWaterPressInCh
	#define commIdDayOfWeek				comm7IdDayOfWeek
	#define commIdRoomTemp				comm7IdRoomTemp
	#define commIdFlowWaterTemp			comm7IdFlowWaterTemp
	#define commIdDwhTemp				comm7IdDwhTemp
	#define commIdOutsideTemp			comm7IdOutsideTemp
	#define commIdReturnWaterTemp		comm7IdReturnWaterTemp
	#define commIdDhwSetpoint			comm7IdDhwSetpoint
	#define commIdMaxChWaterSetpoint	comm7IdMaxChWaterSetpoint
	#define commIdRemoteOverRide		comm7IdRemoteOverRide
	#define commStateCheckKnown			comm7StateCheckKnown
	#define commReadData				comm7ReadData
	#define commWriteData				comm7WriteData
	#define commWriteInvalid			comm7WriteInvalid
	#define commStateCheckWrongMsgtype	comm7StateCheckWrongMsgtype
	;data
	#define myCommState			myComm7State
	#define myCommTimer			myComm7Timer
	#define myRxBuffer			myRx7Buffer
	#define myTxBuffer			myTx7Buffer
	#define myCmdSetpoint		myCmd7Setpoint
	;bits
	#define bitCommBusy			bitComm7Busy
	#define bitCommStartMsg		bitComm7StartMsg
	#define bitRxStart			bitRx7Start
	#define bitRxBusy			bitRx7Busy
	#define bitTxStart			bitTx7Start
	#define bitTxBusy			bitTx7Busy
	#include ot-comm-states.inc
#endif
	return

