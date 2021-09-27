#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define MAXPARLST  100 /* maximum number of characters that a parameter list can have */
#define BUFSIZE    100 /* buf for ungetch, it should be >= MAXPARLST                  */
#define MAXWORD    100
#define MAXTOKEN   100

struct tnode {
	char *word;
	int count;
	struct tnode *left;
	struct tnode *right;
};

struct group {
	struct tnode *root;
	struct group *left;
	struct group *right;
} *root;

char *iq[] = {
    "char",
	"_Bool",
	"int",
	"float",
	"double",
    "void",
	"signed",
	"unsigned",
	"short",
	"long",
	"static",
    "const",
	"struct",
	 NULL
};
enum {NAME=-100, PARENS, BRACKETS, FUNCPROTOTYPE, FUNCDEF, PARSEERROR, GDTREAD, GDTDREAD};

int dcl(void);
int getporb(void);
_Bool addtree(void);
void printenum(int c, char *whr);

int   tokentype;            /* type of last token           */
char  token[MAXTOKEN];      /* pointer type qualifier       */
char  name[MAXTOKEN];       /* identifier name              */
char  buf[BUFSIZE];         /* buffer for ungetch           */
int   bufp = 0;             /* buf current position         */
int   fnc = 6;              /* first n characters           */

int main(int argc, char *argv[])
{
	if (argc > 1)
		fnc = atoi(argv[1]);

	void treeprint(struct group *);
	int getDataType(void);

	int c;
	printf("DEBUG =========================================\n");
	while ((c = getDataType()) != EOF) {
		if (c == GDTDREAD) {
			c = dcl();
			printenum(c, "MN");
			addtree();
		}
		else if (c == GDTREAD) {
			c = getporb();
			printenum(c, "MY");
			addtree();
		}
		while (c == ',') {
			name[0] = '\0';
			c = dcl();
			printenum(c, "MC");
			addtree();
		}
		name[0] = '\0';
	}
	printf("GROUPS ========================================\n");
	treeprint(root);

	return 0;
}


void printenum(int c, char *whr)
{
	// enum {NAME=-100, PARENS, BRACKETS, FUNCPROTOTYPE, FUNCDEF, FUNCVOID, PARSEERROR, GDTREAD, GDTDREAD};
	if (c == -100)
		printf("%s: [%d - %s] - %s\n", whr, c, "NAME", name);
	else if (c == -99)
		printf("%s: [%d - %s] - %s\n", whr, c, "PARENS", name);
	else if (c == -98)
		printf("%s: [%d - %s] - %s\n", whr, c, "BRACKETS", name);
	else if (c == -97)
		printf("%s: [%d - %s] - %s\n", whr, c, "FUNCPROTOTYPE", name);
	else if (c == -96)
		printf("%s: [%d - %s] - %s\n", whr, c, "FUNCDEF", name);
	else if (c == -95)
		printf("%s: [%d - %s] - %s\n", whr, c, "PARSEERROR", name);
	else if (c == -94)
		printf("%s: [%d - %s] - %s\n", whr, c, "GDTREAD", name);
	else if (c == -93)
		printf("%s: [%d - %s] - %s\n", whr, c, "GDTDREAD", name);
	else
		printf("%s: [%d - %c] - %s\n", whr, c, c, name);
}


_Bool iniq(char **iq, char *word)
{
	while (*iq != NULL)
		if (strcmp(*iq++, word) == 0)
			return 1;
	return 0;
}


void tnodeprint(struct tnode *p)
{
	if (p != NULL) {
		tnodeprint(p->left);
		printf("%4d %s\n", p->count, p->word);
		tnodeprint(p->right);
	}
}


void treeprint(struct group *p)
{
	if (p != NULL) {
		treeprint(p->left);
		
		printf("group ");
		for (int i = 0; i < fnc; i++)
			putchar(p->root->word[i]);
		printf(":\n");
		tnodeprint(p->root);
		
		treeprint(p->right);
	}
}


char *strdup(char *s)
{
	char *p;

	p = (char *) malloc(strlen(s)+1);
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
		p->left = p->right = NULL;
	}
	else if ((cond = strcmp(w, p->word)) == 0)
		p->count++;
	else if (cond < 0)
		p->left = addtnode(p->left, w);
	else
		p->right = addtnode(p->right, w);
	return p;
}


