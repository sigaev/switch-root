void stress_sse(float *fp) {
$$	foreach my $b(0 .. 1) {
#ifndef SSE2
	#define MOVAP	"movaps"
	#define ADDP	"addps"
	#define MULP	"mulps"
#else
	#define MOVAP	"movapd"
	#define ADDP	"addpd"
	#define MULP	"mulpd"
#endif
	__asm__ volatile(
$$	local $" = "\n\t\t";
$$	my $a = '$';
		@{[map 'MOVAP "'."	(%0), %%xmm$_\n\t".'"', 0 .. 7]}
		"movl	${a}4999999, %%eax\n\t"
		".align	16\n\t"
		"label$b:\n\t"
		"subl	${a}1, %%eax\n\t"
#if 0
/* looks like NetBurst is not very sensitive to the number
 * of incl's in the cycle; P4 does feel it though */
		ADDP "	%%xmm0, %%xmm0\n\t"
// ADDPS has latency of 4-5, throughput of 2 (P4, PM)
//		"incl	%%ecx\n\t"
		MULP "	%%xmm1, %%xmm1\n\t"
// MULPS has latency of 4-7, throughput of 2 (P4, PM)
		"incl	%%edx\n\t"
		ADDP "	%%xmm2, %%xmm2\n\t"
//		"incl	%%ebx\n\t"
		MULP "	%%xmm3, %%xmm3\n\t"
//		"incl	%%ebp\n\t"
		ADDP "	%%xmm4, %%xmm4\n\t"
		"incl	%%esi\n\t"
		MULP "	%%xmm5, %%xmm5\n\t"
//		"incl	%%edi\n\t"
		ADDP "	%%xmm6, %%xmm6\n\t"
//		"movl	%%esp, %%esp\n\t"
		MULP "	%%xmm7, %%xmm7\n\t"
#else
$$	foreach(0 .. 799) {
		ADDP "	%%xmm@{[(2*$_) & 7]}, %%xmm@{[(2*$_) & 7]}\n\t"
		MULP "	%%xmm@{[(2*$_ + 1) & 7]}, %%xmm@{[(2*$_ + 1) & 7]}\n\t"
$$	}
#endif
		"jnc	label$b\n\t"
		:
		: "r" (fp)
		: "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7"
	);
$$	if($b == 0) {
}

void stress_sse2(float *fp) {
#define SSE2
#undef ADDP
#undef MULP
#undef MOVAP
$$	}
$$	}
}
