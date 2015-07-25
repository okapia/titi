#include "term.h"
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
    return tgetstr(name, 0) ? 1 : 0;
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