int slicestrcmp(char *s, char *t)
{
	char *slcs = malloc(strlen(s)+1);
	strncpy(slcs, s, fnc);
	char *slct = malloc(strlen(t)+1);
	strncpy(slct, t, fnc);
	return strcmp(slcs, slct);
}


struct group *addgroup(struct group *p, char *w)
{
	int cond;

	if (p == NULL) {
		p = malloc(sizeof(struct group));
		p->root = addtnode(p->root, w);
		p->left = p->right = NULL;
	}
	else if ((cond = slicestrcmp(w, p->root->word)) == 0)
		p->root = addtnode(p->root, w);
	else if (cond < 0)
		p->left = addgroup(p->left, w);
	else
		p->right = addgroup(p->right, w);
	return p;
}


_Bool addtree(void)
{
	if ((int)strlen(name) >= fnc) {
		root = addgroup(root, name);
		return 1;
	}
	return 0;

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


_Bool ungets(char *s, char *nc)
{
	while (s < nc && bufp < BUFSIZE)
		buf[bufp++] = *--nc;
	if (nc > s)
		return 1;
	return 0;
}


int dsoc(int q)
{
	int c, pc = 0;
	for (c = getch(); (c != q || pc == '\\') && c != EOF; c = getch())  
		if (pc == '\\' && c == '\\')
 			pc = 0; /* '\\' */      
        else
        	pc = c;
	if (c == q)
		c = getch();
	return c;
}


int dcom(void)
{
	int c, pc = 0;

	if ((c = getch()) == '/')
		do {
			while ((c = getch()) != '\n' && c != EOF) {
				if (c != ' ' && c != '\t')
					pc = c;
			}
		} while (pc == '\\' && c != EOF);
	else if (c == '*')
		for (c = getch(); (c != '/' || pc != '*') && c != EOF; c = getch())
			pc = c;
	else {
		ungetch(c);
		return 1; /* it's not a comment */
	}
	if (c == '/') { /* skips '\n' on multi line comments */
		if ((c = getch()) == '\n')
			c = getch();
	}
	else if (c == '\n') /* skips '\n' on single line comments */
		c = getch();
	return c;
}


int dprp(void)
{
	int c, pc = 0;
	while ((c = getch()) == ' ' || c == '\t')
		;
	
	if (c == 'i' && (c = getch()) == 'f') { /* assumes that every preprocessor directive followed by "if" is conditional */
		int i;
		char sendif[] = "endif";
		
		while ((c = getch()) != '\n' && c != EOF) /* endif must be at a different line */
			;
		while (pc != '#' && c != EOF)
			for (i = 0; sendif[i] != '\0' && c != EOF; c = getch()) {
				if (c == sendif[i]) {
					++i;
					continue;
				}
				else if (!isspace(c))
					pc = c;
				i = 0;
			}
		if (sendif[i] == '\0' && pc == '#' && c == '\n') /* skips first '\n' */
			c = getch();
	}
	else {
		while (c != '\n' && c != EOF)
			c = getch();
		if (c == '\n') /* skips first '\n' */
			c = getch();
	}
	return c;
}


int getword(char *word, int lim)
{
	int c;
	char *w = word;

	while ((c = getch()) == ' ' || c == '\t')
		;
	/* it's faster to remove all line continuation characters before calling getword */
	while (1) {
		if (c == '\"' || c == '\'')
			c = dsoc(c);
		else if (c == '#')
			c = dprp();
		else if (c == '/') {
			if ((c = dcom()) == 1) {
				c = '/';
				break;
			}
		}
		else
			break;
	}
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


int getDataType(void)
{
	/*
	getDataType passes over empty declarations. 
	return GDTREAD    ---> getDataType read the variable name
	return GDTDREAD   ---> getDataType didn't read the variable name
	return EOF        ---> EOF
	*/
	_Bool isvoid = 0;
	int c;
	char word[MAXWORD];

	do {
		while ((c = getword(word, MAXWORD)) != EOF && !iniq(iq, word))
			;
		if (c != EOF) {
			if (strcmp("struct", word) == 0) {
				int n = 1;
				
				if (isalpha(c = getword(word, MAXWORD))) // structure tag
					c = getword(word, MAXWORD);
				if (c == '{') {
					do {
						while ((c = getch()) != '}' && c != EOF)
							if (c == '{')
								++n;
					} while (c != EOF && --n > 0);	
					if (n > 0)
						return EOF;
					c = getword(word, MAXWORD);
				}
			}
			else {
				if (c == 'v') // void
					isvoid = 1;
				do {
					c = getword(word, MAXWORD);
				} while (iniq(iq, word));
			}

			if (c == '(' || c == '*') {
				ungetch(c);
				return GDTDREAD;
			}
			else if (isalpha(c)) {
				strcpy(name, word);
				return GDTREAD;
			}
			else if (isvoid) {
				ungetch(c); // ')'
				strcpy(name, "void parameter");
				return GDTREAD;
			}
		}
	} while (c != EOF);
	return EOF;
}


/* since it's impossible to know in advance if PARSEERROR will occour, gettoken will add parameter list identifiers to the tree */
int gettoken(void) /* return next token */
{
	int  c;
	char *p = token;
	tokentype = 0;
	while ((c = getch()) == ' ' || c == '\t')
		;
	if (c == '(') {
		while ((c = getch()) == ' ' || c == '\t')
			;
		if (c == ')')
			return tokentype = PARENS;
		else if (name[0] != '\0') { /* parameter list */
			ungetch(c);

			char plbuf[MAXPARLST], *plbufp; /* does not parse variables inside functions prototype */
			for (plbufp = plbuf; (c = getch()) != '{' && c != ';' && c != EOF; plbufp++)
				*plbufp = c;
			if (c == EOF)
				return tokentype = PARSEERROR;
			else if (c == ';')
				return tokentype = FUNCPROTOTYPE;
			else {
				*plbufp = '\0';
				if (ungets(plbuf, plbufp) == 1)
					return tokentype = PARSEERROR;
				do {
					char currname[MAXTOKEN];
					strcpy(currname, name);
					name[0] = '\0';

					if ((c = getDataType()) == GDTREAD) {
						c = getporb();
						printenum(c, "GY");
						addtree();
					}
					else if (c == GDTDREAD) {
						c = dcl();
						printenum(c, "GN");
						addtree();
					}
					if (c == PARSEERROR || c == EOF)
						return tokentype = PARSEERROR;

					strcpy(name, currname);

					if (tokentype == ',')
						continue;
					break;
				} while(1);

				if (tokentype != ')') {
					ungetch(tokentype);
					return tokentype = PARSEERROR;
				}
				return tokentype = FUNCDEF;
			}
		}
		else {
			ungetch(c);
			return tokentype = '(';
		}
	}
	else if (c == '[') {
		int n = 1;
		do {
			while ((c = getch()) != ']' && c != EOF)
				if (c == '[')
					++n;
		} while (c != EOF && --n > 0);
				
		if (n > 0)
			return tokentype = EOF;
		return tokentype = BRACKETS;
	}
	else if (isalpha(c)) {
		for (*p++ = c; isalnum(c = getch()); *p++ = c)
			;
		*p = '\0';
		ungetch(c);
		return tokentype = NAME;
	}
	else
		return tokentype = c;
}


int dirdcl(void);

/* dcl: parse a declarator */
int dcl(void)
{
	int c;

	while (1) {
		for ( ; (c = gettoken()) == '*'; ) /* count *'s */
			;
		if (c == EOF || c == PARSEERROR)
			return PARSEERROR;
		if (c != NAME || strcmp(token, "const") != 0) /* const is the only pointer type qualifier learned so far */
			break;
	}
	return dirdcl();
}


/* dirdcl: parse a direct declarator */
int dirdcl(void)
{
	if (tokentype == '(') {     /* ( dcl ) */
		if (dcl() == PARSEERROR || tokentype != ')')
			return PARSEERROR;
	}
	else if (tokentype == NAME) /* variable name */
		strcpy(name, token);
	else
		return PARSEERROR;

	return getporb();
}


int getporb(void)
{
	int type;

	/* a function cannot return another function */
	type = gettoken();
	if (type == PARENS || type == BRACKETS)
		while ((type = gettoken()) == BRACKETS)
			;
	return type;
}
