#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <dirent.h>

#define MAX_PATH 1024

/* dirwalk: apply fcn to all files in dir */
void dirwalk(char *dir, void (*fcn)(char *))
{
	char name[MAX_PATH];
	struct dirent *dp;
	DIR *dfd;

	if ((dfd = opendir(dir)) == NULL) {
		fprintf(stderr, "dirwalk: can't open %s\n", dir);
		return;
	}
	while ((dp = readdir(dfd)) != NULL) {
		if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
			continue; // skip self and parent
		if (strlen(dir) + strlen(dp->d_name) + 2 > sizeof name) // +2 for '/' and '\0'
			fprintf(stderr, "dirwalk: name %s/%s too long\n", dir, dp->d_name);
		else {
			sprintf(name, "%s/%s", dir, dp->d_name);
			(*fcn)(name);
		}
	}
	closedir(dfd);
}


/* fsize: print size of file "name" */
void fsize(char *name)
{
	struct stat stbuf;

	if (stat(name, &stbuf) == -1) {
		fprintf(stderr, "fsize: can't access %s\n", name);
		return;
	}
	if ((stbuf.st_mode & S_IFMT) == S_IFDIR)
		dirwalk(name, fsize);
	// st_* may vary its typedef from machine to machine, thus printf percent format is implementation defined
	// to see the typedef of off_t for example: echo "#include <sys/types.h>" | gcc -E -x c - | grep off_t
	printf("---------------------------------------------- %s\n", name);
	printf("ID of device containing file: %lu\n", stbuf.st_dev);
	printf("Inode number: %lu\n", stbuf.st_ino);
	printf("File type and mode: %u\n", stbuf.st_mode);
	printf("Number of hard links: %lu\n", stbuf.st_nlink);
	printf("User ID of owner: %u\n", stbuf.st_uid);
	printf("Group ID of owner: %u\n", stbuf.st_gid);
	printf("Device ID (if special file): %lu\n", stbuf.st_rdev);
	printf("Total size, in bytes: %ld\n", stbuf.st_size);
	printf("Block size for filesystem I/O: %ld\n", stbuf.st_blksize);
	printf("Number of 512B blocks allocated: %ld\n", stbuf.st_blocks);
	printf("---------------------------------------------- %s\n", name);
}


/* print file sizes */
int main(int argc, char **argv)
{
	if (argc == 1) // default: current directory
		fsize(".");
	else {
		while (--argc > 0)
			fsize(*++argv);
	}
	return 0;
}
