;*******************************************************************************
;    timing.inc                                                                *
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
;use HFINTOSC (16MHz), no divide, no PLL
;use timer 2 to get interrupt every 100usec (10KHz)
;Fosc/4/Prescale/TMR2/Postscale
;prescale=4,PR2=100, postscale=1
;Fosc/4/4/100/1
;FOSC/1600 = 16MHz / 1600 = 10KHz

;Prescale for 10msec:
#define PRESCALE_1MS	d'10' 
#define PRESCALE_125MS	d'125'
#define PRESCALE_500MS	d'4'
#define PRESCALE_500US	d'5'

;OSCCON
;7	IDLEN	0	0=Device enters Sleep mode on SLEEP instruction
;6	IRCF2	1	111 =  HFINTOSC – (16 MHz)
;5	IRCF1	1
;4	IRCF0	1
;3	OSTS	0	0 = Device is running from the internal oscillator (HFINTOSC, MFINTOSC or LFINTOSC)
;2	HFIOFS	0	0 = HFINTOSC frequency is not stable
;1	SCS1	1	1x = Internal oscillator block
;0	SCS0	1
#define INIT_OSCCON	b'01110011'
;OSCCON2
;7	PLLRDY	0	0 = System clock comes from an oscillator, other than 4xPLL
;6	SOSCRUN	0	0 = System clock comes from an oscillator, other than SOSC
;5	-		0
;4	MFIOSEL	0	0 = MFINTOSC is not used
;3	SOSCGO	0	0 = Secondary oscillator is shut off if no other sources are requesting it.
;2	PRISD	0	0 = Oscillator drive circuit off (zero power)
;1	MFIOFS	0	0 = MFINTOSC is not stable
;0	LFIOFS	0	0 = LFINTOSC is not stable
#define INIT_OSCCON2 b'00000000'
;T2CON
;7	-			0
;6	T2OUTPS3	0	0000 = 1:1 Postscaler
;5	T2OUTPS2	0
;4	T2OUTPS1	0
;3	T2OUTPS0	0
;2	TMR2ON		1	1 = Timer2 is on		
;1	T2CKPS1		0	01 = Prescaler is 4
;0	T2CKPS0		1
#define INIT_T2CON b'00000101'
#define TMR2_INTERVAL d'100'

timingInit:
	movlw	INIT_OSCCON
	movwf	OSCCON
	movlw	INIT_OSCCON2
	movwf	OSCCON2
	movlw	INIT_T2CON
	movwf	T2CON
	movlw	TMR2_INTERVAL
	movwf	PR2
	movlw	PRESCALE_1MS
	movwf	myPrescale1msec
	movlw	PRESCALE_125MS
	movwf	myPrescale125msec		
	movlw	PRESCALE_500MS
	movwf	myPrescale500msec		
	return