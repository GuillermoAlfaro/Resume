

EXECBIN  = httpserver
SOURCES  = $(wildcard *.c)
HEADERS  = $(wildcard *.h)
OBJECTS  = $(SOURCES:%.c=%.o)
LIBRARY  =  asgn4_helper_funcs.a
FORMATS  = $(SOURCES:%.c=.format/%.c.fmt) $(HEADERS:%.h=.format/%.h.fmt)

CC       = clang
FORMAT   = clang-format
CFLAGS   = -Wall -Wpedantic -Werror -Wextra -DDEBUG

.PHONY: all clean format #test

all: $(EXECBIN)

test: test.o asgn4_helper_funcs.a
	$(CC) -g -o test test.o asgn4_helper_funcs.a

test.o : test.c
	$(CC) $(CFLAGS) -g -c test.c

$(EXECBIN): $(OBJECTS) $(LIBRARY)
	$(CC) -o $@ $^ asgn4_helper_funcs.a

%.o : %.c %.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(EXECBIN) $(OBJECTS)

nuke: clean
	rm -rf .format

format: $(FORMATS)

.format/%.c.fmt: %.c
	mkdir -p .format
	$(FORMAT) -i $<
	touch $@

.format/%.h.fmt: %.h
	mkdir -p .format
	$(FORMAT) -i $<
	touch $@
