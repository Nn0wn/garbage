AStack    SEGMENT  STACK
          DW 12 DUP(?)
AStack    ENDS

DATA      SEGMENT

a     DW    0		; Условие a первой функции	1)3,2,4,0 right 13 wrong 27
b     DW    0	; Условие b первой функции		2)-3,-2,-4,-5 right -8 wrong -6
i     DW     0		; Вводимое число
k     DW    0		; Условие второй функции
temp 	DW	 0		; Промежуточное значение
res   DW    0		; Результат

DATA      ENDS

CODE      SEGMENT
          ASSUME CS:CODE, DS:DATA, SS:AStack
          
Main      PROC  FAR
	push ds
	sub ax, ax
	push ax
	mov ax, DATA
	mov ds, ax
	
	mov ax, i		; подготовка 3i
	sal ax, 1
	add ax, i
	mov temp, ax

	mov ax, a
	cmp ax, b

	jg AgB		; если a > b
				; если a <= b
	mov ax, temp	; считаем  i1
	add ax, 4

	mov bx, temp	; считаем  i2
	add bx, 6

	jmp NextFunc
					
AgB: 				; если a > b
	mov ax, i		; считаем  i1
	sal ax, 1
	neg ax
	add ax, 15
	
	mov bx, temp	; считаем  i2
	sal bx, 1
	sub bx, 4
	;neg bx
NextFunc:
	cmp k, 0		
	jnz Min		; если k /= 0

	add ax, bx		; если k = 0
	neg ax		
	jl $-2		; если i1+i2 < 0	
	jmp Result
Min:				; если k /= 0
	cmp ax, bx
	jg Result
	;jle Result		; если i1 < i2
	mov ax, bx	; если i2 < i1
Result:			
	mov res, ax

_End:
	ret

Main      ENDP
CODE      ENDS
          END Main
