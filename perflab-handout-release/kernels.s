	.file	"kernels.c"
	.text
	.p2align 4,,15
	.globl	naive_complex
	.type	naive_complex, @function
naive_complex:
.LFB22:
	.cfi_startproc
	testl	%edi, %edi
	jle	.L7
	leal	-1(%rdi), %ecx
	movslq	%edi, %r8
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	movl	$-1431655765, %r10d
	movl	%ecx, %eax
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	leaq	(%r8,%r8,2), %r13
	imull	%edi, %eax
	negl	%edi
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	movq	%rsi, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	addq	%r13, %r13
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	cltq
	addq	%r8, %rax
	leaq	(%rax,%rax,2), %r8
	movl	%ecx, %eax
	addq	%r8, %r8
	leaq	(%rax,%rax,2), %rax
	addq	%rax, %rax
	leaq	-6(%rdx,%r8), %r14
	leaq	(%rdx,%r8), %rbp
	leaq	6(%rsi,%rax), %rsi
	subq	%rax, %r14
	movslq	%edi, %rax
	leaq	(%rax,%rax,2), %rbx
	addq	%rbx, %rbx
	.p2align 4,,10
	.p2align 3
.L3:
	movq	%rbp, %r8
	movq	%r12, %rdi
	.p2align 4,,10
	.p2align 3
.L4:
	movzwl	2(%rdi), %ecx
	movzwl	(%rdi), %edx
	addq	$6, %rdi
	movzwl	-2(%rdi), %r9d
	addl	%ecx, %edx
	addl	%r9d, %edx
	movl	%edx, %eax
	mull	%r10d
	shrl	%edx
	movw	%dx, -6(%r8)
	movzwl	-6(%rdi), %r11d
	leal	(%rcx,%r11), %edx
	addl	%r9d, %edx
	movl	%edx, %eax
	mull	%r10d
	shrl	%edx
	movw	%dx, -4(%r8)
	movzwl	-4(%rdi), %edx
	addl	%r11d, %edx
	addl	%r9d, %edx
	movl	%edx, %eax
	mull	%r10d
	shrl	%edx
	movw	%dx, -2(%r8)
	addq	%rbx, %r8
	cmpq	%rdi, %rsi
	jne	.L4
	subq	$6, %rbp
	addq	%r13, %r12
	addq	%r13, %rsi
	cmpq	%r14, %rbp
	jne	.L3
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
.L7:
	.cfi_restore 3
	.cfi_restore 6
	.cfi_restore 12
	.cfi_restore 13
	.cfi_restore 14
	ret
	.cfi_endproc
.LFE22:
	.size	naive_complex, .-naive_complex
	.p2align 4,,15
	.globl	complexImplementationV1
	.type	complexImplementationV1, @function
complexImplementationV1:
.LFB23:
	.cfi_startproc
	testl	%edi, %edi
	jle	.L18
	leal	-1(%rdi), %ecx
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	movslq	%edi, %r8
	movss	.LC0(%rip), %xmm1
	movl	%ecx, %eax
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	leaq	(%r8,%r8,2), %r11
	imull	%edi, %eax
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	sall	$2, %edi
	addq	%r11, %r11
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	negl	%edi
	xorl	%r13d, %r13d
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	cltq
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	leaq	(%rax,%rax,2), %rax
	addq	%rax, %rax
	leaq	-6(%rdx,%rax), %r12
	leaq	-12(%rdx,%rax), %rdx
	movl	%ecx, %eax
	shrl	$2, %ecx
	leaq	(%rax,%rax,2), %rax
	addq	%rax, %rax
	subq	%rax, %rdx
	movslq	%edi, %rax
	leaq	(%rax,%rax,2), %rbx
	leaq	0(,%r8,4), %rax
	movq	%rdx, -16(%rsp)
	subq	%rax, %r8
	addq	%rbx, %rbx
	leaq	0(,%r8,4), %rax
	movq	%rax, -8(%rsp)
	movl	%ecx, %eax
	leaq	(%rax,%rax,2), %rax
	leaq	24(%rsi,%rax,8), %r14
	.p2align 4,,10
	.p2align 3
