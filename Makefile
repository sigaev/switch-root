install:
	gcc -O2 -pipe -Xlinker -s -o switch_root switch_root.c
	gcc -O2 -mavx -pipe -Wall -c -o x86gflops-avx.o -std=c99 x86gflops-avx.c
	./pp.pl x86gflops-sse.c | gcc -O2 -pipe -Wall -Xlinker -s -o x86gflops x86gflops-avx.o -x c -std=c99 - x86gflops.c
	install -m755 -t/usr/local/bin pp.pl switch_root x86gflops
