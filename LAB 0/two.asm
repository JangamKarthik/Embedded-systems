	AREA RESET,DATA,READONLY
	EXPORT __Vectors
__Vectors
	DCD 0x40001000
	DCD Reset_Handler
	ALIGN
	AREA mycode,CODE,READONLY
ENTRY
	EXPORT Reset_Handler
Reset_Handler

	mov r1,#0x22
	mov r2,#33
	mov r3,#2_1010
	end