#include <cstdio>
#include <ctime>
#include <immintrin.h>

static inline unsigned long long mytime() {
  timespec tp;
  return clock_gettime(CLOCK_MONOTONIC, &tp)
      ? 0 : 1000000000ULL * tp.tv_sec + tp.tv_nsec;
}

const static unsigned sz = 128 * 1024 * 1024U;
static float data[sz / 4] __attribute__((aligned(32)));

int main() {
  for (unsigned i = 0; i < sz / 4; ++i) {
    data[i] = 1e-15f * i;
  }
  unsigned long long t = 0;
  for (int j = 0; j < 9; ++j) {
    if (3 == j) {
      t = mytime();
    }
    __m256 a = _mm256_load_ps(data);
    for (unsigned i = 8; i < sz / 4; i += 8) {
      a = _mm256_add_ps(a, _mm256_load_ps(data + i));
    }
    _mm256_store_ps(data, a);
  }
  printf("%.3f Gb/s\n", 6. * sz / (mytime() - t + 1e-99));
  return 0;
}