.L14:
	movq	-8(%rsp), %rcx
	leaq	(%rsi,%r13), %rax
	leaq	(%r14,%r13), %rbp
	movq	%r12, %rdx
	addq	%r12, %rcx
	.p2align 4,,10
	.p2align 3
.L13:
	movzwl	4(%rax), %r8d
	movzwl	(%rax), %edi
	pxor	%xmm0, %xmm0
	addq	$24, %rax
	movzwl	-2(%rax), %r15d
	addl	%r8d, %edi
	movzwl	-22(%rax), %r8d
	addl	%r8d, %edi
	movzwl	-14(%rax), %r8d
	cvtsi2ss	%edi, %xmm0
	movzwl	-18(%rax), %edi
	addl	%r8d, %edi
	movzwl	-16(%rax), %r8d
	mulss	%xmm1, %xmm0
	addl	%r8d, %edi
	movzwl	-8(%rax), %r8d
	cvttss2si	%xmm0, %r10d
	pxor	%xmm0, %xmm0
	cvtsi2ss	%edi, %xmm0
	movzwl	-12(%rax), %edi
	addl	%r8d, %edi
	movzwl	-10(%rax), %r8d
	mulss	%xmm1, %xmm0
	addl	%r8d, %edi
	cvttss2si	%xmm0, %r9d
	pxor	%xmm0, %xmm0
	cvtsi2ss	%edi, %xmm0
	movzwl	-6(%rax), %edi
	addl	%r15d, %edi
	movzwl	-4(%rax), %r15d
	movw	%r10w, (%rdx,%r11)
	movw	%r10w, 2(%rdx,%r11)
	mulss	%xmm1, %xmm0
	addl	%r15d, %edi
	movw	%r10w, 4(%rdx,%r11)
	movw	%r9w, (%rdx)
	movw	%r9w, 4(%rdx)
	movw	%r9w, 2(%rdx)
	addq	%rbx, %rdx
	cvttss2si	%xmm0, %r8d
	pxor	%xmm0, %xmm0
	cvtsi2ss	%edi, %xmm0
	movw	%r8w, (%rcx,%r11)
	mulss	%xmm1, %xmm0
	movw	%r8w, 2(%rcx,%r11)
	movw	%r8w, 4(%rcx,%r11)
	cvttss2si	%xmm0, %edi
	movw	%di, (%rcx)
	movw	%di, 2(%rcx)
	movw	%di, 4(%rcx)
	addq	%rbx, %rcx
	cmpq	%rax, %rbp
	jne	.L13
	addq	%r11, %r13
	subq	$6, %r12
	cmpq	%r12, -16(%rsp)
	jne	.L14
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
.L18:
	.cfi_restore 3
	.cfi_restore 6
	.cfi_restore 12
	.cfi_restore 13
	.cfi_restore 14
	.cfi_restore 15
	ret
	.cfi_endproc
.LFE23:
	.size	complexImplementationV1, .-complexImplementationV1
	.p2align 4,,15
	.type	weighted_combo_V4, @function
weighted_combo_V4:
.LFB29:
	.cfi_startproc
	leal	-3(%rdi), %eax
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	cmpl	%esi, %eax
	jg	.L22
	leal	-4(%rdi), %r8d
	movl	%esi, %r13d
	movl	%edx, %r11d
	subl	%r8d, %r11d
	subl	%r8d, %r13d
	cmpl	%edx, %eax
	movl	$3, %eax
	cmovg	%eax, %r11d
	testl	%r13d, %r13d
	jle	.L42
.L34:
	imull	%edi, %esi
	xorl	%r12d, %r12d
	xorl	%r10d, %r10d
	xorl	%r9d, %r9d
	xorl	%r8d, %r8d
	xorl	%ebp, %ebp
	leal	(%rsi,%rdx), %ebx
	.p2align 4,,10
	.p2align 3
.L27:
	testl	%r11d, %r11d
	jle	.L25
	movl	%ebx, %esi
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L26:
	leal	(%rsi,%rax), %edx
	addl	$1, %eax
	movq	%rdx, %rsi
	leaq	(%rdx,%rdx,2), %rdx
	leaq	(%rcx,%rdx,2), %rdx
	movzwl	(%rdx), %r14d
	addl	%r14d, %r8d
	movzwl	2(%rdx), %r14d
	movzwl	4(%rdx), %edx
	addl	%r14d, %r9d
	addl	%edx, %r10d
	cmpl	%r11d, %eax
	jne	.L26
	addl	%r11d, %r12d
