.586p
.MODEL FLAT, C
.DATA
offsetcountn	dd 0
.CODE

PUBLIC C SORTSTEP1				;распределение по интервалам единичной длины
SORTSTEP1 PROC C Number:dword, NumRanDat:byte, CountNumUnit1:dword, Xmin:byte
PUSH EDI					;сохранение регистров
PUSH ESI
MOV EDI, DWORD PTR Xmin			;получение значение xmin
MOV EAX, Number				;получение адреса массива случайных чисел
MOV ESI, CountNumUnit1			;получение адреса массива счетчика чисел
MOV ECX, DWORD PTR NumRanDat	;получение длины массива случайных чисел
cycle:
MOV EBX,[EAX]					; получение числа
SUB EBX,EDI	; вычесть xmin
MOV EDX,[ESI+4*EBX]				; получить счетчик этого числа
INC EDX	; прибавить 1
MOV [ESI+4*EBX],EDX				; занести в обратно массив
ADD EAX,4					; переход к след числу
LOOP cycle					; повторять пока не прошли весь массив
POP ESI					;восстановление значений регистров
POP EAX
RET
SORTSTEP1 ENDP

PUBLIC C SORTSTEP2				;распределение по интервалам произвольной длины
SORTSTEP2 PROC C CountNumUnit1:dword, lenUnit1:byte, LGrInt:dword, CountNumN:dword ,NInt:byte, Xmin:byte
PUSH EDI					;сохранение регистров
PUSH ESI
MOV EDI,DWORD PTR lenUnit1			;получение указателя на последний элемент массива счечика еденичной длины
DEC EDI
SHL EDI,2
ADD EDI,CountNumUnit1
MOV ECX,DWORD PTR lenUnit1			;поолучение счетчика
MOV EAX, DWORD PTR NInt			;получение указателей по последние элементы массивов
DEC EAX
SHL EAX,2
push edi
MOV ESI, LGrInt
MOV Edi, CountNumN
ADD Edi,EAX
ADD ESI,EAX
MOV offsetcountn,edi
pop edi					; опрерации перед выполнением цикла
SUB EAX,EAX
MOV EAX, dword PTR Xmin
ADD EAX, ECX
DEC EAX
MOV EBX,[ESI]
cycle:
CMP EAX,EBX
JL falseif
PUSH EAX
push esi
mov esi,offsetcountn
MOV EDX,[Esi]
MOV EAX,[EDI]
ADD EDX,EAX
MOV [Esi],EDX
pop esi
POP EAX
JMP endif1
falseif:
SUB ESI,4
SUB offsetcountn,4
MOV EBX,[ESI]
JMP cycle
endif1:
DEC EAX
SUB EDI,4
LOOP cycle					;повторять, пока не проверим весь массив
POP ESI					;восстановление значений регистров
POP EDI
RET
SORTSTEP2 ENDP
END
