install:
	gcc -O2 -pipe -Xlinker -s -o switch_root switch_root.c
	./pp.pl x86gflops.c | gcc -O2 -mavx -pipe -Wall -Xlinker -s -o x86gflops -x c -std=c99 -
	install -m755 -t/usr/local/bin pp.pl switch_root x86gflops
