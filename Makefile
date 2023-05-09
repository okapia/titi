TARGET = libtiti.a
SRCS = titi.c tparm.c
TEST = test.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

test: ttest
	./ttest
check: ref
	./ref

ttest: $(TEST:.c=.o) $(TARGET)
	$(CC) $(LDFLAGS) $(CFLAGS) -L. -ltiti -o $@ $(TEST:.c=.o)

ref: $(TEST:.c=.o)
	$(CC) -lcurses -o $@ test.o

libtiti.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

clean:
	rm -f $(TARGET) $(OBJS) test.o ttest ref
