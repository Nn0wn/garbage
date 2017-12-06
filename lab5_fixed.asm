AStack    SEGMENT  STACK
	DW 400 DUP(?)
AStack    ENDS

DATA      SEGMENT

	KEEP_CS DW 0			 ; для хранения сегмента
	KEEP_IP DW 0			 ; и смещения прерывани


DATA      ENDS

CODE      SEGMENT
          ASSUME CS:CODE, DS:DATA, SS:AStack
rout proc far    


	push ax			 ;сохраняем все изменяемые регистры
	push dx			 ;сохраняем все изменяемые регистры

; вывод сообщения о прерывании
	mov ah,9h			 ;функция установки вектора
	mov dx,offset message2	 ;в dx загружаем адрес сообщения Message2
	int 21h			 ;вывод строки на экран

	pop dx			 ;восстанавливаем регистры
	pop ax			 ;восстанавливаем регистры

	mov al,20h		;paзpeшeние oбpaбoтки пpepывaний c бoлee низкими 
	out 20h,al		;уpoвнями, чeм тoлькo чтo oбpaбoтaннoe(только для 				;аппаратных прерываний)
	
	iret				;конец прерывания
	Message db 'Normal program cycle',10,13,'$'	 ;строка для сообщения нормальной работы
	Message2 db 'Interruption attention',10,13,'$'	 ;строка для сообщения прерывания
rout endp			 ;конец процедуры

main proc far
	push ds			;запоминание адреса PSP и
	sub ax,ax			;номера команды выхода
	push ax
	mov ax,data
	mov ds,ax
	MOV  AH, 35H 		  ; функция получения вектора
	MOV  AL, 6H		  ; номер вектора
	INT  21H
	MOV  KEEP_IP, BX		  ; запоминание смещения
	MOV  KEEP_CS, ES		  ; и сегмента
	push ds
	mov dx,offset rout		 ;смещение для процедуры в DX
	mov ax,seg rout 		 ;сегмент процедуры
	mov ds,ax 			 ;помещаем в ds
	mov ah,25h			 ;функция установки вектора
	mov al,60h			 ;номер вектора
	int 21h			 ;меняем прерывание
	;pop ds			 ;восстанавливаем ds
	;push ds
	mov di, cs
	mov ds, di 
begin:
	mov bx, 0
begin_1:
	mov dx, offset message 	;выводим сообщение номальной работы 
	mov ah, 9h			;программы
	int 21h
	add bx, 1
	cmp bx,3
	jle begin_1			;пока bx не больше трех

	int 60h			;выводим наше прерывание
button:
	mov ah, 0
	int 16h			;ожидание клавиши
	cmp ah, 01h
	je to_the_end		;если нажата клавиша Esc
	cmp ah, 1ch
	je begin			;если нажата клавиша Enter
	jmp button		;если нажата любая другая клавиша
to_the_end:
	pop ds
	CLI					;сброс флага прерываний
	PUSH DS			 	;сохранение DS
	MOV DX,keep_ip		;подготовка к восстановлению
	MOV AX,keep_cs		;
	MOV DS,AX			;подготовка к восстановлению
	MOV AH,25h			;функция установки вектора	
	MOV AL,60h			;номер вектора
	INT 21h			 	;восстанавливаем вектор
	POP DS			 	;восстанавливаем DS
	STI					;установка флага прерываний

	ret 


Main      ENDP
CODE      ENDS
          END Main