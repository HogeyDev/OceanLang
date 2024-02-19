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
	push QWORD [rsp+16]
	push 5
	pop rbx
	pop rax
	cmp rax, rbx
	setl al
	movzx rax, al
	push rax
	pop rax
	cmp rax, 0
	je lbl0
	push QWORD [rsp+16]
	pop rax
	mov rsp, rbp
	pop rbp
	ret
lbl0:
	push 5
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
	call _reassignmentTest
	add rsp, 0
	push rax
	pop rax
	mov rsp, rbp
	pop rbp
	ret
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
