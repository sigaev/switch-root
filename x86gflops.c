// PP + C99, 1 Dec 2005 MIT, 29 Aug 2012 Google

#include <error.h>
#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/time.h>

static inline unsigned long long mytime() {
	struct timeval tv;

	return gettimeofday(&tv, NULL)
		? 0ULL : 1000000ULL * tv.tv_sec + tv.tv_usec;
}

static inline void usage() {
	error(1, 0, "Usage: x86gflops sse|sse2|avx [n]");
}

static float fp[128] __attribute__ ((aligned(32))) = { .0f };

int main(int argc, char* argv[]) {
	if(argc < 2 || argc > 3) {
		usage();
	}
	enum {
		SSE, SSE2, AVX
	} mode;
	if(!strcasecmp(argv[1], "sse")) {
		mode = SSE;
	} else if(!strcasecmp(argv[1], "sse2")) {
		mode = SSE2;
	} else if(!strcasecmp(argv[1], "avx")) {
		mode = AVX;
	} else {
		usage();
	}
	int n = argc == 3 ? atoi(argv[2]) : 0;
	do {
		unsigned long long t = mytime();
		if(mode == AVX) {
			__m256 a0 = _mm256_load_ps(fp);
			__m256 a1 = _mm256_load_ps(fp + 8);
			__m256 a2 = _mm256_load_ps(fp + 16);
			__m256 a3 = _mm256_load_ps(fp + 24);
			__m256 a4 = _mm256_load_ps(fp + 32);
			__m256 a5 = _mm256_load_ps(fp + 40);
			__m256 a6 = _mm256_load_ps(fp + 48);
			__m256 a7 = _mm256_load_ps(fp + 56);
			__m256 a8 = _mm256_load_ps(fp + 64);
			__m256 a9 = _mm256_load_ps(fp + 72);
			__m256 aa = _mm256_load_ps(fp + 80);
			__m256 ab = _mm256_load_ps(fp + 88);
			__m256 ac = _mm256_load_ps(fp + 96);
			__m256 ad = _mm256_load_ps(fp + 104);
			__m256 ae = _mm256_load_ps(fp + 112);
			__m256 af = _mm256_load_ps(fp + 120);
			for(int i = 0; i < 500000000; ++i) {
				a0 = _mm256_add_ps(a0, a0);
				a1 = _mm256_mul_ps(a1, a1);
				a2 = _mm256_add_ps(a2, a2);
				a3 = _mm256_mul_ps(a3, a3);
				a4 = _mm256_add_ps(a4, a4);
				a5 = _mm256_mul_ps(a5, a5);
				a6 = _mm256_add_ps(a6, a6);
				a7 = _mm256_mul_ps(a7, a7);
				a8 = _mm256_add_ps(a8, a8);
				a9 = _mm256_mul_ps(a9, a9);
				aa = _mm256_add_ps(aa, aa);
				ab = _mm256_mul_ps(ab, ab);
				ac = _mm256_add_ps(ac, ac);
				ad = _mm256_mul_ps(ad, ad);
				ae = _mm256_add_ps(ae, ae);
				af = _mm256_mul_ps(af, af);
			}
			_mm256_store_ps(fp, _mm256_add_ps(
				_mm256_add_ps(
					_mm256_add_ps(_mm256_add_ps(a0, a1), _mm256_add_ps(a2, a3)),
					_mm256_add_ps(_mm256_add_ps(a4, a5), _mm256_add_ps(a6, a7))),
				_mm256_add_ps(
					_mm256_add_ps(_mm256_add_ps(a8, a9), _mm256_add_ps(aa, ab)),
					_mm256_add_ps(_mm256_add_ps(ac, ad), _mm256_add_ps(ae, af)))));
		} else if(mode == SSE) {
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
		} else {
#define SSE2
#undef ADDP
#undef MULP
#undef MOVAP
$$	}
$$	}
		}
		printf("%.3f Gflops\n", (mode == AVX ? 64e6 : (mode == SSE ? 32e6 : 16e6))
			/ (double)(mytime() - t));
	}
	while(0 < --n);

	return 0;
}
