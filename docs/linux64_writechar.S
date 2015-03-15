	.global _start	
.section .bss
	.comm	writec_buf, 1	
.section .text	
_start:
	mov		$0x61, %r9
	mov		%r9, (writec_buf)
	
	push	$L0
	jmp		writec
L0:
	mov		$60, %rax
	xor		%rdi, %rdi
	syscall

writec:
	mov		$1, %rax
	mov		$1, %rdi
	mov		$writec_buf, %rsi
	mov		$1, %rdx	
	syscall
	ret

test:
	.ascii "I like cheese."


