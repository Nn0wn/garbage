AStack    SEGMENT  STACK
          DW 12 DUP(?)
AStack    ENDS

DATA      SEGMENT

a     DW    0		; ������� a ������ �������	1)3,2,4,0 right 13 wrong 27
b     DW    0	; ������� b ������ �������		2)-3,-2,-4,-5 right -8 wrong -6
i     DW     0		; �������� �����
k     DW    0		; ������� ������ �������
temp 	DW	 0		; ������������� ��������
res   DW    0		; ���������

DATA      ENDS

CODE      SEGMENT
          ASSUME CS:CODE, DS:DATA, SS:AStack
          
Main      PROC  FAR
	push ds
	sub ax, ax
	push ax
	mov ax, DATA
	mov ds, ax
	
	mov ax, i		; ���������� 3i
	sal ax, 1
	add ax, i
	mov temp, ax

	mov ax, a
	cmp ax, b

	jg AgB		; ���� a > b
				; ���� a <= b
	mov ax, temp	; �������  i1
	add ax, 4

	mov bx, temp	; �������  i2
	add bx, 6

	jmp NextFunc
					
AgB: 				; ���� a > b
	mov ax, i		; �������  i1
	sal ax, 1
	neg ax
	add ax, 15
	
	mov bx, temp	; �������  i2
	sal bx, 1
	sub bx, 4
	;neg bx
NextFunc:
	cmp k, 0		
	jnz Min		; ���� k /= 0

	add ax, bx		; ���� k = 0
	neg ax		
	jl $-2		; ���� i1+i2 < 0	
	jmp Result
Min:				; ���� k /= 0
	cmp ax, bx
	jg Result
	;jle Result		; ���� i1 < i2
	mov ax, bx	; ���� i2 < i1
Result:			
	mov res, ax

_End:
	ret

Main      ENDP
CODE      ENDS
          END Main
