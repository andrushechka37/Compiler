%include "andy_lib.s"
global _start
section .text
_start:
push 1
pop rax ; 
push 3
pop rbx ; 
push 1
pop rcx ; 
push 0
pop rdx ; 
call discriminant
call answer


; start of declaration of func
jmp a
discriminant:
push rbx ; 
push rbx ; 

; start of mul
mov r15, rax
pop rax
pop r13
mul r13
push rax
mov rax, r15
; end of mul

push 4
push rax ; 

; start of mul
mov r15, rax
pop rax
pop r13
mul r13
push rax
mov rax, r15
; end of mul

push rcx ; 

; start of mul
mov r15, rax
pop rax
pop r13
mul r13
push rax
mov rax, r15
; end of mul


; start of sub
pop r12
pop r13
sub r13, r12
push r13
; end of sub

pop rdx ; 
a:


; start of declaration of func
jmp b
answer:
push rax ; 
push 0

; start of cmp
pop r12
pop r13
cmp r13, r12
jne c
; end of cmp

push rbx ; 
push 0
push -1
push rbx ; 

; start of mul
mov r15, rax
pop rax
pop r13
mul r13
push rax
mov rax, r15
; end of mul

push rcx ; 

; start of div
pop r12
pop r13
div r13, r12
push r13
; end of div

pop rgx ; 


 ;just print-------------
push rax
push rgx
pop rax
call print_integer
push rsi
mov rsi, neg_flag
mov byte [rsi], 10
call print_char
pop rsi
pop rax


 ;just print-------------
ret
d:
push rbx ; 
push 0

; start of cmp
pop r12
pop r13
cmp r13, r12
jne g
; end of cmp

push rcx ; 
push 0

; start of cmp
pop r12
pop r13
cmp r13, r12
jne h
; end of cmp



 ;just print-------------
push rax
push rpx
pop rax
call print_integer
push rsi
mov rsi, neg_flag
mov byte [rsi], 10
call print_char
pop rsi
pop rax


 ;just print-------------
ret
h:
g:


 ;just print-------------
push rax
push r
x
pop rax
call print_integer
push rsi
mov rsi, neg_flag
mov byte [rsi], 10
call print_char
pop rsi
pop rax


 ;just print-------------
ret
c:
push rdx ; 
push 0

; start of cmp
pop r12
pop r13
cmp r13, r12
jbe i
; end of cmp

push -1
push rbx ; 

; start of mul
mov r15, rax
pop rax
pop r13
mul r13
push rax
mov rax, r15
; end of mul


; start of add
pop r12
pop r13
add r13, r12
push r13
; end of add

push 2
push rax ; 

; start of mul
mov r15, rax
pop rax
pop r13
mul r13
push rax
mov rax, r15
; end of mul


; start of div
pop r12
pop r13
div r13, r12
push r13
; end of div

pop rgx ; 
push -1
push rbx ; 

; start of mul
mov r15, rax
pop rax
pop r13
mul r13
push rax
mov rax, r15
; end of mul


; start of sub
pop r12
pop r13
sub r13, r12
push r13
; end of sub

push 2
push rax ; 

; start of mul
mov r15, rax
pop rax
pop r13
mul r13
push rax
mov rax, r15
; end of mul


; start of div
pop r12
pop r13
div r13, r12
push r13
; end of div

pop rhx ; 


 ;just print-------------
push rax
push rgx
pop rax
call print_integer
push rsi
mov rsi, neg_flag
mov byte [rsi], 10
call print_char
pop rsi
pop rax


 ;just print-------------


 ;just print-------------
push rax
push rhx
pop rax
call print_integer
push rsi
mov rsi, neg_flag
mov byte [rsi], 10
call print_char
pop rsi
pop rax


 ;just print-------------
ret
i:
push rdx ; 
push 0

; start of cmp
pop r12
pop r13
cmp r13, r12
jne j
; end of cmp

push -1
push rbx ; 

; start of mul
mov r15, rax
pop rax
pop r13
mul r13
push rax
mov rax, r15
; end of mul

push 2
push rax ; 

; start of mul
mov r15, rax
pop rax
pop r13
mul r13
push rax
mov rax, r15
; end of mul


; start of div
pop r12
pop r13
div r13, r12
push r13
; end of div

pop rgx ; 


 ;just print-------------
push rax
push rgx
pop rax
call print_integer
push rsi
mov rsi, neg_flag
mov byte [rsi], 10
call print_char
pop rsi
pop rax


 ;just print-------------
ret
j:
push rdx ; 
push 0

; start of cmp
pop r12
pop r13
cmp r13, r12
jae k
; end of cmp



 ;just print-------------
push rax
push r
x
pop rax
call print_integer
push rsi
mov rsi, neg_flag
mov byte [rsi], 10
call print_char
pop rsi
pop rax


 ;just print-------------
ret
k:
b:
mov rax, 60 
mov rdi, 0
syscall
