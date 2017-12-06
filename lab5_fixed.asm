AStack    SEGMENT  STACK
	DW 400 DUP(?)
AStack    ENDS

DATA      SEGMENT

	KEEP_CS DW 0			 ; ��� �������� ��������
	KEEP_IP DW 0			 ; � �������� ���������


DATA      ENDS

CODE      SEGMENT
          ASSUME CS:CODE, DS:DATA, SS:AStack
rout proc far    


	push ax			 ;��������� ��� ���������� ��������
	push dx			 ;��������� ��� ���������� ��������

; ����� ��������� � ����������
	mov ah,9h			 ;������� ��������� �������
	mov dx,offset message2	 ;� dx ��������� ����� ��������� Message2
	int 21h			 ;����� ������ �� �����

	pop dx			 ;��������������� ��������
	pop ax			 ;��������������� ��������

	mov al,20h		;pa�pe�e��� o�pa�o��� �pep��a��� c �o�ee ������� 
	out 20h,al		;�po�����, �e� �o���o ��o o�pa�o�a��oe(������ ��� 				;���������� ����������)
	
	iret				;����� ����������
	Message db 'Normal program cycle',10,13,'$'	 ;������ ��� ��������� ���������� ������
	Message2 db 'Interruption attention',10,13,'$'	 ;������ ��� ��������� ����������
rout endp			 ;����� ���������

main proc far
	push ds			;����������� ������ PSP �
	sub ax,ax			;������ ������� ������
	push ax
	mov ax,data
	mov ds,ax
	MOV  AH, 35H 		  ; ������� ��������� �������
	MOV  AL, 6H		  ; ����� �������
	INT  21H
	MOV  KEEP_IP, BX		  ; ����������� ��������
	MOV  KEEP_CS, ES		  ; � ��������
	push ds
	mov dx,offset rout		 ;�������� ��� ��������� � DX
	mov ax,seg rout 		 ;������� ���������
	mov ds,ax 			 ;�������� � ds
	mov ah,25h			 ;������� ��������� �������
	mov al,60h			 ;����� �������
	int 21h			 ;������ ����������
	;pop ds			 ;��������������� ds
	;push ds
	mov di, cs
	mov ds, di 
begin:
	mov bx, 0
begin_1:
	mov dx, offset message 	;������� ��������� ��������� ������ 
	mov ah, 9h			;���������
	int 21h
	add bx, 1
	cmp bx,3
	jle begin_1			;���� bx �� ������ ����

	int 60h			;������� ���� ����������
button:
	mov ah, 0
	int 16h			;�������� �������
	cmp ah, 01h
	je to_the_end		;���� ������ ������� Esc
	cmp ah, 1ch
	je begin			;���� ������ ������� Enter
	jmp button		;���� ������ ����� ������ �������
to_the_end:
	pop ds
	CLI					;����� ����� ����������
	PUSH DS			 	;���������� DS
	MOV DX,keep_ip		;���������� � ��������������
	MOV AX,keep_cs		;
	MOV DS,AX			;���������� � ��������������
	MOV AH,25h			;������� ��������� �������	
	MOV AL,60h			;����� �������
	INT 21h			 	;��������������� ������
	POP DS			 	;��������������� DS
	STI					;��������� ����� ����������

	ret 


Main      ENDP
CODE      ENDS
          END Main