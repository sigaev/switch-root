#include <xmmintrin.h>

void stress_sse_16(float *fp) {
	__m128 a0 = _mm_load_ps(fp);
	__m128 a1 = _mm_load_ps(fp + 4);
	__m128 a2 = _mm_load_ps(fp + 8);
	__m128 a3 = _mm_load_ps(fp + 12);
	__m128 a4 = _mm_load_ps(fp + 16);
	__m128 a5 = _mm_load_ps(fp + 20);
	__m128 a6 = _mm_load_ps(fp + 24);
	__m128 a7 = _mm_load_ps(fp + 28);
	__m128 a8 = _mm_load_ps(fp + 32);
	__m128 a9 = _mm_load_ps(fp + 36);
	__m128 aa = _mm_load_ps(fp + 40);
	__m128 ab = _mm_load_ps(fp + 44);
	__m128 ac = _mm_load_ps(fp + 48);
	__m128 ad = _mm_load_ps(fp + 52);
	__m128 ae = _mm_load_ps(fp + 56);
	__m128 af = _mm_load_ps(fp + 60);
	for(int i = 0; i < 500000000; ++i) {
		a0 = _mm_add_ps(a0, a0);
		a1 = _mm_mul_ps(a1, a1);
		a2 = _mm_add_ps(a2, a2);
		a3 = _mm_mul_ps(a3, a3);
		a4 = _mm_add_ps(a4, a4);
		a5 = _mm_mul_ps(a5, a5);
		a6 = _mm_add_ps(a6, a6);
		a7 = _mm_mul_ps(a7, a7);
		a8 = _mm_add_ps(a8, a8);
		a9 = _mm_mul_ps(a9, a9);
		aa = _mm_add_ps(aa, aa);
		ab = _mm_mul_ps(ab, ab);
		ac = _mm_add_ps(ac, ac);
		ad = _mm_mul_ps(ad, ad);
		ae = _mm_add_ps(ae, ae);
		af = _mm_mul_ps(af, af);
	}
	_mm_store_ps(fp, _mm_add_ps(
		_mm_mul_ps(
			_mm_mul_ps(_mm_mul_ps(a0, a1), _mm_add_ps(a2, a3)),
			_mm_add_ps(_mm_mul_ps(a4, a5), _mm_add_ps(a6, a7))),
		_mm_add_ps(
			_mm_mul_ps(_mm_mul_ps(a8, a9), _mm_add_ps(aa, ab)),
			_mm_add_ps(_mm_mul_ps(ac, ad), _mm_add_ps(ae, af)))));
}

void stress_sse2_16(double *fp) {
	__m128d a0 = _mm_load_pd(fp);
	__m128d a1 = _mm_load_pd(fp + 4);
	__m128d a2 = _mm_load_pd(fp + 8);
	__m128d a3 = _mm_load_pd(fp + 12);
	__m128d a4 = _mm_load_pd(fp + 16);
	__m128d a5 = _mm_load_pd(fp + 20);
	__m128d a6 = _mm_load_pd(fp + 24);
	__m128d a7 = _mm_load_pd(fp + 28);
	__m128d a8 = _mm_load_pd(fp + 32);
	__m128d a9 = _mm_load_pd(fp + 36);
	__m128d aa = _mm_load_pd(fp + 40);
	__m128d ab = _mm_load_pd(fp + 44);
	__m128d ac = _mm_load_pd(fp + 48);
	__m128d ad = _mm_load_pd(fp + 52);
	__m128d ae = _mm_load_pd(fp + 56);
	__m128d af = _mm_load_pd(fp + 60);
	for(int i = 0; i < 500000000; ++i) {
		a0 = _mm_add_pd(a0, a0);
		a1 = _mm_mul_pd(a1, a1);
		a2 = _mm_add_pd(a2, a2);
		a3 = _mm_mul_pd(a3, a3);
		a4 = _mm_add_pd(a4, a4);
		a5 = _mm_mul_pd(a5, a5);
		a6 = _mm_add_pd(a6, a6);
		a7 = _mm_mul_pd(a7, a7);
		a8 = _mm_add_pd(a8, a8);
		a9 = _mm_mul_pd(a9, a9);
		aa = _mm_add_pd(aa, aa);
		ab = _mm_mul_pd(ab, ab);
		ac = _mm_add_pd(ac, ac);
		ad = _mm_mul_pd(ad, ad);
		ae = _mm_add_pd(ae, ae);
		af = _mm_mul_pd(af, af);
	}
	_mm_store_pd(fp, _mm_add_pd(
		_mm_mul_pd(
			_mm_mul_pd(_mm_mul_pd(a0, a1), _mm_add_pd(a2, a3)),
			_mm_add_pd(_mm_mul_pd(a4, a5), _mm_add_pd(a6, a7))),
		_mm_add_pd(
			_mm_mul_pd(_mm_mul_pd(a8, a9), _mm_add_pd(aa, ab)),
			_mm_add_pd(_mm_mul_pd(ac, ad), _mm_add_pd(ae, af)))));
}

void stress_sse_8(float *fp) {
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

void stress_sse2_8(float *fp) {
#define SSE2
#undef ADDP
#undef MULP
#undef MOVAP
$$	}
$$	}
}
