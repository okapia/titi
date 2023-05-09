#include "term.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* See "Parameterized Strings" in terminfo(5) for details */

#define STACKSIZE	20
#define npush(X) \
    do { \
	uintptr_t tmp = X; \
        if (stackptr < STACKSIZE) \
	    stack[stackptr++] = tmp; \
    } while(0)
#define npop() (int) (stackptr > 0 ? stack[--stackptr] : 0)
#define spop() (char *) (stackptr > 0 ? stack[--stackptr] : 0)

char *
tparm(const char *str, ...)
{
    static char buf[256];

    va_list varg;
    uintptr_t param[9];
    int vars[26 * 2];
    int sign, depth, prec, width, flags[6], x;
    int stackptr = 0;
    uintptr_t stack[STACKSIZE];
    char *endptr, *bufptr = buf;
    char spec[11], *flagch = "+-0 #:", *flag, *fmt;
    *spec = '%';

    va_start(varg, str);
    for (x = 0; x < 9; x++)
	param[x] = va_arg(varg, uintptr_t);
    va_end(varg);

    while (*str) {
	if (*str != '%') {
	    *(bufptr++) = *str++;
	    continue;
	}
	++str;

	width = 0;
	prec = -1;
	fmt = spec + 1;
	/* copy only one of each flag as spec has finite size */
	memset(flags, 0, sizeof(flags));
	flags[5] = 1; /* don't output ':' */
	flag = flagch + 2; /* skip '-' and '+' on first iteration */
	while (*str && (flag = strchr(flag, *str))) {
	    if (!flags[flag - flagch]) {
		flags[flag - flagch] = 1;
		*fmt++ = *str;
	    }
	    str++;
	    flag = flagch;
	}

	if (isdigit(*str)) {
	    width = strtoul(str, &endptr, 0);
	    str = endptr;
	}
	*fmt++ = '*';

	if (*str == '.') {
	    str++;
	    if (*str >= '1' && *str <= '9') {
		prec = strtoul(str, &endptr, 0);
		str = endptr;
	    } else
		prec = 0;
	    if (prec >= 0) *fmt++ = '.', *fmt++ = '*';
	}
	fmt[1] = '\0';

	switch ((*fmt = *str)) {
	    case '%':
		*(bufptr++) = *(str++);
		break;
	    case 'd':
	    case 'o':
	    case 'x':
	    case 'X':
	    case 's':
		if (prec >= 0)
                    bufptr += sprintf(bufptr, spec, width, prec, npop());
		else
                    bufptr += sprintf(bufptr, spec, width, npop());
		break;
	    case 'c':
		*(bufptr++) = (char) npop();
		break;
	    case 'i':
		param[0]++;
		param[1]++;
		break;
	    case 'p':
		str++;
		if ('1' <= *str && *str <= '9')
		    npush(param[*str - '1']);
		break;
	    case 'P':
		x = (int)*++str;
		x = (x | 32) - 'a' + 4*!!(x & 32);
		if (x >= 0 && x < 52)
		    vars[x] = npop();
		break;
	    case 'g':
		x = (int)*++str;
		x = (x | 32) - 'a' + 4*!!(x & 32);
		if (x >= 0 && x < 52)
		    npush(vars[x]);
		break;
	    case '\'': /* string literal */
		str++;
		npush(*str);
		str++;
		break;
	    case '{': /* numeric literal */
		x = strtoul(++str, &endptr, 10);
		str = endptr;
		npush(x);
		break;
	    case 'l':
		npush(strlen(spop()));
		break;

	    /* binary operators */
	    case '+':
		x = npop();
		npush(x + npop());
		break;
	    case '-':
		x = npop();
		npush(npop() - x);
		break;
	    case '*':
		x = npop();
		npush(x * npop());
		break;
	    case '/':
		if ((x = npop())) /* avoid division by zero */
		    npush(npop() / x);
		break;
	    case 'm':
		if ((x = npop()))
		    npush(npop() % x);
		break;
	    case '&':
		x = npop();
		npush(x & npop());
		break;
	    case '|':
		x = npop();
		npush(x | npop());
		break;
	    case '^':
		x = npop();
		npush(x ^ npop());
		break;
	    case '=':
		x = npop();
		npush(npop() == x);
		break;
	    case '>':
		x = npop();
		npush(npop() > x);
		break;
	    case '<':
		x = npop();
		npush(npop() < x);
		break;
	    case 'A':
		npush(npop() && npop());
		break;
	    case 'O':
		npush(npop() || npop());
		break;
	    /* unary operators */
	    case '!':
		npush(!npop());
		break;
	    case '~':
		npush(~npop());
		break;

	    case 't':
		if ((x = npop()))
		    break;
		/* search for %e or %; allowing for nesting */
		for (depth = 0, str++; *str; str++) {
		    if (*str != '%')
			continue;
		    if (!*++str || (*str == 'e' && depth == 0) ||
			    (*str == ';' && depth-- <= 0))
			break;
		    if (*str == '?')
			depth++;
		}
		break;
	    case 'e':
		/* search forward for %; at same depth */
		for (depth = 0, str++; *str; str++) {
		    if (*str != '%')
			continue;
		    if (!*++str || (*str == ';' && depth-- <= 0))
			break;
		    if (*str == '?')
			depth++;
		}
		break;
	    default: break;
	}
	if (*str == '\0')
	    break;
	str++;
    }
    *bufptr = '\0';

    return buf;
}