.L25:
	addl	$1, %ebp
	addl	%edi, %ebx
	cmpl	%r13d, %ebp
	jne	.L27
	pxor	%xmm3, %xmm3
	pxor	%xmm0, %xmm0
	pxor	%xmm1, %xmm1
	pxor	%xmm2, %xmm2
	cvtsi2sd	%r12d, %xmm3
	cvtsi2ssq	%r8, %xmm0
	cvtsi2ssq	%r9, %xmm1
	cvtsi2ssq	%r10, %xmm2
.L24:
	movsd	.LC3(%rip), %xmm4
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	divsd	%xmm3, %xmm4
	pxor	%xmm3, %xmm3
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	cvtsd2ss	%xmm4, %xmm3
	mulss	%xmm3, %xmm1
	mulss	%xmm3, %xmm0
	mulss	%xmm3, %xmm2
	cvttss2si	%xmm1, %eax
	cvttss2si	%xmm0, %edx
	cvttss2si	%xmm2, %esi
	movzwl	%ax, %eax
	salq	$16, %rax
	movq	%rax, %rcx
	movzwl	%dx, %eax
	movzwl	%si, %edx
	salq	$32, %rdx
	orq	%rcx, %rax
	orq	%rdx, %rax
	ret
.L22:
	.cfi_restore_state
	cmpl	%edx, %eax
	jg	.L37
	leal	-4(%rdi), %eax
	movl	%edx, %r11d
	movl	$3, %r13d
	subl	%eax, %r11d
	jmp	.L34
.L37:
	movl	$3, %r11d
	movl	$3, %r13d
	jmp	.L34
.L42:
	pxor	%xmm2, %xmm2
	pxor	%xmm1, %xmm1
	pxor	%xmm0, %xmm0
	pxor	%xmm3, %xmm3
	jmp	.L24
	.cfi_endproc
.LFE29:
	.size	weighted_combo_V4, .-weighted_combo_V4
	.p2align 4,,15
	.globl	naive_motion
	.type	naive_motion, @function
naive_motion:
.LFB31:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	movq	%rsi, -32(%rsp)
	testl	%edi, %edi
	jle	.L43
	movslq	%edi, %rax
	movq	%rdx, -16(%rsp)
	movl	$3, %r12d
	xorl	%r13d, %r13d
	leaq	(%rax,%rax,2), %rax
	movl	$0, -20(%rsp)
	addq	%rax, %rax
	movq	%rax, -8(%rsp)
	.p2align 4,,10
	.p2align 3
.L45:
	movq	-16(%rsp), %r10
	xorl	%ebp, %ebp
	.p2align 4,,10
	.p2align 3
.L50:
	movl	-20(%rsp), %ebx
	movl	%r13d, %r9d
	xorl	%ecx, %ecx
	xorl	%esi, %esi
	xorl	%r8d, %r8d
	xorl	%eax, %eax
	leal	3(%rbp), %r15d
.L46:
	movl	%ebp, %edx
.L48:
	cmpl	%r9d, %edi
	jle	.L47
	cmpl	%edi, %edx
	jge	.L47
	leal	(%rbx,%rdx), %r11d
	movq	-32(%rsp), %r14
	addl	$1, %ecx
	movslq	%r11d, %r11
	leaq	(%r11,%r11,2), %r11
	leaq	(%r14,%r11,2), %r11
	movzwl	(%r11), %r14d
	addl	%r14d, %eax
	movzwl	2(%r11), %r14d
	movzwl	4(%r11), %r11d
	addl	%r14d, %r8d
	addl	%r11d, %esi
