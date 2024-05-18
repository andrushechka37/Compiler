section .text
main:
:0
push rax
push 1

; start of cmp
pop r12
pop r13
cmp r13, r12
jne :0
; end of cmp

push 1
pop rax
jmp :0
:1
