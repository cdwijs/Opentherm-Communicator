;*******************************************************************************
;    ot-comm.inc                                                               *
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
	;data
	#define myCommState			myComm0State
	#define myCommTimer			myComm0Timer
	#define myRxBuffer			myRx0Buffer
	#define myTxBuffer			myTx0Buffer
	;bits
	#define bitCommBusy			bitComm0Busy
	#define bitCommStartMsg		bitComm0StartMsg
	#define bitRxStart			bitRx0Start
	#define bitRxBusy			bitRx0Busy
	#define bitTxStart			bitTx0Start
	#define bitTxBusy			bitTx0Busy
	#include ot-comm-common.inc
#endif

#ifdef USE_CHANNEL_1
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
	;data
	#define myCommState			myComm1State
	#define myCommTimer			myComm1Timer
	#define myRxBuffer			myRx1Buffer
	#define myTxBuffer			myTx1Buffer
	;bits
	#define bitCommBusy			bitComm1Busy
	#define bitCommStartMsg		bitComm1StartMsg
	#define bitRxStart			bitRx1Start
	#define bitRxBusy			bitRx1Busy
	#define bitTxStart			bitTx1Start
	#define bitTxBusy			bitTx1Busy
	#include ot-comm-common.inc
#endif

#ifdef USE_CHANNEL_2
	;labels
	#define commState0			comm2State0
	#define commState1			comm2State1
	#define commState2			comm2State2
	#define commState3			comm2State3
	#define commState4			comm2State4
	#define commState5			comm2State5
	#define commState6			comm2State6
	#define commState7			comm2State7
	#define commNextState		comm2NextState
	#define commDone			comm2Done
	;data
	#define myCommState			myComm2State
	#define myCommTimer			myComm2Timer
	#define myRxBuffer			myRx2Buffer
	#define myTxBuffer			myTx2Buffer
	;bits
	#define bitCommBusy			bitComm2Busy
	#define bitCommStartMsg		bitComm2StartMsg
	#define bitRxStart			bitRx2Start
	#define bitRxBusy			bitRx2Busy
	#define bitTxStart			bitTx2Start
	#define bitTxBusy			bitTx2Busy
	#include ot-comm-common.inc
#endif

#ifdef USE_CHANNEL_3
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
	;data
	#define myCommState			myComm3State
	#define myCommTimer			myComm3Timer
	#define myRxBuffer			myRx3Buffer
	#define myTxBuffer			myTx3Buffer
	;bits
	#define bitCommBusy			bitComm3Busy
	#define bitCommStartMsg		bitComm3StartMsg
	#define bitRxStart			bitRx3Start
	#define bitRxBusy			bitRx3Busy
	#define bitTxStart			bitTx3Start
	#define bitTxBusy			bitTx3Busy
	#include ot-comm-common.inc
#endif
	return

