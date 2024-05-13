section .text
main:
push 9
push 7

; start of add
pop r12
pop r13
add r13, r12
push r13
; end of add

pop rax
