install:
	gcc -O2 -pipe -Xlinker -s -static -o chroot+ chroot+.c
	gcc -O2 -pipe -Xlinker -s -o switch-root switch-root.c
	gcc -O2 -mavx -pipe -Wall -c -o x86gflops-avx.o -std=c99 x86gflops-avx.c
	./pp.pl x86gflops-sse.c | gcc -O2 -msse2 -pipe -Wall -Xlinker -s -o x86gflops x86gflops-avx.o -x c -std=c99 - x86gflops.c
	install -m4711 -t/usr/local/bin chroot+
	ln -sfn chroot+ /usr/local/bin/chroot-
	install -m755 -t/usr/local/bin dev-rsync pp.pl switch-root x86gflops
