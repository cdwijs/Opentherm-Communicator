;*******************************************************************************
;    config.inc                                                                *
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
#define Carry	STATUS,C
#define Zero	STATUS,Z
#define BANK_RAM_0	d'0'
#define BANK_SFR	d'15'
;*** P18F2x4xK22 ***
#ifdef  __18F23K22
	#include P18F23K22.INC
	#define P18F2x4xK22
#endif

#ifdef __18F24K22	
	#include P18F24K22.INC
	#define P18F2x4xK22
#endif

#ifdef __18F25K22	
	#include P18F25K22.INC
	#define P18F2x4xK22
#endif

#ifdef __18F26K22	
	#include P18F26K22.INC
	#define P18F2x4xK22
#endif

#ifdef  __18F43K22
	#include P18F43K22.INC
	#define P18F2x4xK22
#endif

#ifdef __18F44K22	
	#include P18F44K22.INC
	#define P18F2x4xK22
#endif

#ifdef __18F45K22	
	#include P18F45K22.INC
	#define P18F2x4xK22
#endif

#ifdef __18F46K22	
	#include P18F46K22.INC
	#define P18F2x4xK22
#endif

#ifdef P18F2x4xK22
	CONFIG	FOSC = INTIO67		;Internal oscillator block
	CONFIG	PLLCFG = OFF		;Oscillator used directly
	CONFIG	PRICLKEN = ON		;Primary clock is always enabled
	CONFIG	FCMEN = OFF			;Fail-Safe Clock Monitor disabled
	CONFIG	IESO = OFF			;Oscillator Switchover mode disabled
	CONFIG	PWRTEN = OFF		;Power up timer disabled
	CONFIG	BOREN = SBORDIS		;Brown-out Reset enabled in hardware only (SBOREN is disabled)
	CONFIG	BORV = 285			;VBOR set to 2.85 V nominal
	#ifdef 	__DEBUG
		CONFIG	WDTEN = OFF		;Watch dog timer is always disabled. SWDTEN has no effect.
	#else
		CONFIG	WDTEN = ON		;WDT is always enabled. SWDTEN bit has no effect
	#endif
	CONFIG	WDTPS = 32768		;1:32768
	CONFIG	CCP2MX = PORTB3		;CCP2 input/output is multiplexed with RB3
	CONFIG	PBADEN = OFF		;PORTB<5:0> pins are configured as digital I/O on Reset
	CONFIG	CCP3MX = PORTE0		;P3A/CCP3 input/output is mulitplexed with RE0
	CONFIG	HFOFST = ON			;HFINTOSC output and ready status are not delayed by the oscillator stable status
	CONFIG	T3CMX = PORTB5		;T3CKI is on RB5
	CONFIG	P2BMX = PORTC0		;P2B is on RC0
	CONFIG	MCLRE = EXTMCLR		;MCLR pin enabled, RE3 input pin disabled
	CONFIG	STVREN = ON			;Stack full/underflow will cause Reset
	#ifdef __DEBUG	
		CONFIG	LVP = OFF       ;Single-Supply ICSP disabled
	#else
		CONFIG	LVP = ON		;Single-Supply ICSP enabled if MCLRE is also 1
	#endif
	CONFIG	XINST = OFF			;Instruction set extension and Indexed Addressing mode disabled (Legacy mode)
	#ifdef __DEBUG
		CONFIG	DEBUG = ON		;Enabled
	#else
		CONFIG	DEBUG = OFF		;Disabled
	#endif	
	CONFIG	CP0 = OFF			;Block 0 (000200-000FFFh) not code-protected
	CONFIG	CP1 = OFF			;Block 1 (001000-001FFFh) not code-protected
	CONFIG	CPB = OFF			;Boot block (000000-0001FFh) not code-protected
	CONFIG	CPD = OFF			;Data EEPROM not code-protected
	CONFIG	WRT0 = OFF			;Block 0 (000200-000FFFh) not write-protected
	CONFIG	WRT1 = OFF			;Block 1 (001000-001FFFh) not write-protected
	CONFIG	WRTC = OFF			;Configuration registers (300000-3000FFh) not write-protected
	CONFIG	WRTB = OFF			;Boot Block (000000-0001FFh) not write-protected
	CONFIG	WRTD = OFF			;Data EEPROM not write-protected
	CONFIG	EBTR0 = OFF			;Block 0 (000200-000FFFh) not protected from table reads executed in other blocks
	CONFIG	EBTR1 = OFF			;Block 1 (001000-001FFFh) not protected from table reads executed in other blocks
	CONFIG	EBTRB = OFF			;Boot Block (000000-0001FFh) not protected from table reads executed in other blocks
