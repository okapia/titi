#ifndef TITI_TERM_H
#define TITI_TERM_H

extern int setupterm(const char *,int,int *);
extern char *tgetstr(const char *, char **);
extern char *tgoto(const char *, int, int);
extern int tgetent(char *, const char *);
extern int tgetflag(const char *);
extern int tgetnum(const char *);
extern int tputs(const char *, int, int (*)(int));

#endif
