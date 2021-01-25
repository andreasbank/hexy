INSTALL ?= install
PROGS = hexy biny

# Set TEST to 1 to enable a simple self-test
# The test expects a lower-case HEX string as input
TEST = 0

CFLAGS += -Wall -Werror
CFLAGS += -DTEST=$(TEST)

all: $(PROGS)

install:
	$(INSTALL) -m 755 $(PROGS) /usr/local/bin/

.PHONY: clean
clean:
	$(RM) hexy *.o
