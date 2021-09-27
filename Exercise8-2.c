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
	//int flag;
	my_flags flag; /* mode of file access */
	int fd;        /* file descriptor */
} myFILE;
extern myFILE my_iob[myOPEN_MAX];

#define mystdin  (&my_iob[0])
#define mystdout (&my_iob[1])
#define mystderr (&my_iob[2])
/*
enum my_flags {
	my_READ   = 01,   // file open for reading 
	my_WRITE  = 02,   // file open for writing 
	my_UNBUF  = 04,   // file is unbuffered 
	my_EOF    = 010,  // EOF has occurred on this file 
	my_ERR    = 020   // error occurred on this file 
};
*/
int my_fillbuf(myFILE *);
int my_flushbuf(int, myFILE *);

#define myfeof(p)    (((p)->flag.my_EOF) != 0)
#define myferror(p)  (((p)->flag.my_ERR) != 0)
//#define myfeof(p)    (((p)->flag & my_EOF) != 0)
//#define myferror(p)  (((p)->flag & my_ERR) != 0)
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
		//if ((fp->flag & (my_READ | my_WRITE)) == 0)
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
	fp->base = myNULL;

	fp->flag.my_UNBUF = fp->flag.my_EOF = fp->flag.my_ERR = 0;
	if(*mode == 'r') {
		fp->flag.my_READ  = 1;
		fp->flag.my_WRITE = 0;
	}
	else {
		fp->flag.my_READ  = 0;
		fp->flag.my_WRITE = 1;
	}

	//fp->flag = (*mode == 'r') ? my_READ : my_WRITE;
	return fp;
}

#include <stdlib.h> // for malloc

/* _fillbuf: allocate and fill input buffer */
int my_fillbuf(myFILE *fp)
{
	int bufsize;

	if (!(fp->flag.my_READ && !fp->flag.my_EOF && !fp->flag.my_ERR))
	//if ((fp->flag & (my_READ | my_EOF | my_ERR)) != my_READ)
		return myEOF;
	bufsize = (fp->flag.my_UNBUF) ? 1 : myBUFSIZ;
	//bufsize = (fp->flag & my_UNBUF) ? 1 : myBUFSIZ;
	if (fp->base == myNULL) /* no buffer yet */
		if ((fp->base = malloc(bufsize)) == NULL)
			return myEOF;
	fp->ptr = fp->base;
	fp->cnt = read(fp->fd, fp->ptr, bufsize);
	if (--fp->cnt < 0) {
		if (fp->cnt == -1)
			fp->flag.my_EOF = 1;
			//fp->flag |= my_EOF;
		else
			fp->flag.my_ERR = 1;
			//fp->flag |= my_ERR;
		fp->cnt = 0;
		return myEOF;
	}
	return (unsigned char) *fp->ptr++;
}

myFILE my_iob[myOPEN_MAX] = {
	{0, (char *) 0, (char *) 0, {1, 0, 0, 0, 0}, 0},
	{0, (char *) 0, (char *) 0, {0, 1, 0, 0, 0}, 1},
	{0, (char *) 0, (char *) 0, {0, 1, 1, 0, 0}, 2}
	//{0, (char *) 0, (char *) 0, my_READ, 0},
	//{0, (char *) 0, (char *) 0, my_WRITE, 1},
	//{0, (char *) 0, (char *) 0, my_WRITE & my_UNBUF, 2}
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
	write(1, a, 10);

	clock_t toc = clock();
	printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
	return 0;
}
