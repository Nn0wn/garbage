.586p
.MODEL FLAT, C
.CODE

PUBLIC C SORT				;распределение чисел по интервалам
SORT PROC C Number:dword, NumRanDat:byte, CountNumN:dword, LGrInt:dword
PUSH EDI					;сохранение регистров
PUSH ESI
PUSH EBP
MOV EDI, Number				;получение адреса массива случайных чисел
MOV EAX, LGrInt				;получение массива границ интервалов
MOV ESI, CountNumN			;получение адреса массива счетчика чисел
MOV ECX, DWORD PTR NumRanDat	;получение длины массива случайных чисел
ADD EAX, 4						;переход ко второму интервалу
cycle:
MOV EBX,[EDI]					; получение числа
MOV EBP, [EAX]					;получение левой границы интервала
CMP EBP, EBX					;сравниваем число с границей
JGE nothing_new					;обработка числа
ADD EAX, 4						;переход к следующей границе
ADD ESI, 4						;переход к счетчику слеующего интервала
nothing_new:
MOV EDX, [ESI]					;получение счетчика интервала
INC EDX							;увеличиваем счетчик интервала
MOV [ESI], EDX					;кладем его обратно в массив
ADD EDI,4					; переход к след числу
LOOP cycle					; повторять пока не прошли весь массив					 
POP EBP						; восстановление значений регистров
POP ESI
POP EDI
RET
SORT ENDP
END