.L47:
	addl	$1, %edx
	cmpl	%r15d, %edx
	jne	.L48
	addl	$1, %r9d
	addl	%edi, %ebx
	cmpl	%r12d, %r9d
	jne	.L46
	cltd
	addq	$6, %r10
	idivl	%ecx
	movw	%ax, -6(%r10)
	movl	%r8d, %eax
	cltd
	idivl	%ecx
	movw	%ax, -4(%r10)
	movl	%esi, %eax
	cltd
	idivl	%ecx
	movw	%ax, -2(%r10)
	leal	1(%rbp), %eax
	cmpl	%eax, %edi
	je	.L54
	movl	%eax, %ebp
	jmp	.L50
	.p2align 4,,10
	.p2align 3
.L54:
	movq	-8(%rsp), %rdx
	leal	1(%r13), %eax
	addl	$1, %r12d
	addl	%edi, -20(%rsp)
	addq	%rdx, -16(%rsp)
	cmpl	%ebp, %r13d
	je	.L43
	movl	%eax, %r13d
	jmp	.L45
	.p2align 4,,10
	.p2align 3
.L43:
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE31:
	.size	naive_motion, .-naive_motion
	.p2align 4,,15
	.globl	motionV2
	.type	motionV2, @function
motionV2:
.LFB34:
	.cfi_startproc
	testl	%edi, %edi
	jle	.L62
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	movq	%rdx, %r15
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	leal	-1(%rdi), %r14d
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	andl	$-4, %r14d
	movq	%rsi, %r13
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	leal	4(%r14), %eax
	movl	%edi, %r12d
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	xorl	%ebp, %ebp
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$24, %rsp
	.cfi_def_cfa_offset 80
	movl	$0, 20(%rsp)
	movl	%eax, 16(%rsp)
	.p2align 4,,10
	.p2align 3
.L58:
	movl	20(%rsp), %r8d
	xorl	%ebx, %ebx
	.p2align 4,,10
	.p2align 3
.L57:
	addl	%ebx, %r8d
	movl	%r12d, %edi
	movl	%ebx, %edx
	movq	%r13, %rcx
	movslq	%r8d, %rax
	movl	%ebp, %esi
	movl	%r8d, 12(%rsp)
	leaq	(%rax,%rax,2), %r11
	leaq	(%r11,%r11), %r14
	movq	%r11, (%rsp)
	call	weighted_combo_V4
	movq	(%rsp), %r11
	movq	%r13, %rcx
	movl	%ebp, %esi
	movq	%rax, %rdx
	movw	%ax, (%r15,%r11,2)
	shrq	$16, %rdx
	shrq	$32, %rax
	movw	%dx, 2(%r15,%r14)
	movl	%ebx, %edx
	movw	%ax, 4(%r15,%r14)
	call	weighted_combo_V4
	movq	%r13, %rcx
	movl	%ebp, %esi
	movq	%rax, %rdx
	movw	%ax, 6(%r15,%r14)
	shrq	$32, %rax
	shrq	$16, %rdx
	movw	%ax, 10(%r15,%r14)
	movw	%dx, 8(%r15,%r14)
	movl	%ebx, %edx
	call	weighted_combo_V4
	movq	%r13, %rcx
	movl	%ebp, %esi
	movq	%rax, %rdx
	movw	%ax, 12(%r15,%r14)
	shrq	$32, %rax
	shrq	$16, %rdx
	movw	%ax, 16(%r15,%r14)
	movw	%dx, 14(%r15,%r14)
	movl	%ebx, %edx
	addl	$4, %ebx
	call	weighted_combo_V4
	movl	12(%rsp), %r8d
	movq	%rax, %rdx
	movw	%ax, 18(%r15,%r14)
	shrq	$32, %rax
	shrq	$16, %rdx
	cmpl	16(%rsp), %ebx
	movw	%ax, 22(%r15,%r14)
	movw	%dx, 20(%r15,%r14)
	jne	.L57
	addl	$1, %ebp
	addl	%r12d, 20(%rsp)
	cmpl	%ebp, %r12d
	jne	.L58
	addq	$24, %rsp
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
.L62:
	.cfi_restore 3
	.cfi_restore 6
	.cfi_restore 12
	.cfi_restore 13
	.cfi_restore 14
	.cfi_restore 15
	ret
	.cfi_endproc
.LFE34:
	.size	motionV2, .-motionV2
	.p2align 4,,15
	.globl	complexImplementationV2
	.type	complexImplementationV2, @function
