install:
	gcc -O2 -Xlinker -s -o switch_root switch_root.c
	install -m755 -t/usr/local/bin switch_root
