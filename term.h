#ifndef TITI_TERM_H
#define TITI_TERM_H

/* termcap */
extern int setupterm(const char *,int,int *);
extern char *tgetstr(const char *, char **);
extern char *tgoto(const char *, int, int);
extern int tgetent(char *, const char *);
extern int tgetflag(const char *);
extern int tgetnum(const char *);

/* terminfo */
extern char *tigetstr(char *capname);
extern int tigetflag(char *capname);
extern int tigetnum(char *capname);

extern char *tparm(const char *str, ...);
extern int tputs(const char *str, int affcnt, int (*putc)(int));
extern int putp(const char *);

#endif
