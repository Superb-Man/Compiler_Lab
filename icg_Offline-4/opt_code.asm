.MODEL SMALL
.STACK 100h
.DATA

	arr DW 12 DUP(0)

.CODE
	main PROC
		MOV AX, @DATA
		mov DS, AX
		MOV BP, SP
		PUSH 11
		POP AX
		SHL AX, 1
		LEA BX, arr
		SUB BX, AX
		PUSH BX
		PUSH 10
		POP AX
		POP BX
		MOV [BX], AX
		PUSH 10
		POP AX
		SHL AX, 1
		LEA BX, arr
		SUB BX, AX
		PUSH BX
		PUSH 11
		POP AX
		POP BX
		MOV [BX], AX
		PUSH 10
		POP AX
		SHL AX, 1
		LEA BX, arr
		SUB BX, AX
		PUSH BX
		MOV BX, [BX]
		CALL PRINT_NUMBER
		PUSH 11
		POP AX
		SHL AX, 1
		LEA BX, arr
		SUB BX, AX
		PUSH BX
		MOV BX, [BX]
		CALL PRINT_NUMBER

		L0:
		MOV AH, 4CH
		INT 21H
	main ENDP

	PRINT_NEWLINE PROC
		PUSH AX
		PUSH DX
		MOV AH, 2
		MOV DL, 0Dh
		INT 21h
		MOV DL, 0Ah
		INT 21h
		POP DX
		POP AX
		RET
	PRINT_NEWLINE ENDP

PRINT_NUMBER PROC
	PUSH CX
	MOV AX, 'X'
	PUSH AX
	CMP BX, 0
	JE ZERO_NUM
	JNL NON_NEGATIVE
	NEG BX
	MOV DL, '-'
	MOV AH, 2
	INT 21H
	JMP NON_NEGATIVE
	ZERO_NUM:
		MOV DX, 0 
		PUSH DX
		JMP POP_PRINT_LOOP
	NON_NEGATIVE:
	MOV CX, 10
	MOV AX, BX
	PRINT_LOOP:
		CMP AX, 0
		JE END_PRINT_LOOP
		MOV DX, 0
		DIV CX
		PUSH DX
		JMP PRINT_LOOP
	END_PRINT_LOOP:
	POP_PRINT_LOOP:
		POP DX
		CMP DX, 'X'
		JE END_POP_PRINT_LOOP
		CMP DX, '-'
		JE PRINT_TO_CONSOLE
		ADD DX, 30H
		PRINT_TO_CONSOLE:
		MOV AH, 2
		INT 21H
		JMP POP_PRINT_LOOP
	END_POP_PRINT_LOOP:
CALL PRINT_NEWLINE
	POP CX
	RET
PRINT_NUMBER ENDP

END MAIN
