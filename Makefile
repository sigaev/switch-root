install:
	gcc -O2 -pipe -Xlinker -s -o chroot-mnt chroot-mnt.c
	gcc -O2 -pipe -Xlinker -s -o switch-root switch-root.c
	gcc -O2 -mavx -pipe -Wall -c -o x86gflops-avx.o -std=c99 x86gflops-avx.c
	./pp.pl x86gflops-sse.c | gcc -O2 -msse2 -pipe -Wall -Xlinker -s -o x86gflops x86gflops-avx.o -x c -std=c99 - x86gflops.c
	install -m4711 -t/usr/local/bin chroot-mnt
	install -m755 -t/usr/local/bin pp.pl switch-root x86gflops
