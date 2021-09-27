#define myNULL     0
#define myEOF      (-1)
#define myBUFSIZ   1024
#define myOPEN_MAX 20 /* max #files open at once */

typedef struct {
	unsigned my_READ   : 1;  // file open for reading 
	unsigned my_WRITE  : 1;  // file open for writing 
	unsigned my_UNBUF  : 1;  // file is unbuffered 
	unsigned my_EOF    : 1;  // EOF has occurred on this file 
	unsigned my_ERR    : 1;  // error occurred on this file 
} my_flags;

typedef struct my_iobuf {
	int cnt;       /* characters left */
	char *ptr;     /* next character position */
	char *base;    /* location of buffer */
	my_flags flag; /* mode of file access */
	int fd;        /* file descriptor */
} myFILE;
extern myFILE my_iob[myOPEN_MAX];

#define mystdin  (&my_iob[0])
#define mystdout (&my_iob[1])
#define mystderr (&my_iob[2])

int my_fillbuf(myFILE *);
int my_flushbuf(int, myFILE *);

#define myfeof(p)    (((p)->flag.my_EOF) != 0)
#define myferror(p)  (((p)->flag.my_ERR) != 0)
#define myfileno(p)  ((p)->fd)

#define mygetc(p)    (--(p)->cnt >= 0 \
					? (unsigned char) *(p)->ptr++ : my_fillbuf(p))
#define myputc(x, p) (--(p)->cnt >= 0 \
					? *(p)->ptr++ = (x) : my_flushbuf((x), p))

#define mygetchar()  mygetc(mystdin)
#define myputchar(x) myputc((x), mystdout)

#include <fcntl.h>
#include <unistd.h>

#define PERMS 0666 /* RW for owner, group, others */

/* fopen: open file, return file ptr */
myFILE *myfopen(char *name, char *mode)
{
	int fd;
	myFILE *fp;

	if (*mode != 'r' && *mode != 'w' && *mode != 'a')
		return myNULL;
	for (fp = my_iob; fp < my_iob + myOPEN_MAX; fp++)
		if (!fp->flag.my_READ && !fp->flag.my_WRITE)
			break; /* found free slot */
	if (fp >= my_iob + myOPEN_MAX) /* no free slots */
		return myNULL;
	
	if (*mode == 'w')
		fd = creat(name, PERMS);
	else if (*mode == 'a') {
		if ((fd = open(name, O_WRONLY, 0)) == -1)
			fd = creat(name, PERMS);
		lseek(fd, 0L, 2);
	}
	else
		fd = open(name, O_RDONLY, 0);
	if (fd == -1)
		return myNULL;
	fp->fd = fd;
	fp->cnt = 0;
	fp->ptr = fp->base = myNULL;

	fp->flag.my_UNBUF = fp->flag.my_EOF = fp->flag.my_ERR = 0;
	if(*mode == 'r') {
		fp->flag.my_READ  = 1;
		fp->flag.my_WRITE = 0;
	}
	else {
		fp->flag.my_READ  = 0;
		fp->flag.my_WRITE = 1;
	}

	return fp;
}

#include <stdlib.h>

/* _fillbuf: allocate and fill input buffer */
int my_fillbuf(myFILE *fp)
{
	int bufsize;

	if (fp == myNULL || !(fp->flag.my_READ && !fp->flag.my_EOF && !fp->flag.my_ERR))
		return myEOF;
	bufsize = (fp->flag.my_UNBUF) ? 1 : myBUFSIZ;
	if (fp->base == myNULL) /* no buffer yet */
		if ((fp->base = malloc(bufsize)) == NULL) {
			fp->base = myNULL;
			return myEOF;
		}
	fp->ptr = fp->base;
	fp->cnt = read(fp->fd, fp->ptr, bufsize);
	if (--fp->cnt < 0) {
		if (fp->cnt == -1)
			fp->flag.my_EOF = 1;
		else
			fp->flag.my_ERR = 1;
		fp->cnt = 0;
		return myEOF;
	}
	return (unsigned char) *fp->ptr++;
}


