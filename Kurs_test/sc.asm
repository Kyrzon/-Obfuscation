
BITS 32
 
segment .code
_main:
sub eax, 5
sub ebx, 5
sub edx, 5
sub ecx, 5
sub esp, 5
sub ebp, 5
sub eax, ebx
or eax, 5
add eax, ebx
push ebp
mov ebp,esp

mov eax, [esp-12]
push eax
mov eax, [esp-8]
call eax
mov esp, ebp
pop ebp
ret