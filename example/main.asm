section .text
_strlen:
	push rbp
	mov rbp, rsp
	mov rsi, [rsp+16]
	mov rax, -1
.strlen0:
	inc rax
	cmp byte [rsi + rax], 0x00
	jne .strlen0
.strlen1:
	mov rsp, rbp
	pop rbp
	ret
global _print
_print:
	push rbp
	mov rbp, rsp
	push QWORD [rsp+16]
	call _strlen
	add rsp, 8
	push rax
	mov rax, 1
	mov rdi, 1
	pop rdx
	mov rsi, [rsp+16]
	syscall
	mov rsp, rbp
	pop rbp
	ret
global _println
_println:
	push rbp
	mov rbp, rsp
	push QWORD [rsp+16]
	call _print
	add rsp, 8
	push STR0
	call _print
	add rsp, 8
	mov rsp, rbp
	pop rbp
	ret
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
global _reassignmentTest
_reassignmentTest:
	push rbp
	mov rbp, rsp
	push 69
	push QWORD [rsp+24]
	push 99
	pop rbx
	pop rax
	cmp rax, rbx
	setg al
	movzx rax, al
	push rax
	pop rax
	cmp rax, 0
	je lbl0
	push 42
	pop rax
	mov QWORD [rsp+0], rax
lbl0:
	push QWORD [rsp+0]
	pop rax
	mov rsp, rbp
	pop rbp
	ret
	mov rsp, rbp
	pop rbp
	ret
global _fib
_fib:
	push rbp
	mov rbp, rsp
	push QWORD [rsp+16]
	push 3
	pop rbx
	pop rax
	cmp rax, rbx
	setl al
	movzx rax, al
	push rax
	pop rax
	cmp rax, 0
	je lbl1
	push 1
	pop rax
	mov rsp, rbp
	pop rbp
	ret
lbl1:
	push QWORD [rsp+16]
	push 1
	pop rbx
	pop rax
	sub rax, rbx
	push rax
	call _fib
	add rsp, 8
	push rax
	push QWORD [rsp+24]
	push 2
	pop rbx
	pop rax
	sub rax, rbx
	push rax
	call _fib
	add rsp, 8
	push rax
	pop rbx
	pop rax
	add rax, rbx
	push rax
	pop rax
	mov rsp, rbp
	pop rbp
	ret
	mov rsp, rbp
	pop rbp
	ret
global _main
_main:
	push rbp
	mov rbp, rsp
	push 14
	call _reassignmentTest
	add rsp, 8
	push rax
	pop rax
	mov rsp, rbp
	pop rbp
	ret
	push 13
	call _fib
	add rsp, 8
	push rax
	call _exit
	add rsp, 8
	mov rsp, rbp
	pop rbp
	ret
global _start
_start:
	call _main
	push rax
	mov rax, 60
	pop rdi
	syscall
section .data
STR0 db "", 0xa, "", 0
