#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHSIZE 101

static struct nlist *hashtab[HASHSIZE]; /* pointer table */

struct nlist { /* table entry: */
	struct nlist *next; /* next entry in chain */
	char *name;         /* defined name */
	char *defn;         /* replacement text */
};

int main(void)
{
	struct nlist *install(char *name, char *defn);
	void undef(char *name);
	install("a", "1");
	install("b", "2");
	install("c", "3");
	undef("a");
	//undef("b");
	//undef("c");
	for (int i = 0; i < HASHSIZE; ++i)
		if (hashtab[i])
			printf("%s - %s\n", hashtab[i]->name, hashtab[i]->defn);

	return 0;
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
	struct nlist *np = hashtab[hashval];

	for ( ; np->next; np = np->next)
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
