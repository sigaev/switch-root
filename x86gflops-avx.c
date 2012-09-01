#include <immintrin.h>

void stress_avx(float *fp) {
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
}
