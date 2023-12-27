
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
	push 4
	push 3
	push 11
	pop rax
	pop rbx
	imul rax, rbx
	pop rbx
	div rbx
	push rax
	push 31
	pop rax
	pop rbx
	sub rax, rbx
	push rax
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