complexImplementationV2:
.LFB24:
	.cfi_startproc
	ret
	.cfi_endproc
.LFE24:
	.size	complexImplementationV2, .-complexImplementationV2
	.p2align 4,,15
	.globl	complex
	.type	complex, @function
complex:
.LFB25:
	.cfi_startproc
	testl	%edi, %edi
	jle	.L73
	leal	-1(%rdi), %ecx
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	movslq	%edi, %r8
	movss	.LC0(%rip), %xmm1
	movl	%ecx, %eax
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	leaq	(%r8,%r8,2), %r11
	imull	%edi, %eax
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	sall	$2, %edi
	addq	%r11, %r11
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	negl	%edi
	xorl	%r13d, %r13d
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	cltq
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	leaq	(%rax,%rax,2), %rax
	addq	%rax, %rax
	leaq	-6(%rdx,%rax), %r12
	leaq	-12(%rdx,%rax), %rdx
	movl	%ecx, %eax
	shrl	$2, %ecx
	leaq	(%rax,%rax,2), %rax
	addq	%rax, %rax
	subq	%rax, %rdx
	movslq	%edi, %rax
	leaq	(%rax,%rax,2), %rbx
	leaq	0(,%r8,4), %rax
	movq	%rdx, -16(%rsp)
	subq	%rax, %r8
	addq	%rbx, %rbx
	leaq	0(,%r8,4), %rax
	movq	%rax, -8(%rsp)
	movl	%ecx, %eax
	leaq	(%rax,%rax,2), %rax
	leaq	24(%rsi,%rax,8), %r14
	.p2align 4,,10
	.p2align 3
.L69:
	movq	-8(%rsp), %rcx
	leaq	(%rsi,%r13), %rax
	leaq	(%r14,%r13), %rbp
	movq	%r12, %rdx
	addq	%r12, %rcx
	.p2align 4,,10
	.p2align 3
.L68:
	movzwl	4(%rax), %r8d
	movzwl	(%rax), %edi
	pxor	%xmm0, %xmm0
	addq	$24, %rax
	movzwl	-2(%rax), %r15d
	addl	%r8d, %edi
	movzwl	-22(%rax), %r8d
	addl	%r8d, %edi
	movzwl	-14(%rax), %r8d
	cvtsi2ss	%edi, %xmm0
	movzwl	-18(%rax), %edi
	addl	%r8d, %edi
	movzwl	-16(%rax), %r8d
	mulss	%xmm1, %xmm0
	addl	%r8d, %edi
	movzwl	-8(%rax), %r8d
	cvttss2si	%xmm0, %r10d
	pxor	%xmm0, %xmm0
	cvtsi2ss	%edi, %xmm0
	movzwl	-12(%rax), %edi
	addl	%r8d, %edi
	movzwl	-10(%rax), %r8d
	mulss	%xmm1, %xmm0
	addl	%r8d, %edi
	cvttss2si	%xmm0, %r9d
	pxor	%xmm0, %xmm0
	cvtsi2ss	%edi, %xmm0
	movzwl	-6(%rax), %edi
	addl	%r15d, %edi
	movzwl	-4(%rax), %r15d
	movw	%r10w, (%rdx,%r11)
	movw	%r10w, 2(%rdx,%r11)
	mulss	%xmm1, %xmm0
	addl	%r15d, %edi
	movw	%r10w, 4(%rdx,%r11)
	movw	%r9w, (%rdx)
	movw	%r9w, 4(%rdx)
	movw	%r9w, 2(%rdx)
	addq	%rbx, %rdx
	cvttss2si	%xmm0, %r8d
	pxor	%xmm0, %xmm0
	cvtsi2ss	%edi, %xmm0
	movw	%r8w, (%rcx,%r11)
	mulss	%xmm1, %xmm0
	movw	%r8w, 2(%rcx,%r11)
	movw	%r8w, 4(%rcx,%r11)
	cvttss2si	%xmm0, %edi
	movw	%di, (%rcx)
	movw	%di, 2(%rcx)
	movw	%di, 4(%rcx)
	addq	%rbx, %rcx
	cmpq	%rax, %rbp
	jne	.L68
	addq	%r11, %r13
	subq	$6, %r12
	cmpq	%r12, -16(%rsp)
	jne	.L69
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
.L73:
	.cfi_restore 3
	.cfi_restore 6
	.cfi_restore 12
	.cfi_restore 13
	.cfi_restore 14
	.cfi_restore 15
	ret
	.cfi_endproc
