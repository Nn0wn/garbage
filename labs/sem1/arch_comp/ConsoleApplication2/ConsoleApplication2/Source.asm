.586p
.MODEL FLAT, C
.CODE

PUBLIC C SORT				;������������� ����� �� ����������
SORT PROC C Number:dword, NumRanDat:byte, CountNumN:dword, LGrInt:dword
PUSH EDI					;���������� ���������
PUSH ESI
PUSH EBP
MOV EDI, Number				;��������� ������ ������� ��������� �����
MOV EAX, LGrInt				;��������� ������� ������ ����������
MOV ESI, CountNumN			;��������� ������ ������� �������� �����
MOV ECX, DWORD PTR NumRanDat	;��������� ����� ������� ��������� �����
ADD EAX, 4						;������� �� ������� ���������
cycle:
MOV EBX,[EDI]					; ��������� �����
MOV EBP, [EAX]					;��������� ����� ������� ���������
CMP EBP, EBX					;���������� ����� � ��������
JGE nothing_new					;��������� �����
ADD EAX, 4						;������� � ��������� �������
ADD ESI, 4						;������� � �������� ��������� ���������
nothing_new:
MOV EDX, [ESI]					;��������� �������� ���������
INC EDX							;����������� ������� ���������
MOV [ESI], EDX					;������ ��� ������� � ������
ADD EDI,4					; ������� � ���� �����
LOOP cycle					; ��������� ���� �� ������ ���� ������					 
POP EBP						; �������������� �������� ���������
POP ESI
POP EDI
RET
SORT ENDP
END