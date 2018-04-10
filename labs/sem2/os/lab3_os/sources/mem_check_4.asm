MEM_CHECK	SEGMENT
		ASSUME	CS:MEM_CHECK,	DS:MEM_CHECK,	ES:NOTHING,	SS:NOTHING
		ORG		100H
START:	JMP		BEGIN

AVL_MEM  	db  'Available memory:          Bytes;',0dh,0ah,'$'
EXT_MEM  	db  'Extended memory:           Kilobytes;',0dh,0ah,'$'
MCB_HEAD  	db  'List of MCB:',0dh,0ah,'$'
MCB_TYPE  	db  'Type of MCB:   h; $'
MCB_SEG  	db  'Adress of a segment:     h; $'
MCB_SIZE  	db  'MCB size:       b$'
MCB_TAIL  	db  ';               ',0dh,0ah,'$'
ERROR		db  'Memory overflow$'

; Вывод на экран
Write_msg	PROC	near
		mov	ah,09h
		int	21h
		ret
Write_msg	ENDP
; Функция перевода из 2 с/с в 16 половины байта
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
		push		cx
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
		push		bx
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
BYTE_TO_DEC		PROC	near
; перевод одного байта в 10 с/с, SI - адрес поля младшей цифры
		push		cx
		push		dx
		push		ax
		xor		ah,ah
		xor		dx,dx
		mov		cx,10
	loop_bd:
		div		cx
		or 		dl,30h
		mov 		[si],dl
		dec 		si
		xor		dx,dx
		cmp		ax,10
		jae		loop_bd
		cmp		ax,00h
		jbe		end_l
		or		al,30h
		mov		[si],al
	end_l:	
		pop		ax
		pop		dx
		pop		cx
		ret
BYTE_TO_DEC		ENDP
;---------------------------
WRD_TO_DEC		PROC	near
; перевод 2 байтов в 10 с/с, SI - адрес поля младшей цифры
		push		cx
		push		dx
		push		ax
		mov		cx,10
	wrd_loop_bd:
		div		cx
		or 		dl,30h
		mov 		[si],dl
		dec 		si
		xor		dx,dx
		cmp		ax,10
		jae		wrd_loop_bd
		cmp		ax,00h
		jbe		wrd_end_l
		or		al,30h
		mov		[si],al
	wrd_end_l:	
		pop		ax
		pop		dx
		pop		cx
		ret
WRD_TO_DEC		ENDP
; Определение доступной памяти
GET_AVL_MEM    	PROC    near
		push 	bx
		push 	cx
		push 	dx
		mov 		bx, 0ffffh
		mov 		ah, 4Ah
   		int 		21h
  		mov 		ax, bx
    		mov 		cx, 10h
  		mul 		cx
   		lea  		si, AVL_MEM+25
   		call 		WRD_TO_DEC
		lea		dx, AVL_MEM
		call 		Write_msg
		mov		bx, 4096h
    		mov		ah, 48h
		int		21h
		jnc		NOT_OF
		mov 		ax, 1
		jmp 		OF
	NOT_OF:
		sub		ax, ax
	OF:
		pop		dx
		pop		cx
		pop		bx
		ret
GET_AVL_MEM		ENDP
; Определение расширенной памяти
GET_EXT_MEM    PROC    near
		push 	ax
		push 	bx
		push 	si
		push 	dx
		mov		al, 30h
		out		70h, al 
		in		al, 71h
		mov		bl, al
		mov		al, 31h
		out		70h, al
		in		al, 71h
		mov 		ah, al
		mov 		al, bl
		sub 		dx, dx
		lea 		si, EXT_MEM+25
		call 		WRD_TO_DEC
		lea 		dx, EXT_MEM
		call 		write_msg
		pop		dx
		pop		si
		pop		bx
		pop		ax
		ret
GET_EXT_MEM    	ENDP

; Получаем восьмибайтное окончание блока
GET_TAIL			PROC	near
		push 	si
		push 	cx
		push 	bx
		push 	ax
		mov		bx,0008h
		mov		cx,4
	RE:
		mov		ax,es:[bx]
		mov		[si],ax
		add 		bx,2h
		add		si,2h
		loop 	RE
		pop		ax
    		pop		bx
    		pop		cx
		pop		si
		ret
GET_TAIL			ENDP
; Определяем и выводим цепочку блоков управления памятью
GET_MCB			PROC  near
		push 	ax
		push 	bx
		push 	cx
		push 	dx
		lea		dx, MCB_HEAD
		call 		Write_msg
		mov		ah,52h
		int		21h
		mov		es,es:[bx-2]
		mov		bx,1
	CIRCLE:
		sub		ax,ax
		sub		cx,cx
		sub		di,di
		sub		si,si
		mov		al,es:[0000h]
		call 		BYTE_TO_HEX
		lea		di,MCB_TYPE+13
		mov		[di],ax
		cmp		ax,4135h
		je 		DEC_BX
	STEP:
		lea		di,MCB_SEG+24
		mov		ax,es:[0001h]
		call 		WRD_TO_HEX
		mov		ax,es:[0003h]
		mov 		cx,10h
    		mul 		cx
    		lea		si,MCB_SIZE+15
		call 		WRD_TO_DEC
		lea		dx,MCB_TYPE
		call 		Write_msg
		lea		dx,MCB_SEG
		call 		Write_msg
		lea		dx,MCB_SIZE
		call 		Write_msg
		lea		si,MCB_Tail+2
		call 		GET_TAIl
		lea		dx,MCB_Tail
		call 		Write_msg
		cmp		bx,0
		jz		FINISH
		xor 		ax, ax
    		mov 		ax, es
    		add 		ax, es:[0003h]
    		inc 		ax
    		mov 		es, ax
		jmp 		CIRCLE
	FINISH:
		pop		dx
		pop		cx
		pop		bx
		pop		ax
		ret
	DEC_BX:
		mov		bx,0
		jmp 		STEP
GET_MCB			ENDP

BEGIN:
; Выполнение основного задания
		push		ax
		call 		GET_AVL_MEM
		cmp		ax, 1
		jne		CONT
		lea		dx,ERROR
		call		Write_msg
		jmp		PROG_END
	CONT:
		call 		GET_EXT_MEM
		call 		GET_MCB
		pop		ax
; выход в DOS
	PROG_END:
		xor		al,al
		mov		ah,4Ch
		int		21h
		ret
MEM_CHECK		ENDS
		END 		START