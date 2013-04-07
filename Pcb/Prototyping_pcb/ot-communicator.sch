EESchema Schematic File Version 2  date 3/16/2013 10:51:54 AM
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 8
Title "noname.sch"
Date "16 mar 2013"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 3350 1800 600  400 
U 513DBB79
F0 "frontend2" 50
F1 "ot-frontend.sch" 50
F2 "in" I R 3950 1900 60 
F3 "out" I R 3950 2050 60 
F4 "5V" I L 3350 1850 60 
F5 "GND" I L 3350 2050 60 
F6 "24V" I L 3350 1950 60 
$EndSheet
$Sheet
S 3350 2350 600  400 
U 513DD90A
F0 "frontend3" 50
F1 "ot-frontend.sch" 50
F2 "in" I R 3950 2450 60 
F3 "out" I R 3950 2600 60 
F4 "5V" I L 3350 2400 60 
F5 "GND" I L 3350 2600 60 
F6 "24V" I L 3350 2500 60 
$EndSheet
$Sheet
S 3350 2900 600  400 
U 513DE799
F0 "frontend4" 50
F1 "ot-frontend.sch" 50
F2 "in" I R 3950 3000 60 
F3 "out" I R 3950 3150 60 
F4 "5V" I L 3350 2950 60 
F5 "GND" I L 3350 3150 60 
F6 "24V" I L 3350 3050 60 
$EndSheet
$Sheet
S 3350 3450 600  400 
U 513DE7A0
F0 "frontend5" 50
F1 "ot-frontend.sch" 50
F2 "in" I R 3950 3550 60 
F3 "out" I R 3950 3700 60 
F4 "5V" I L 3350 3500 60 
F5 "GND" I L 3350 3700 60 
F6 "24V" I L 3350 3600 60 
$EndSheet
$Sheet
S 3350 4000 600  400 
U 513E59F7
F0 "frontend6" 50
F1 "ot-frontend.sch" 50
F2 "in" I R 3950 4100 60 
F3 "out" I R 3950 4250 60 
F4 "5V" I L 3350 4050 60 
F5 "GND" I L 3350 4250 60 
F6 "24V" I L 3350 4150 60 
$EndSheet
$Sheet
S 3350 4550 600  400 
U 513F0C73
F0 "frontend7" 50
F1 "ot-frontend.sch" 50
F2 "in" I R 3950 4650 60 
F3 "out" I R 3950 4800 60 
F4 "5V" I L 3350 4600 60 
F5 "GND" I L 3350 4800 60 
F6 "24V" I L 3350 4700 60 
$EndSheet
$Sheet
S 3350 5100 600  400 
U 513F9436
F0 "frontend8" 50
F1 "ot-frontend.sch" 50
F2 "in" I R 3950 5200 60 
F3 "out" I R 3950 5350 60 
F4 "5V" I L 3350 5150 60 
F5 "GND" I L 3350 5350 60 
F6 "24V" I L 3350 5250 60 
$EndSheet
Wire Wire Line
	3350 2050 3200 2050
Wire Wire Line
	3200 2050 3200 5350
Wire Wire Line
	3200 2600 3350 2600
Wire Wire Line
	3200 3150 3350 3150
Connection ~ 3200 2600
Wire Wire Line
	3200 3700 3350 3700
Connection ~ 3200 3150
Wire Wire Line
	3350 1950 3100 1950
Wire Wire Line
	3100 1950 3100 5250
Wire Wire Line
	3100 2500 3350 2500
Wire Wire Line
	3100 3050 3350 3050
Connection ~ 3100 2500
Wire Wire Line
	3100 3600 3350 3600
Connection ~ 3100 3050
Wire Wire Line
	3350 1850 3150 1850
Wire Wire Line
	3150 2400 3350 2400
Wire Wire Line
	3150 2950 3350 2950
Connection ~ 3150 2400
Wire Wire Line
	3150 3500 3350 3500
Connection ~ 3150 2950
Connection ~ 3200 2050
Connection ~ 3150 1850
Connection ~ 3100 1950
Wire Wire Line
	3200 4250 3350 4250
Connection ~ 3200 3700
Wire Wire Line
	3150 4050 3350 4050
Connection ~ 3150 3500
Wire Wire Line
	3100 4150 3350 4150
Connection ~ 3100 3600
Wire Wire Line
	3200 4800 3350 4800
Connection ~ 3200 4250
Wire Wire Line
	3100 4700 3350 4700
Connection ~ 3100 4150
Wire Wire Line
	3150 4600 3350 4600
Connection ~ 3150 4050
Wire Wire Line
	3200 5350 3350 5350
Connection ~ 3200 4800
Wire Wire Line
	3100 5250 3350 5250
Connection ~ 3100 4700
Wire Wire Line
	3150 5150 3350 5150
Connection ~ 3150 4600
Wire Wire Line
	3150 1850 3150 5150
$EndSCHEMATC
