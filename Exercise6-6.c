#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define HASHSIZE 101
#define BUFSIZE  100
#define MAXWORD  100

char buf[BUFSIZE];
int bufp;
static struct nlist *hashtab[HASHSIZE]; /* pointer table */

struct nlist { /* table entry: */
	struct nlist *next; /* next entry in chain */
	char *name;         /* defined name */
	char *defn;         /* replacement text */
};

int main(void)
{
	/* this program does not handle line continuation characters */
	struct nlist *install(char *name, char *defn);
	struct nlist *lookup(char *s);
	void undef(char *name);
	int getch(void);
	void ungetch(int c);
	int getpre(char *word, int lim);
	int getdefn(char *word, int lim);
	void printHashTab(char s);

	int c, pc;
	char word[MAXWORD];
	char name[MAXWORD];
	char defn[MAXWORD];
	
	while ((c = getch()) != EOF)
		if (c == '/') {
			putchar('/');
			if ((c = getch()) == '*') {
				putchar('*');
				for (pc = 0; ((c = getch()) != '/' || pc != '*') && c != EOF; pc = putchar(c))
					;
			}
			else if (c == '/')
				do {
					putchar(c);
				} while ((c = getch()) != '\n' && c != EOF);
			putchar(c);
		}
		else if (c == '"' || c == '\'') {
			int q = putchar(c);
			pc = 0;
			for (putchar(c = getch()); (c != q || pc == '\\') && c != EOF; putchar(c = getch()))  
				if (pc == '\\' && c == '\\')
 					pc = 0; /* '\\' */      
        		else
        			pc = c;
		}
		else if (c == '#') {  // always read till '\n' or EOF
			c = getpre(word, MAXWORD);
			if (strcmp(word, "define") == 0 || strcmp(word, "undef") == 0) {
				c = getpre(name, MAXWORD);
				if (isalpha(name[0]) || name[0] == '_') {
					//printf("=========================================\n");
					if (word[0] == 'd') {  // define
						c = getdefn(defn, MAXWORD);
						install(name, defn);
						//printHashTab('i');
					}
					else {  // undef
						undef(name); // "#undef ALPHANUM_@xxx0-@#$9874(5)" == "#undef ALPHANUM_"
						//printHashTab('u');
					}
				}
			}
			while (c != '\n' && c != EOF)
				c = getch();
		}
		else if (isalpha(c) || c == '_') {
			ungetch(c);
			getpre(word, MAXWORD);
			struct nlist *np = lookup(word);
			printf("%s", np ? np->defn : word);
		}
		else
			putchar(c);

	return 0;
}


void printHashTab(char s)
{
	struct nlist *np;
	printf((s == 'i') ? "install\n" : "undef\n");
	for (int i = 0; i < HASHSIZE; ++i)
		for (np = hashtab[i]; np != NULL; np = np->next)
			printf("%d: %s - %s\n", i, np->name, np->defn);
}


/* hash: form hash value for string s */
unsigned hash(char *s)
{
	unsigned hashval;

	for (hashval = 0; *s != '\0'; s++)
		hashval = *s + 31 * hashval;
	return hashval % HASHSIZE;
}


/* lookup: look for s in hashtab */
struct nlist *lookup(char *s)
{
	struct nlist *np;

	for (np = hashtab[hash(s)]; np != NULL; np = np->next)
		if (strcmp(s, np->name) == 0)
			return np;  /* found */
	return NULL;        /* not found */
}


void undef(char *name)
{
	unsigned hashval = hash(name);
	if (hashtab[hashval] == NULL)
		return;

	for (struct nlist *np = hashtab[hashval]; np->next; np = np->next)
		if (strcmp(np->next->name, name) == 0) {
			free(np->next->name);
			free(np->next->defn);
			np->next = np->next->next;
			free(np->next);
			return;
		}
	if (strcmp(hashtab[hashval]->name, name) == 0) {
		free(hashtab[hashval]->name);
		free(hashtab[hashval]->defn);
		hashtab[hashval] = hashtab[hashval]->next;
		free(hashtab[hashval]);
	}
}


/* make a duplicate of s*/
char *mystrdup(char *s)
{
	char *p;

	p = malloc(strlen(s)+1);
	if (p != NULL)
		strcpy(p, s);
	return p;
}


/* install: put (name, defn) in hashtab */
struct nlist *install(char *name, char *defn)
{
	struct nlist *np;
	unsigned hashval;

	if ((np = lookup(name)) == NULL) {  /* not found */
		np = malloc(sizeof(*np));
		if (np == NULL || (np->name = mystrdup(name)) == NULL)
			return NULL;
		hashval = hash(name);
		np->next = hashtab[hashval];
		hashtab[hashval] = np;
	}
	else
		free(np->defn);  /* free previous defn */
	if ((np->defn = mystrdup(defn)) == NULL)
		return NULL;
	return np;
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


int getpre(char *word, int lim)
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

	for ( ; --lim > 0; *w++ = c)
		if (!isalnum(c = getch()) && c != '_') {
			ungetch(c);
			break;
		}
	*w = '\0';
	return word[0];
}


int getdefn(char *word, int lim)
{
	/* this function allows non white spaces characters to appear between the name and the definition, for example:
	 * "#define MAXW@abc" == "define MAXW @abc"
	 * "#define MAXW(abc" == "define MAXW (abc"
	 */
	int c;
	word[0] = '\0';

	while ((c = getch()) == ' ' || c == '\t')
		;

	if (c == '\n' || c == EOF)
		return c;
	*word++ = c; // lim must be >= 1

	for ( ; (c = getch()) != '\n' && c != EOF && --lim > 0; *word++ = c)
		;
	*word = '\0';

	if (lim <= 0)
		printf("getdefn: too many characters\n");
	return c;
}
