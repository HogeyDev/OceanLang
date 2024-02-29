section .text
	push rbp
	mov rbp, rsp
	push rbp
	mov rbp, rsp
	push rbp
	mov rbp, rsp
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
	mov rsp, rbp
	pop rbp
	push rbp
	mov rbp, rsp
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
	mov rsp, rbp
	pop rbp
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
global _print_char
_print_char:
	push rbp
	mov rbp, rsp
	push QWORD [rsp+16]
	mov rax, 1
	mov rdi, 1
	mov rdx, 1
	mov rsi, rsp
	syscall
	add rsp, 8
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
	push 10
	call _print_char
	add rsp, 8
	mov rsp, rbp
	pop rbp
	ret
global _print_number
_print_number:
	push rbp
	mov rbp, rsp
	push QWORD [rsp+16]
	push 1
	pop rbx
	pop rax
	cmp rax, rbx
	setl al
	movzx rax, al
	push rax
	pop rax
	cmp rax, 0
	je lbl0
	push rbp
	mov rbp, rsp
	push 48
	call _print_char
	add rsp, 8
	mov rsp, rbp
	pop rbp
lbl0:
	push QWORD [rsp+16]
	push 0
	pop rbx
	pop rax
	cmp rax, rbx
	setg al
	movzx rax, al
	push rax
	pop rax
	cmp rax, 0
	je lbl1
	push rbp
	mov rbp, rsp
	push QWORD [rsp+24]
lbl2:
	push QWORD [rsp+0]
	push 0
	pop rbx
	pop rax
	cmp rax, rbx
	setg al
	movzx rax, al
	push rax
	pop rax
	cmp rax, 0
	je lbl3
	push rbp
	mov rbp, rsp
	push QWORD [rsp+8]
	push 10
	pop rbx
	pop rax
	mov rdx, 0
	div rbx
	mov rax, rdx
	push rax
	push 48
	push QWORD [rsp+8]
	pop rbx
	pop rax
	add rax, rbx
	push rax
	call _print_char
	add rsp, 8
	push QWORD [rsp+16]
	push 10
	pop rbx
	pop rax
	mov rdx, 0
	div rbx
	push rax
	pop rax
	mov QWORD [rsp+16], rax
	mov rsp, rbp
	pop rbp
	jmp lbl2
lbl3:
	mov rsp, rbp
	pop rbp
lbl1:
	push 10
	call _print_char
	add rsp, 8
	mov rsp, rbp
	pop rbp
	ret
	mov rsp, rbp
	pop rbp
global _main
_main:
	push rbp
	mov rbp, rsp
	push 0
	push QWORD [rsp+0]
	call _print_number
	add rsp, 8
	push 0
	pop rax
	mov rsp, rbp
	pop rbp
	ret
	mov rsp, rbp
	pop rbp
	ret
	mov rsp, rbp
	pop rbp
global _start
_start:
	call _main
	push rax
	mov rax, 60
	pop rdi
	syscall
section .data
