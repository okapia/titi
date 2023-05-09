#include <term.h>

#include <stdio.h>
#include <string.h>

static int pass, fail;

#define EQ(EXPECTED, ACTUAL) \
    do { \
	char *ex = EXPECTED; \
	char *ac = ACTUAL; \
	if (strcmp(ex, ac)) { \
	    printf("FAIL: %d\n  actual: %s\n  expected: %s\n", __LINE__, ac, ex); \
	    ++fail; \
        } else ++pass; \
    } while (0)

#define SETB "%?%p1%{7}%>%tCSI38;5;%p1%dm%eCSI3%?%p1%{1}%=%t4%e%p1%{3}%=%t6%e%p1%{4}%=%t1%e%p1%{6}%=%t3%e%p1%d%;m%;"

int main() {
    EQ("%", tparm("%%"));
    EQ("19", tparm("%{19}%d"));
    EQ("6194", tparm("%{06194}%d"));
    EQ("45", tparm("%p1%d", 45));
    EQ(" ", tparm("%p1%c", 32));
    EQ(":", tparm("%':'%c"));
    EQ("foo:3", tparm("%p1%s:%p1%l%d", "foo"));
    EQ("10", tparm("%p1%p2%+%d", 2, 8));
    EQ("6", tparm("%p1%p2%-%d", 8, 2));
    EQ("16", tparm("%p1%p2%*%d", 2, 8));
    EQ("2", tparm("%p1%p2%/%d", 6, 3));
    EQ("7", tparm("%p1%p2%m%d", 17, 10));
    EQ("0", tparm("%p1%p2%m%d", 0, 0));
    EQ("0023   x", tparm("%{23}%:-7.4dx"));
    EQ(" +78x", tparm("%{78}%:+4dx"));
    EQ("0X1B", tparm("%p1%#X", 27));
    EQ("12", tparm("%p1%#d", 12));
    EQ("          hello", tparm("%p1%15.5s", "helloworld"));
    EQ("6", tparm("%{2}%p1%PA%Pa%gA%ga%-%d", 8));
    EQ("7", tparm("%i%p2%p1%+%d", 2, 3));
    EQ("CSI38;5;125m", tparm(SETB, 125));
    EQ("CSI33m", tparm(SETB, 6));
    EQ("now", tparm("%{0}%Pa%?%ga%tno%e%ga%tno%e%ga%tstill no%e%{1}%tnow%enot%;"));
    printf("%d tests passed, %d failed\n", pass, fail);
    return 0;
}
