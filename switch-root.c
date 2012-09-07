#include <sys/mount.h>
#include <linux/fs.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[])
{
	char buf[16], *p;
	p = getcwd(buf, sizeof(buf));
	if(mount(".", "/", NULL, MS_MOVE, NULL) || (p && rmdir(p), chroot(".")) || chdir("/"))
		return 1;
	execve(argv[1], argv + 1, envp);
	return 2;
}