#endif
;*** P18F2455/2550/4455/4550 ***

#ifdef __18F4550
	#include P18F4550.INC
	#define P18F2455_2550_4455_4550

	CONFIG	PLLDIV = 1			;No prescale (4 MHz oscillator input drives PLL directly)
	CONFIG	CPUDIV = OSC1_PLL2	;[Primary Oscillator Src: /1][96 MHz PLL Src: /2]
	CONFIG	USBDIV = 1			;USB clock source comes directly from the primary oscillator block with no postscale
	CONFIG	FOSC = INTOSC_HS	;Internal oscillator, HS oscillator used by USB (INTHS)
	CONFIG	FCMEN = OFF			;Fail-Safe Clock Monitor disabled
	CONFIG	IESO = OFF			;Oscillator Switchover mode disabled
	CONFIG	PWRT = OFF			;PWRT disabled
	CONFIG	BOR = ON			;Brown-out Reset enabled in hardware only (SBOREN is disabled)
	CONFIG	BORV = 3			;Minimum setting
	CONFIG	VREGEN = OFF		;USB voltage regulator disabled
#ifdef __DEBUG
	CONFIG	WDT = OFF			;WDT disabled (control is placed on the SWDTEN bit)
#else
	CONFIG	WDT = ON			;WDT enabled
#endif
	CONFIG	WDTPS = 32768		;1:32768
	CONFIG	CCP2MX = ON			;CCP2 input/output is multiplexed with RC1
	CONFIG	PBADEN = OFF		;PORTB<4:0> pins are configured as digital I/O on Reset
	CONFIG	LPT1OSC = OFF		;Timer1 configured for higher power operation
	CONFIG	MCLRE = ON			;MCLR pin enabled; RE3 input pin disabled
	CONFIG	STVREN = ON			;Stack full/underflow will cause Reset
#ifdef __DEBUG
	CONFIG	LVP = OFF			;Single-Supply ICSP disabled
#else
	CONFIG	LVP = ON			;Single-Supply ICSP enabled
#endif
	CONFIG	ICPRT = OFF			;ICPORT disabled
	CONFIG	XINST = OFF			;Instruction set extension and Indexed Addressing mode disabled (Legacy mode)
#ifdef __DEBUG
	CONFIG	DEBUG = ON			;Background debugger enabled, RB6 and RB7 are dedicated to In-Circuit Debug
#else
	CONFIG	DEBUG = OFF			;Background debugger disabled, RB6 and RB7 configured as general purpose I/O pins
#endif
	CONFIG	CP0 = OFF			;Block 0 (000800-001FFFh) is not code-protected
	CONFIG	CP1 = OFF			;Block 1 (002000-003FFFh) is not code-protected
	CONFIG	CP2 = OFF			;Block 2 (004000-005FFFh) is not code-protected
	CONFIG	CP3 = OFF			;Block 3 (006000-007FFFh) is not code-protected
	CONFIG	CPB = OFF			;Boot block (000000-0007FFh) is not code-protected
	CONFIG	CPD = OFF			;Data EEPROM is not code-protected
	CONFIG	WRT0 = OFF			;Block 0 (000800-001FFFh) is not write-protected
	CONFIG	WRT1 = OFF			;Block 1 (002000-003FFFh) is not write-protected
	CONFIG	WRT2 = OFF			;Block 2 (004000-005FFFh) is not write-protected
	CONFIG	WRT3 = OFF			;Block 3 (006000-007FFFh) is not write-protected
	CONFIG	WRTC = OFF			;Configuration registers (300000-3000FFh) are not write-protected
	CONFIG	WRTB = OFF			;Boot block (000000-0007FFh) is not write-protected
	CONFIG	WRTD = OFF			;Data EEPROM is not write-protected
	CONFIG  EBTR0 = OFF			;Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks	
	CONFIG	EBTR1 = OFF			;Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks
	CONFIG	EBTR2 = OFF			;Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks
	CONFIG	EBTR3 = OFF			;Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks
	CONFIG	EBTRB = OFF			;Boot block (000000-0007FFh) is not protected from table reads executed in other blocks
#endif

