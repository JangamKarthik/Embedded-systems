	AREA RESET,DATA,READONLY
	EXPORT __Vectors
__Vectors
	DCD 0x10004000
	DCD Reset_Handler
	ALIGN
	AREA mycode,CODE,READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler
	ldr r12,=arr
	ldr r11,=list
	mov r2,#10
up  ldr r10,[r12],#4
	str r10,[r11],#4
	subs r2,#1
	bne up

	ldr r0,=list
	mov r3,#0
outer_loop
	ldr r5,[r0,r3]
	mov r6,r3
	mov r10,r3
	add r6,#4
inner_loop
	ldr r7,[r0,r6]
	cmp r7,r5
	movls r10,r6
	movls r5,r7 
	add r6,#4
	cmp r6,#40
	bne inner_loop
	ldr r7,[r0,r3]
	str r7,[r0,r10]
	str r5,[r0,r3]
	add r3,#4
	cmp r3,#36
	bne outer_loop
	
stop b stop
arr dcd 0x0a,0x01,0x08,0x07,0x09,0x05,0x04,0x03,0x02,0x00
	AREA dataseg,DATA,READWRITE
list dcd 0x0
	end
