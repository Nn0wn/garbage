TESTPC	SEGMENT
		ASSUME	CS:TESTPC,	DS:TESTPC,	ES:NOTHING,	SS:NOTHING
		ORG		100H
START:	JMP		BEGIN

; ДАННЫЕ
WR_MEM			db		'Address of the unavailable memory:     ',0dh,0ah,'$'
ENV_ADR			db		'Address of the environment:     ',0dh,0ah,'$'
TAIL				db		'Command line tail:',0dh,0ah,'$'
ENV_CONT		db		'Content of the environment: ' , '$'
PATH			db		'Load module path: ' , '$'
ENDL			db		0dh,0ah,'$'

NEW_LINE		PROC	near
		lea		dx,ENDL
		call		Write_msg
		ret
NEW_LINE		ENDP

Write_msg		PROC	near
		mov		ah,09h
		int		21h
		ret
Write_msg		ENDP

TETR_TO_HEX		PROC	near
		and		al,0fh
		cmp		al,09
		jbe		NEXT
		add		al,07
NEXT:	add		al,30h
		ret
TETR_TO_HEX		ENDP
;---------------------------
BYTE_TO_HEX		PROC near
; байт в AL переводится в два символа шестн. числа в AX
		push	cx
		mov		ah,al
		call		TETR_TO_HEX
		xchg		al,ah
		mov		cl,4
		shr		al,cl
		call		TETR_TO_HEX ; в AL старшая цифра
		pop		cx 			; в AH младшая
		ret
BYTE_TO_HEX		ENDP
;--------------------------
WRD_TO_HEX		PROC	near
; первод в 16 с/с 16-ти разрядного числа
; в AX - число, DI - адрес последнего символа
		push	bx
		mov		bh,ah
		call		BYTE_TO_HEX
		mov		[di],ah
		dec		di
		mov		[di],al
		dec		di
		mov		al,bh
		call		BYTE_TO_HEX
		mov		[di],ah
		dec		di
		mov		[di],al
		pop		bx
		ret
WRD_TO_HEX		ENDP
;----------------------------
; Определяем первый байт недоступной памяти
DEFINE_WR_MEM	PROC	near
		push		ax
		mov 		ax,es:[2]
		lea		di,WR_MEM
		add 		di,38
		call		WRD_TO_HEX
		pop		ax
		ret
DEFINE_WR_MEM	ENDP
;----------------------------
; Определяем сегментный адрес среды передаваемой программе
DEFINE_ENV_ADR	PROC	near
		push		ax
		mov 		ax,es:[2Ch]
		lea		di,ENV_ADR
		add 		di,31
		call		WRD_TO_HEX
		pop		ax
		ret
DEFINE_ENV_ADR	ENDP
;----------------------------
; Определяем хвост командной строки в символьном виде
DEFINE_TAIL		PROC	near
		push		ax
		push		cx
    		xor 		ax, ax
    		mov 		al, es:[80h]
    		add 		al, 81h
    		mov 		si, ax
    		push 	es:[si]
    		mov 	byte ptr es:[si+1], '$'
    		push 	ds
    		mov 		cx, es
    		mov 		ds, cx
    		mov 		dx, 81h
    		call		Write_msg
		cmp		al,81h
		je		SKIP
		call		NEW_LINE
	SKIP:
   	 	pop 		ds
    		pop 		es:[si]
    		pop		cx
    		pop		ax
		ret
DEFINE_TAIL		ENDP
;----------------------------
; Определяем содержимое области среды и путь к модулю
DEFINE_ENV_CONT	PROC	near
		push 	es 
		push		ax 
		push		bx
		push		cx 
		mov		bx,1
		mov		es,es:[2ch]
		mov		si,0
	NEW_VALUE:
		call		NEW_LINE
		mov		ax,si
	NULL_BYTE:
		cmp 	byte ptr es:[si], 0
		je 		NEXT2
		inc		si
		jmp 		NULL_BYTE
	NEXT2:
		push		es:[si]
		mov		byte ptr es:[si], '$' 
		push		ds
		mov		cx,es
		mov		ds,cx
		mov		dx,ax
		call		Write_msg
		pop		ds
		pop		es:[si]
		cmp		bx,0
		je 		PATH_ADDED
		inc		si
		cmp 	byte ptr es:[si], 01h
    		jne 		NEW_VALUE
    		call		NEW_LINE

    		lea		dx,PATH
    		call		Write_msg
    		dec		bx
    		add 		si,2
    		jmp 		NEW_VALUE
    	PATH_ADDED:
    		call		NEW_LINE
		pop		cx 
		pop		bx
		pop		ax
		pop		es
		ret
DEFINE_ENV_CONT	ENDP
;----------------------------
BEGIN:
		call		DEFINE_WR_MEM
		call		DEFINE_ENV_ADR
		lea		dx,WR_MEM 
		call		Write_msg
		call		NEW_LINE
		lea		dx,ENV_ADR
		call		Write_msg
		call		NEW_LINE 
		lea 		dx,TAIL   
    		call 		Write_msg
    		call		DEFINE_TAIL
    		call		NEW_LINE 
		lea		dx,ENV_CONT
		call		Write_msg
		call		DEFINE_ENV_CONT
		xor		al,al
		mov 		ah, 04Ch
		int 		21h
		ret
TESTPC	ENDS
		END 	START