SwitchCase2:
	andlw	h'01'
SwitchCase4:
	andlw	h'03'
Switchcase8:
	andlw	h'07'
Switchcase16:
	andlw	h'0F'
	rlncf   WREG ;cdw, does not work when access or banked is specified?
	addwf   TOSL
	movlw   0
	addwfc  TOSH
	addwfc  TOSU            ; for >64K bytes memory
	return



ascii2nibble
	;wreg holds the ascii char	
#define asciichar	reg0
#define result		reg1
	movwf	asciichar,BANKED
	movlw	'0'
	subwf	asciichar,w,BANKED 	;wreg now holds ascii - 0x30
	movwf	result		
	sublw	d'9' 		;carry set when ascii '0'..'9'
	bc		between0and9
	movlw	h'27'
	subwf	result,f,BANKED
between0and9:		
	movf	result,w,BANKED
	andlw	h'0f'	;output in lower nibble of wreg
#undefine asciichar
#undefine result
	return