// PP + C99, 1 Dec 2005 MIT, 29 Aug 2012 Google, 21 Apr 2020 Snap.

#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/time.h>

void stress_sse_8(float *);
void stress_sse2_8(float *);
void stress_sse_16(float *);
void stress_sse2_16(float *);
void stress_avx(float *);
void stress_avx_512(float *);

static inline unsigned long long mytime() {
  struct timeval tv;

  return gettimeofday(&tv, NULL) ? 0ULL : 1000000ULL * tv.tv_sec + tv.tv_usec;
}

static inline void usage() {
  error(1, 0, "Usage: x86gflops sse-8|sse2-8|sse-16|sse2-16|avx|avx-512 [n]");
}

static float fp[256] __attribute__((aligned(64))) = {.0f};

int main(int argc, char *argv[]) {
  if (argc < 2 || argc > 3) {
    usage();
  }
  enum { SSE_8, SSE2_8, SSE_16, SSE2_16, AVX, AVX_512 } mode;
  if (!strcasecmp(argv[1], "sse-8")) {
    mode = SSE_8;
  } else if (!strcasecmp(argv[1], "sse2-8")) {
    mode = SSE2_8;
  } else if (!strcasecmp(argv[1], "sse-16")) {
    mode = SSE_16;
  } else if (!strcasecmp(argv[1], "sse2-16")) {
    mode = SSE2_16;
  } else if (!strcasecmp(argv[1], "avx")) {
    mode = AVX;
  } else if (!strcasecmp(argv[1], "avx-512")) {
    mode = AVX_512;
  } else {
    usage();
  }
  int n = argc == 3 ? atoi(argv[2]) : 0;
  do {
    unsigned long long t = mytime();
    if (mode == AVX_512) {
      stress_avx_512(fp);
    } else if (mode == AVX) {
      stress_avx(fp);
    } else if (mode == SSE_8) {
      stress_sse_8(fp);
    } else if (mode == SSE2_8) {
      stress_sse2_8(fp);
    } else if (mode == SSE_16) {
      stress_sse_16(fp);
    } else {
      stress_sse2_16(fp);
    }
    printf("%.3f Gflops\n",
           (mode == AVX_512
                ? 256e6
                : (mode == AVX
                       ? 64e6
                       : (mode == SSE_8 || mode == SSE_16 ? 32e6 : 16e6))) /
               (double)(mytime() - t));
  } while (0 < --n);

  return 0;
}
