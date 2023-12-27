
strlen:
	push rbp
	mov rbp, rsp

	mov rsi, [rsp+16]
	mov rcx, -1
.strlen0:
	inc rcx
	cmp byte [rsi + rcx], 0x00
	jne .strlen0
.strlen1:
	mov rdx, rcx

	mov rsp, rbp
	pop rbp
	ret

print:
	push rbp
	mov rbp, rsp

	mov rax, [rsp+16]
	push rax
	call strlen
	add rsp, 8

	mov rax, 1
	mov rdi, 1
	mov rsi, [rsp+16]
	syscall

	mov rsp, rbp
	pop rbp
	ret

exit:
    mov rax, 60
    mov rdi, QWORD [rsp+8]
    add rsp, 8
    syscall
global main
main:
	push rbp
	mov rbp, rsp
	push 41
	push 3
	pop rax
	pop rbx
	imul rax, rbx
	push rax
	push 123
	pop rax
	pop rbx
	sub rax, rbx
	push rax
	push 1
	pop rax
	pop rbx
	add rax, rbx
	cmp rax, 0
	je lbl0
	push 42
	call exit
lbl0:
	push 69
	call exit
	mov rsp, rbp
	pop rbp
	ret
global _start
_start:
	call main
	mov rax, 60
	mov rdi, 0
	syscall