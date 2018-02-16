TESTPC	SEGMENT
		ASSUME	CS:TESTPC,	DS:TESTPC,	ES:NOTHING,	SS:NOTHING
		ORG		100H
START:	JMP		BEGIN
; ƒјЌЌџ≈
temp				dw		-1
PC_TYPE			db		'PC Type: ', '$', '  ','$'
SYSTEM_VERSION	db		0DH,0AH,'System version:  . ',0DH,0AH,'$'
OEM_NUMBER		db		'OEM number:      ',0DH,0AH,'$'
SERIAL_NUMBER	db		'User serial number:               ',0DH,0AH,'$'
PC_TYPE_NUMBERS	db		255, 254, 251, 252, 250, 252, 248, 253, 249
PC_TYPE_NAMES	db		'PC                    ', '$' ,'PC/XT                 ', '$' ,'PC/XT                 ', '$', 'AT or PS2 model 50(60)', '$' ,'PS2 model 30             ', '$' ,'PS2 model 80          ', '$' ,'PCjr                  ', '$' ,'PC Convertible        ', '$'
; ѕ–ќ÷≈ƒ”–џ
;-----------------------------------------------------------
Write_msg		PROC	near
		mov		ah,09h
		int		21h
		ret
Write_msg		ENDP
TETR_TO_HEX		PROC	near
		and		al,0Fh
		cmp		al,09
		jbe		NEXT
		add		al,07
NEXT:	add		al,30h
		ret
TETR_TO_HEX		ENDP
;---------------------------
BYTE_TO_HEX		PROC near
; байт в AL переводитс€ в два символа шестн. числа в AX
		push		cx
		mov		ah,al
		call		TETR_TO_HEX
		xchg		al,ah
		mov		cl,4
		shr		al,cl
		call		TETR_TO_HEX ; в AL старша€ цифра
		pop		cx 			; в AH младша€
		ret
BYTE_TO_HEX		ENDP
;--------------------------
WRD_TO_HEX		PROC	near
; первод в 16 с/с 16-ти разр€дного числа
; в AX - число, DI - адрес последнего символа
		push		bx
		mov		bh,ah
		call		BYTE_TO_HEX
		mov		[di],ah
		dec		di
		mov		[di],al
		dec		di
		mov		al,bh
		xor		ah,ah
		call		BYTE_TO_HEX
		mov		[di],ah
		dec		di
		mov		[di],al
		pop		bx
		ret
WRD_TO_HEX		ENDP
;----------------------------
BYTE_TO_DEC		PROC	near
; перевод в 10 с/с, SI - адрес пол€ младшей цифры
		push		cx
		push		dx
		push		ax
		xor		ah,ah
		xor		dx,dx
		mov		cx,10
loop_bd:	div		cx
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
end_l:	pop		ax
		pop		dx
		pop		cx
		ret
BYTE_TO_DEC		ENDP
;---------------------------
GET_PC_TYPE 	PROC	near
; ‘ункци€ определ€юща€ тип PC
		push 	es
		push		bx
		push		ax
		push		si
		push 	di
		push		dx
		mov		bx,0F000h
		mov 		es,bx
		mov 		ax,es:[0FFFEh]
		mov 		di,-23
		mov		si,-1
again:	inc		si
		cmp		si,8
		jge		b_to_h
		add		di,23
		mov		bx,offset PC_TYPE_NUMBERS
		mov		dx,[bx][si]
		cmp		dl,al
		jne		again
		mov		temp,di
		jmp		string
b_to_h:	call 		BYTE_TO_HEX
		lea		bx,PC_TYPE
		mov		[bx+9],ax
string:	pop		dx
		pop		di
		pop		si
		pop		ax
		pop		bx
		pop		es
		ret
GET_PC_TYPE		ENDP
;----------------------------
GET_SYS_VER		PROC	near
; ‘ункци€ определ€юща€ версию системы
		push		ax
		push 	si
		lea		si,SYSTEM_VERSION
		add		si,18
		call		BYTE_TO_DEC
		add		si,3
		mov 		al,ah
		call		BYTE_TO_DEC
		pop 		si
		pop 		ax
		ret
GET_SYS_VER		ENDP
;---------------------------
GET_OEM_NUM		PROC	near
; функци€ определ€юща€ OEM
		push		ax
		push		bx
		push		si
		mov 		al,bh
		lea		si,OEM_NUMBER
		add		si,12
		call		BYTE_TO_HEX
		mov		[si],ax
		pop		si
		pop		bx
		pop		ax
		ret
GET_OEM_NUM		ENDP
;--------------------------
GET_SERIAL_NUM	PROC	near
		push		ax
		push		bx
		push		cx
		push		si
		push		di
		mov		al,bl
		call		BYTE_TO_HEX
		lea		di,SERIAL_NUMBER
		add		di,20
		mov		[di],AX
		mov		ax,cx
		lea		di,SERIAL_NUMBER
		add		di,25
		call		WRD_TO_HEX
		pop		di
		pop		si
		pop		cx
		pop		bx
		pop		ax
		ret
GET_SERIAL_NUM	ENDP
BEGIN:
; определ€ем нужную информацию
		call 		GET_PC_TYPE
		mov		ah,30h
		int		21h
		call		GET_SYS_VER
		call		GET_OEM_NUM
		call		GET_SERIAL_NUM
; выводим информацию
		push		di
		mov		di,temp
		lea		dx,PC_TYPE
		call		Write_msg
		cmp		di,-1
		je		no_name
		lea		dx,PC_TYPE_NAMES[di]
		call 		Write_msg
no_name:	lea		dx,SYSTEM_VERSION
		call		Write_msg
		lea		dx,OEM_NUMBER
		call		Write_msg
		lea		dx,SERIAL_NUMBER
		call		Write_msg
		pop		di
; выход в DOS
		xor		al,al
		mov		ah,3Ch
		int		21h
		ret
TESTPC	ENDS
		END 	START