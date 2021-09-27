#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define BUFSIZE 100
#define MAXWORD 100

struct tnode {
	char *word;
	int count;
	struct tnode *left;
	struct tnode *right;
} *root;

char buf[BUFSIZE];
int  bufp;
int  nw;
struct tnode **stnode;

int main(void)
{
	int getword(char *word, int lim);
	struct tnode *addtnode(struct tnode *p, char *w);
	void tnodelen(struct tnode *p);
	void addstnode(struct tnode *p);
	void quicksort(struct tnode **arr, int left, int right);
	
	char word[MAXWORD];
	int  nwcp;

	while (getword(word, MAXWORD) != EOF)
		if (isalpha(word[0]))
			root = addtnode(root, word);
	tnodelen(root);
	stnode = malloc(sizeof *stnode * nw);
	nwcp = nw;
	addstnode(root);
	quicksort(stnode, 0, nwcp - 1);

	for (; nw < nwcp; nw++)
		printf("%4d %s\n", stnode[nw]->count, stnode[nw]->word);
	return 0;
}


void swap(struct tnode **arr, int i, int j)
{
	struct tnode *tmp = arr[i];
	arr[i] = arr[j];
	arr[j] = tmp;
}


void quicksort(struct tnode **arr, int left, int right)
{
	if (left >= right)
		return;
	int i = left, j = left + 1;

	for (; j <= right; ++j)
		if (arr[left]->count < arr[j]->count)
			swap(arr, ++i, j);
	swap(arr, left, i);
	quicksort(arr, left, i - 1);
	quicksort(arr, i + 1, right);

}


void tnodelen(struct tnode *p)
{
	if (p != NULL) {
		tnodelen(p->left);
		tnodelen(p->right);
		++nw;
	}
}


void addstnode(struct tnode *p)
{
	if (p != NULL) {
		addstnode(p->left);
		stnode[--nw] = p;
		addstnode(p->right);
	}
}


char *strdup(char *s)
{
	char *p;

	p = malloc(strlen(s)+1);
	if (p != NULL)
		strcpy(p, s);
	return p;
}


struct tnode *addtnode(struct tnode *p, char *w)
{
	int cond;

	if (p == NULL) {
		p = malloc(sizeof(struct tnode));
		p->word = strdup(w);
		p->count = 1;
		p->left  = p->right = NULL;
	}
	else if ((cond = strcmp(w, p->word)) == 0)
		p->count++;
	else if (cond < 0)
		p->left = addtnode(p->left, w);
	else
		p->right = addtnode(p->right, w);
	return p;
}


int getch(void)
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}


void ungetch(int c)
{
	if (bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
}


int getword(char *word, int lim)
{
	int c;
	char *w = word;

	while ((c = getch()) == ' ' || c == '\t')
		;
	
	if (c != EOF)
		*w++ = c;
	if (!isalpha(c) && c != '_') {
		*w = '\0';
		return c;
	}

	for ( ; --lim > 0; w++)
		if (!isalnum(*w = getch()) && *w != '_') {
			ungetch(*w);
			break;
		}
	*w = '\0';
	return word[0];
}
