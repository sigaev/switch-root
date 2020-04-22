#include <immintrin.h>

void stress_avx_512(float *fp) {
  __m512 a0 = _mm512_load_ps(fp);
  __m512 a1 = _mm512_load_ps(fp + 16);
  __m512 a2 = _mm512_load_ps(fp + 32);
  __m512 a3 = _mm512_load_ps(fp + 48);
  __m512 a4 = _mm512_load_ps(fp + 64);
  __m512 a5 = _mm512_load_ps(fp + 80);
  __m512 a6 = _mm512_load_ps(fp + 96);
  __m512 a7 = _mm512_load_ps(fp + 112);
  __m512 a8 = _mm512_load_ps(fp + 128);
  __m512 a9 = _mm512_load_ps(fp + 144);
  __m512 aa = _mm512_load_ps(fp + 160);
  __m512 ab = _mm512_load_ps(fp + 176);
  __m512 ac = _mm512_load_ps(fp + 192);
  __m512 ad = _mm512_load_ps(fp + 208);
  __m512 ae = _mm512_load_ps(fp + 224);
  __m512 af = _mm512_load_ps(fp + 240);
  for (int i = 0; i < 500000000; ++i) {
    a0 = _mm512_fmadd_ps(a0, a0, a0);
    a1 = _mm512_fmadd_ps(a1, a1, a1);
    a2 = _mm512_fmadd_ps(a2, a2, a2);
    a3 = _mm512_fmadd_ps(a3, a3, a3);
    a4 = _mm512_fmadd_ps(a4, a4, a4);
    a5 = _mm512_fmadd_ps(a5, a5, a5);
    a6 = _mm512_fmadd_ps(a6, a6, a6);
    a7 = _mm512_fmadd_ps(a7, a7, a7);
    a8 = _mm512_fmadd_ps(a8, a8, a8);
    a9 = _mm512_fmadd_ps(a9, a9, a9);
    aa = _mm512_fmadd_ps(aa, aa, aa);
    ab = _mm512_fmadd_ps(ab, ab, ab);
    ac = _mm512_fmadd_ps(ac, ac, ac);
    ad = _mm512_fmadd_ps(ad, ad, ad);
    ae = _mm512_fmadd_ps(ae, ae, ae);
    af = _mm512_fmadd_ps(af, af, af);
  }
  _mm512_store_ps(
      fp,
      _mm512_add_ps(
          _mm512_mul_ps(
              _mm512_mul_ps(_mm512_mul_ps(a0, a1), _mm512_add_ps(a2, a3)),
              _mm512_add_ps(_mm512_mul_ps(a4, a5), _mm512_add_ps(a6, a7))),
          _mm512_add_ps(
              _mm512_mul_ps(_mm512_mul_ps(a8, a9), _mm512_add_ps(aa, ab)),
              _mm512_add_ps(_mm512_mul_ps(ac, ad), _mm512_add_ps(ae, af)))));
}
