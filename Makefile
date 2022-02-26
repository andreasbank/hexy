PROG = hexy
OBJS = hexy.o

CFLAGS += -Wall -Werror

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@
	strip $(PROG)

.PHONY: clean
clean:
	$(RM) hexy *.o

