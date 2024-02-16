section .text
section .text
section .text

_strlen:
	push rbp
	mov rbp, rsp

	mov rsi, [rsp+16]
	mov rcx, -1
.strlen0:
	inc rcx
	cmp byte [rsi + rcx], 0x00
	jne .strlen0
.strlen1:
	mov rax, rcx

	mov rsp, rbp
	pop rbp
	ret
section .data
section .text


_println:
	push rbp
	mov rbp, rsp

	mov rax, QWORD [rsp+16]
	push rax
	call _print
	add rsp, 8

	mov rax, 1
	mov rdi, 1
	mov rsi, 0xa
	mov rdx, 1
	syscall

	mov rsp, rbp
	pop rbp
	ret
global _print
_print: ; STR0 _ STR0 _ 
	push rbp
	mov rbp, rsp
	push QWORD [rsp+16]
	call _strlen
	add rsp, 8
	push rax
	mov rax, 1
	mov rdi, 1
	pop rdx
	mov rsi, QWORD [rsp+16]
	syscall
	mov rsp, rbp
	pop rbp
	ret
section .data
section .text
section .text
global _exit
_exit:
	push rbp
	mov rbp, rsp

		mov rax, 60
		mov rdi, QWORD [rsp+16]
		add rsp, 8
		syscall
		mov rsp, rbp
	pop rbp
	ret
section .data
section .text
global _main
_main:
	push rbp
	mov rbp, rsp
	push STR0
	call _println
	add rsp, 8
	push rax
	push 12
	pop rax
	mov rsp, rbp
	pop rbp
	ret
	mov rsp, rbp
	pop rbp
	ret
section .data
STR0 db "Hello, World!", 0
section .text
global _start
_start:
	call _main
	push rax
	mov rax, 60
	pop rdi
	syscall
