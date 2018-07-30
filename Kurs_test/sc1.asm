
; Hacker Disassembler Engine 32
; Copyright (c) 2006-2009, Vyacheslav Patkov.
; All rights reserved.

cpu 386
bits 32

%macro globals 1
  global %1
  global _%1
  %1:
  _%1:
%endmacro


globals main
push ebp
mov ebp,esp
sub esp,16
;===============================
mov dword [ebp-4],1 ;i=0
.fori:
	mov dword [ebp-8],1 ;j=0
	.forj:
		mov dword eax,[ebp-8] ;j->eax
		add eax,eax ;j=j+j
		add eax,eax ;j=j+j
		add dword eax,[ebp+12]
		mov ecx,eax
		mov dword eax,[ebp-4] ;i->eax
		add eax,eax ;i=i+i
		add eax,eax ;i=i+i
		add dword eax,[ebp+12]
		mov dword ebx,[eax]
		mov dword edx,[ecx]
		cmp edx,ebx ;сравнение элементов
		jle .L1 ;повтор цикла
		mov dword [ecx],ebx
		mov dword [eax],edx
	.L1:
		mov dword eax,[ebp-8] ;j->eax
		inc eax ;j=j+1
		mov dword [ebp-8],eax ;запись j
		cmp dword eax,[ebp-4] ;если j<i
		jle .forj ;повтор цикла для j
		mov dword eax,[ebp-4] ;i->eax
		inc eax ;i=i+1
		mov dword [ebp-4],eax ;запись i
		cmp dword eax,[ebp+8] ;если i<j
		jle .fori ;повтор цикла для i
;===============================
mov eax, [ebp+16]
push dword[ebp+20]
call eax
mov esp,ebp
;add esp, 16
pop ebp
ret