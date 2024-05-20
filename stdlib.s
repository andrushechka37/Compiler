;:====================================================
;: 0-Linux-nasm-64.s                   (c)macho64,2024
;:====================================================

;; =====================================================================
;;                          print_buffer
;; =====================================================================
;; prints stack in reversed order
;; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;;
;; Entry:       /RCX/  --  size of stack
;;
;;
;; Destr: /RCX/
;; ---------------------------------------------------------------------
print_buffer:   push rcx
                push rsi                ; save ptr to str

                mov rsi, rcx            ;
                add rsi, buffer         ; rsi = buffer[size]
                dec rsi                 ;
                
        
        print_buffer_loop:
                                        
                call print_char
                dec rsi 

                loop print_buffer_loop

                pop rsi
                pop rcx
                ret




print_integer:
                mov r12, 10                      
                mov rcx, buffer                 ; rcx = ptr to buffer
                xor rdx, rdx
;; ------------------------rax below zero-------------------------------
                test rax, 80000000h 
                jz print_integer_loop
                mov r13, neg_flag
                mov byte [r13], 1
                neg eax
;; ---------------------------------------------------------------------
        print_integer_loop:
                                        ; make a func to make divs <<< >>>> ------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                div r12                 ; div to the base of the sistema schslenia
                                        ; rdx = rdx:rax % rbx
                                        ; rax = rdx:rax / rbx
                add rdx, hex_alphabet          ;
                mov bl, [rdx]                  ; mov [rcx], hex_apphabet[rdx]
                mov [rcx], bl                  ;
                inc rcx
                cmp rax, 0
                je print_integer_end_of_ioa_loop
                xor rdx, rdx
                inc rcx                 ; because loop "eat" one inc
                loop print_integer_loop
        print_integer_end_of_ioa_loop:
;; -----------------------below zero correction--------------------------
                mov r13, neg_flag
                cmp byte [r13], 0
                mov byte [r13], 0
                je  print_integer_positive_number
                mov byte [rcx], '-'
                inc rcx
;; ---------------------------------------------------------------------
        print_integer_positive_number:
                sub rcx, buffer   ; size of stack
                call print_buffer
                ret


;; =====================================================================
;;                          print_char
;; =====================================================================
;; writes char to stdout
;; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;;
;; Entry:  /RSI/  --  pointer to char
;;
;;
;; Side Effects: writes char to stdout
;;
;; Destr: -
;; ---------------------------------------------------------------------
print_char:
                push rax
                push rdi
                push rdx
                push rcx

                mov rax, 0x01           ; write64 (rdi, rsi, rdx) ... r10, r8, r9
                mov rdi, 1              ; stdout
                mov rdx, 1              ; strlen (Msg)
                syscall

                pop rcx
                pop rdx
                pop rdi
                pop rax

                ret




section     .data
            
Msg:        db "haha", 0x0a
MsgLen      equ $ - Msg

buffer times 256 db 0
hex_alphabet: db '0123456789abcdef'
neg_flag db 0
new_line db 0



;nasm -f elf64 -g  hui.s && ld -s -o hui hui.o && ./hui
