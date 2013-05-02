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
;       |             |
; +---->|BitCommBusy=0 |
; |     +------+------+
; |            |BitStartMsg
; |     +------v------+
; |     |StartA       |
; |     |-------------|
; |     |BitCommBusy=1|
; |     |BitTxStart=1 |
; |     |BitRxStart=1 |
; |     +------+------+
; |            |BitRxBusy
; |     +------v------+
; |     |StartB       |
; |     |-------------|
; |     |BitRxStart=0 |
; |     |             +
; |     +------+------+
; |            |!BitTxBusy
; |     +------v------+      +---------------+
; |     | Wait Answer |      |Timeout        |
; |     |-------------|      |---------------|
; |     |BitTxStart=0 |1 sec | BitRxAbort=1  |
; |     |             |------>               |
; |     +------+------+      +-------+-------+
; |            |!BitRxBusy           |!BitRxBusy
; |     +------v------+      +-------v-------+
; |     |CheckMsg     |      |Aborted        |
; |     |-------------|      |---------------|
; |     |             |      | BitRxAbort=0  |
; |     |             |      |               |
; |     +------+------+      |---------------|
; |            |<-------------------+
; |     +------v------+
; |     |ReportMsg    |
; |     |-------------|
; |     |Msg->rs232   |
; |     |             |
; |     +------+------+
; |            |
; +<-----------+ 
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

