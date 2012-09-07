#include <errno.h>
#include <error.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	static const char root[] = "/mnt";
	const int err = chroot(root) ? errno : 0;
	if(setuid(getuid())) return 1;  // Do not report errors while root.
	if(err) {
		error(1, err, "chroot(%s)", root);
	}
	if(argc < 2) {
		error(1, 0, "Usage: %s PROGRAM [ARGUMENTS]", argv[0]);
	}
	execvp(argv[1], argv + 1);
	error(1, errno, "%s", argv[1]);
	return 1;
}