int my_flushbuf(int ch, myFILE *fp)
{
	int bufsize;
	if (fp == myNULL || !(fp->flag.my_WRITE && !fp->flag.my_ERR))
		return myEOF;
	bufsize = (fp->flag.my_UNBUF) ? 1 : myBUFSIZ;
	if (fp->base == myNULL) {
		if ((fp->base = malloc(bufsize)) == NULL) {
			fp->base = myNULL;
			return myEOF;
		}
	}
	else if (write(fp->fd, fp->base, fp->ptr - fp->base) != fp->ptr - fp->base) {
		fp->flag.my_ERR = 1;
		fp->cnt = 0;
		return myEOF;
	}
	fp->ptr = fp->base;
	fp->cnt = bufsize - 1;

	return (unsigned char) (*fp->ptr++ = ch);
}


int myfflush(myFILE *fp)
{
	if (fp == myNULL || !(fp->flag.my_WRITE && !fp->flag.my_ERR))
		return myEOF;
	if (fp->base != myNULL && fp->base != fp->ptr) {
		if (write(fp->fd, fp->base, fp->ptr - fp->base) != fp->ptr - fp->base) {
			fp->flag.my_ERR = 1;
			fp->cnt = 0;
			return myEOF;
		}
		fp->ptr = fp->base;
		fp->cnt = (fp->flag.my_UNBUF) ? 1 : myBUFSIZ;
	}
	return 0;
}


int myfclose(myFILE *fp)
{
	if (fp == myNULL || (fp->flag.my_WRITE && myfflush(fp) == myEOF) || close(fp->fd) == -1)
		return myEOF;
	free(fp->base);
	fp->cnt = 0; // if we try to read from a closed file
	fp->flag.my_READ = fp->flag.my_WRITE = 0; // this will prevent myputc, mygetc, myfflush and myfseek from accessing a closed file
	return 0;
}


int myfseek(myFILE *fp, long offset, int origin)
{
	if (fp == myNULL || fp->flag.my_ERR || !(fp->flag.my_READ || fp->flag.my_WRITE) || !(origin >= 0 && origin <= 2))
		return myEOF;
	if (fp->flag.my_READ) {
		if (origin == 1)
			offset -= fp->cnt;
		fp->ptr = fp->base;
		fp->cnt = 0;
	}
	else if (myfflush(fp) == myEOF)
		return myEOF;
	if (lseek(fp->fd, offset, origin) == -1)
		return myEOF;

	fp->flag.my_EOF = 0;
	return 0;
}


myFILE my_iob[myOPEN_MAX] = {
	{0, (char *) 0, (char *) 0, {1, 0, 0, 0, 0}, 0},
	{0, (char *) 0, (char *) 0, {0, 1, 0, 0, 0}, 1},
	{0, (char *) 0, (char *) 0, {0, 1, 1, 0, 0}, 2}
};

#include <time.h>
#include <stdio.h>

int main(void)
{
	clock_t tic = clock();
	myFILE *tst;
	char a[10];
	if ((tst = myfopen("tstinp", "r")) == myNULL)
		return 1;
	
	for (size_t i = 0; i < sizeof a; ++i)
		a[i] = mygetc(tst);
	for (size_t i = 0; i < sizeof a; ++i)
		myputchar(a[i]);
	printf("#%d#\n", myfseek(tst, 1, 0));
	printf("#%d#\n", myfseek(mystdout, 1, 0));
	for (size_t i = 0; i < sizeof a; ++i)
		a[i] = mygetc(tst);
	for (size_t i = 0; i < sizeof a; ++i)
		myputchar(a[i]);
	myfflush(mystdout);

	printf("\nclose tst: %d", myfclose(tst));
	if (mygetc(tst) == EOF)
		printf("\n$\ntst closed\n$\n");

	clock_t toc = clock();
	printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
	return 0;
}
