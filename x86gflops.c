// PP + C99, 1 Dec 2005 MIT, 29 Aug 2012 Google

#include <error.h>
#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/time.h>

void stress_sse_8(float *);
void stress_sse2_8(float *);
void stress_sse_16(float *);
void stress_sse2_16(float *);

static inline unsigned long long mytime() {
	struct timeval tv;

	return gettimeofday(&tv, NULL)
		? 0ULL : 1000000ULL * tv.tv_sec + tv.tv_usec;
}

static inline void usage() {
	error(1, 0, "Usage: x86gflops sse-8|sse2-8|sse-16|sse2-16|avx [n]");
}

static float fp[128] __attribute__ ((aligned(32))) = { .0f };

int main(int argc, char* argv[]) {
	if(argc < 2 || argc > 3) {
		usage();
	}
	enum {
		SSE_8, SSE2_8, SSE_16, SSE2_16, AVX
	} mode;
	if(!strcasecmp(argv[1], "sse-8")) {
		mode = SSE_8;
	} else if(!strcasecmp(argv[1], "sse2-8")) {
		mode = SSE2_8;
	} else if(!strcasecmp(argv[1], "sse-16")) {
		mode = SSE_16;
	} else if(!strcasecmp(argv[1], "sse2-16")) {
		mode = SSE2_16;
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
		} else if(mode == SSE_8) {
			stress_sse_8(fp);
		} else if(mode == SSE2_8) {
			stress_sse2_8(fp);
		} else if(mode == SSE_16) {
			stress_sse_16(fp);
		} else {
			stress_sse2_16(fp);
		}
		printf("%.3f Gflops\n", (mode == AVX ? 64e6 :
			(mode == SSE_8 || mode == SSE_16 ? 32e6 : 16e6)) / (double)(mytime() - t));
	}
	while(0 < --n);

	return 0;
}
