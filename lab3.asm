AStack    SEGMENT  STACK
          DW 12 DUP(?)
AStack    ENDS

DATA      SEGMENT

a     DW    0		; �������� ��� ������� f1 � f2
b     DW    0		; -/-
i     DW     0		; �������� �����
k     DW    0		; �������� ��� �������������� ����������
res   DW    0		; ������������� ���������

DATA      ENDS

CODE      SEGMENT
          ASSUME CS:CODE, DS:DATA, SS:AStack
          
Main      PROC  FAR
	push ds
	sub ax, ax
	push ax
	mov ax, DATA
	mov ds, ax
	
	mov ax, a
	cmp ax, b

	jg AgB		; ���� a <= b

	mov ax, i		; �������  i1
	sal ax, 1
	add ax, i
	add ax, 4

	mov bx, i		; �������  i2
	add bx, 2
	sal bx, 1
	add bx, i
	add bx, 2

	jmp NextFunc
					
AgB: 				; ���� a > b
	mov ax, i		;�������  i1
	sal ax, 1
	neg ax
	add ax, 15
	
	mov bx, i		; �������  i2
	sal bx, 1
	sal bx, 1
	add bx, i
	add bx, i
	sub bx, 4
	neg bx
NextFunc:
	cmp k, 0		

	jnz Choice		; ���� k = 0

	add ax, bx		
	cmp ax, 0
	jge min		; ���� i1+i2 >= 0
	neg ax		; ���� i1+i2 < 0
	jmp min
Choice:			; ���� k /= 0
	cmp ax, bx
	jle min		; ���� i1<i2
	mov ax, bx	; ���� i2<i1
min:			
	mov res, ax

_End:
	ret

Main      ENDP
CODE      ENDS
          END Main
