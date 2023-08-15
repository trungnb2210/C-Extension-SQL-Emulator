CC = gcc
LEX = flex
YACC = bison
CFLAGS ?= -std=c17 -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -Wall -pedantic

TARGET_LIB = sql.a
TARGET_EXEC = unit_tests
OBJS = lex.yy.o parser.tab.o ast.o arraylist.o hashtable.o groupby.o aggregates.o table.o from.o orderby.o wherehaving.o join.o select.o utils.o

all: $(TARGET_LIB) $(TARGET_EXEC)

$(TARGET_LIB): $(OBJS)
	ar rcs $(TARGET_LIB) $(OBJS)

$(TARGET_EXEC): unit_tests.o $(TARGET_LIB)
	$(CC) $(CFLAGS) -o $(TARGET_EXEC) unit_tests.o -L. -l:sql.a

lex.yy.o: lex.yy.c parser.tab.h
	$(CC) $(CFLAGS) -c lex.yy.c -o lex.yy.o

parser.tab.o: parser.tab.c parser.tab.h
	$(CC) $(CFLAGS) -c parser.tab.c -o parser.tab.o

lex.yy.c: parser.l parser.tab.h
	$(LEX) parser.l

parser.tab.c parser.tab.h: parser.y
	$(YACC) -d parser.y

clean:
	rm -f $(TARGET_LIB) $(TARGET_EXEC) $(OBJS) lex.yy.c parser.tab.c parser.tab.h parser unit_tests.o

.PHONY: clean all
