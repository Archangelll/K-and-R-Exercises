#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define BUFSIZE 100
#define MAXWORD 100

struct lines {
	int line;
	struct lines *next;
};

struct tnode {
	char *word;
	struct lines *lines;
	struct tnode *left;
	struct tnode *right;
} *root;

char buf[BUFSIZE];
int  bufp = 0;
int  currln = 1;
char *noiseWords[] = {
	"into",
	"onto",
	"just",
	"many",
	"most",
	"the",
	"and",
	"or",
	"at",
	"on",
	"in",
	"so",
	NULL
};

int main(void)
{
	int getword(char *word, int lim);
	struct tnode *addtnode(struct tnode *p, char *w);
	void tnodeprint(struct tnode *p);
	_Bool inStrArr(char **s, char *word);
	
	char word[MAXWORD];

	while (getword(word, MAXWORD) != EOF)
		if (isalpha(word[0]) && !inStrArr(noiseWords, word))
			root = addtnode(root, word);
		else if (word[0] == '\n')
			currln += 1;
	tnodeprint(root);
}


_Bool inStrArr(char **s, char *word)
{
	while (*s != NULL)
		if (strcmp(*s++, word) == 0)
			return 1;
	return 0;
}


void linesprint(struct lines *p)
{
	struct lines *cp = p;
	printf("{ ");
	p = p->next; // p = first node
	while (p != cp) {
		printf("%d, ", p->line);
		p = p->next;
	}
	printf("%d", cp->line);
	printf(" }");
}


void tnodeprint(struct tnode *p)
{
	if (p != NULL) {
		tnodeprint(p->left);
		printf("%s: ", p->word);
		linesprint(p->lines);
		putchar('\n');
		tnodeprint(p->right);
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


struct lines *addline(struct lines *lst)
{
	struct lines *p = malloc(sizeof(struct lines));
	p->line = currln;

	if (lst == NULL)
		p->next = p;
	else if (lst->line != currln){
		p->next = lst->next;
		lst->next = p;
	}
	else
		return lst;
	return p;
}


struct tnode *addtnode(struct tnode *p, char *w)
{
	int cond;

	if (p == NULL) {
		p = malloc(sizeof(struct tnode));
		p->word = strdup(w);
		p->left  = p->right = NULL;
		p->lines = NULL;
		p->lines = addline(p->lines);
	}
	else if ((cond = strcmp(w, p->word)) == 0)
		p->lines = addline(p->lines);
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
