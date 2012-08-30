#ifndef SSE2
	#define MOVAP	"movaps"
	#define ADDP	"addps"
	#define MULP	"mulps"
	#define PNAME	"sse"
#else
	#define MOVAP	"movapd"
	#define ADDP	"addpd"
	#define MULP	"mulpd"
	#define PNAME	"sse2"
#endif

#define id_string "x86" PNAME ".c (Thu Dec  1 16:54:58 2005) Dmitry Sigaev http://sigaev.com"
// PP + C99, 1 Dec 2005 MIT

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

inline unsigned long long mytime(void)
{
	struct timeval tv;

	return gettimeofday(&tv, NULL)
		? 0ULL : 1000000ULL * tv.tv_sec + tv.tv_usec;
}

int main(int argc, char* argv[])
{
	static int n = 0;
	static const float fp[4] __attribute__ ((aligned(16))) = { 0.0f };
	unsigned long long t;

	if(2 == argc)
		n = atoi(argv[1]);
	puts(id_string);
	do
	{
		t = mytime();
	__asm__ volatile(
$$	local $" = "\n\t\t";
$$	my $a = '$';
		"pushal\n\t"
		@{[map 'MOVAP "'."	(%0), %%xmm$_\n\t".'"', 0 .. 7]}
		"movl	${a}4999999, %%eax\n\t"
		".align	16\n\t"
		"label:\n\t"
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
		"jnc	label\n\t"
		"popal\n\t"
		:
		: "r" (fp)
		: "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7"
	);
		printf("%.3f Gflops\n",
#ifndef SSE2
			32e6
#else
			16e6
#endif
			/ (double)(mytime() - t));
	}
	while(0 < --n);

	return 0;
}
