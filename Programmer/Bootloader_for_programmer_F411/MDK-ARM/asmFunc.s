	AREA    |.text|, CODE, READONLY
branth
	EXPORT  branth
	LDR	R1, [R0]	
	MSR	MSP, R1
	LDR	R1, [R0, #4]
	BX R1
	;MSR	r15, R1
	
	END
		
		
