#include <dirent.h>
#include <errno.h>
#include <error.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

static int chroot_plus(bool plus) {
	DIR* dir = opendir(".");
	if(!dir) return errno;
	if(!chdir("/"))
	if(!chroot("mnt"))
	if(plus || !chroot("../../../.."))
	if(!fchdir(dirfd(dir)))
		return closedir(dir) ? errno : 0;
	const int err = errno;
	closedir(dir);
	return err;
}

int main(int argc, char* argv[]) {
	const int err = chroot_plus(index(argv[0], '+'));
	if(setuid(getuid())) return 1;  // Do not report errors while root.
	if(err) {
		error(1, err, "chroot_plus()");
	}
	if(argc < 2) {
		error(1, 0, "Usage: %s PROGRAM [ARGUMENTS]", argv[0]);
	}
	execvp(argv[1], argv + 1);
	error(1, errno, "%s", argv[1]);
	return 1;
}
