;*******************************************************************************
;    ot-comm.inc                                                            *
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
; +---->|BitMsgBusy=0 |
; |     +------+------+
; |            |BitStartMsg
; |     +------v------+
; |     |             |
; |     |-------------|
; |     |BitStartTx=1 |
; |     |BitStartRx=1 |
; |     |BitMsgBusy=1 |
; |     +------+------+
; |            |BitRxBusy
; |     +------v------+
; |     |             |
; |     |-------------|
; |     |BitStartRx=0 |
; |     |             +
; |     +------+------+
; |            |!BitTxBusy
; |     +------v------+     +---------------+
; |     |             |     |Timeout        |
; |     |-------------|     |---------------|
; |     |BitStartTx=0 |1 sec|               |
; |     |             |----->               |
; |     +------+------+     +-------+-------+
; |            |!BitRxBusy          |
; |     +------v------+             |
; |     |CheckMsg     |             |
; |     |-------------|             |
; |     |             |             |
; |     |             |             |
; |     +------+------+             |
; |            |<-------------------+
; |     +------v------+
; |     |ReportMsg    |
; |     |-------------|
; |     |Msg->rs232   |
; |     |             |
; |     +------+------+
; |            |
; +<-----------+ 