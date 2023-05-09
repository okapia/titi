#include "term.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Jfor f d in "${(@kv)termcap}"; do
printf '  "%s", "%s"\n' $f ${${${(qqqq)d}#\$\'}%\'}
done */

/*
tgetflag
tgetent
tgetnum
tputs
tgoto
tgetstr
*/ 

/* termcap module:
boolcodes
numcodes
setupterm
tgetflag
tgetstr
tgoto
strcodes
tputs
tgetnum
 */

/* terminfo module:
numnames
boolnames
setupterm
tigetnum
putp
tparm
tigetflag
strnames
tigetstr
*/ 

/* some like co, might need to be dynamic */
static struct { char *tc; char *ti; char *val; } db[] = {
    #include "harddb.tbl"
};

int setupterm(const char *term, int fildes, int *errret) {
    return 0;
}

char *tgetstr(const char* id, char **area) {
    int min = 0;
    int max = sizeof(db)/sizeof(*db) - 1;
    int current, compare;

    while (min <= max) {
	current = min + (max - min) / 2;
	compare = strcmp(db[current].tc, id);
	if (!compare)
	    return db[current].val;
	if (compare > 0)
	    max = current - 1;
	else
	    min = current + 1;
    }
    return 0;
}

int tgetent(char *bp, const char *name) {
    return 1;
}

int tgetflag(const char *id) {
    char *flag = tgetstr(id, 0);
    return flag && !strcmp(flag, "yes");
}

int tgetnum(const char *id) {
    char *num = tgetstr(id, 0);
    if (!num)
	return -1;
    char *end;
    long n = strtol(num, &end, 10);
    if (*end)
	return -1;
    return n;
}

char *tgoto(const char *cap, int col, int row) {
    static char output[20];
    int top;
    const char *in;
    char *out = output;
    for (in = cap; *in; in++) {
	if (*in != '%' || !++in) {
	    *out++ = *in;
	    continue;
	}
	switch (*in) {
	    case 'i':
		col++; row++;
		break;
	    case 'p':
		top = *++in == '1' ? row : col;
		break;
	    case 'd':
		out += sprintf(out, "%d", top);
		break;
	}
    }
    *out = '\0';
    return output;
}

int tputs(const char *str, int affcnt, int (*putc)(int)) {
    const char *p;
    if (!str)
	return 1;
    for (p=str; *p; p++) {
	if ((putc)(*p) == EOF)
	    return 1;
    }
    return 0;
}

int putp(const char *str) {
    return tputs(str, 1, putchar);
}

char *tigetstr(char *capname) {
    int i;
    for (i=0; i < sizeof(db)/sizeof(*db); i++) {
	if (!strcmp(db[i].ti, capname))
	    return db[i].val;
    }
    return 0;
}

int tigetflag(char *capname) {
    char *flag = tigetstr(capname);
    return flag && !strcmp(flag, "yes");
}

int tigetnum(char *capname) {
    char *num = tigetstr(capname);
    if (!num)
	return -1;
    char *end;
    long n = strtol(num, &end, 10);
    if (*end)
	return -1;
    return n;
}