.LFE25:
	.size	complex, .-complex
	.p2align 4,,15
	.globl	register_complex_functions
	.type	register_complex_functions, @function
register_complex_functions:
.LFB26:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$naive_complex_descr, %esi
	movl	$naive_complex, %edi
	call	add_complex_function
	movl	$V1Descr, %esi
	movl	$complexImplementationV1, %edi
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	jmp	add_complex_function
	.cfi_endproc
.LFE26:
	.size	register_complex_functions, .-register_complex_functions
	.p2align 4,,15
	.globl	motion
	.type	motion, @function
motion:
.LFB32:
	.cfi_startproc
	jmp	naive_motion
	.cfi_endproc
.LFE32:
	.size	motion, .-motion
	.p2align 4,,15
	.globl	motionV1
	.type	motionV1, @function
motionV1:
.LFB33:
	.cfi_startproc
	testl	%edi, %edi
	jle	.L79
	cmpl	$3, %edi
	jg	.L81
	movl	$4, %eax
	subl	%edi, %eax
	testl	%eax, %eax
	jle	.L82
.L81:
.L83:
	jmp	.L83
	.p2align 4,,10
	.p2align 3
.L82:
	ud2
.L79:
	ret
	.cfi_endproc
.LFE33:
	.size	motionV1, .-motionV1
	.p2align 4,,15
	.globl	register_motion_functions
	.type	register_motion_functions, @function
register_motion_functions:
.LFB35:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$motV2Desc, %esi
	movl	$motionV2, %edi
	call	add_motion_function
	movl	$naive_motion_descr, %esi
	movl	$naive_motion, %edi
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	jmp	add_motion_function
	.cfi_endproc
.LFE35:
	.size	register_motion_functions, .-register_motion_functions
	.globl	motV2Desc
	.data
	.align 32
	.type	motV2Desc, @object
	.size	motV2Desc, 41
motV2Desc:
	.string	"Motion V2: Unrolling + V4 weighted Combo"
	.globl	motionV1_descr
	.align 32
	.type	motionV1_descr, @object
	.size	motionV1_descr, 42
motionV1_descr:
	.string	"motion: Used Common Expressions technique"
	.globl	motion_descr
	.align 16
	.type	motion_descr, @object
	.size	motion_descr, 25
motion_descr:
	.string	"motion: Working Baseline"
	.globl	naive_motion_descr
	.align 32
	.type	naive_motion_descr, @object
	.size	naive_motion_descr, 44
naive_motion_descr:
	.string	"naive_motion: Naive baseline implementation"
	.globl	complex_descr
	.align 32
	.type	complex_descr, @object
	.size	complex_descr, 43
complex_descr:
	.string	"complex: Current working version (2nd Try)"
	.globl	V2Descr
	.align 32
	.type	V2Descr, @object
	.size	V2Descr, 52
V2Descr:
	.string	"Loop Unrolling 8x+ Strength Reduction + Code Motion"
	.globl	V1Descr
	.align 16
	.type	V1Descr, @object
	.size	V1Descr, 21
V1Descr:
	.string	"Version 1: unrolling"
	.globl	naive_complex_descr
	.align 32
	.type	naive_complex_descr, @object
	.size	naive_complex_descr, 45
naive_complex_descr:
	.string	"naive_complex: Naive baseline implementation"
	.globl	student
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC4:
	.string	"Aidan Naito"
.LC5:
	.string	"u1280587@utah.edu"
	.data
	.align 16
	.type	student, @object
	.size	student, 16
student:
	.quad	.LC4
	.quad	.LC5
	.section	.rodata.cst4,"aM",@progbits,4
	.align 4
.LC0:
	.long	1051372203
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC3:
	.long	0
	.long	1072693248
	.ident	"GCC: (GNU) 8.5.0 20210514 (Red Hat 8.5.0-10)"
	.section	.note.GNU-stack,"",@progbits
