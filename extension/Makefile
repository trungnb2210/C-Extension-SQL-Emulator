CC		?= gcc
CFLAGS  ?= -std=c17 -g \
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-g -Wall -pedantic -Isql_files -I.

LDLIBS 	=  -Lsql_files -l:sql.a

BUILD	=	subs sql

.SUFFIXES: .c .o

.PHONY: all clean

all:	$(BUILD)

clean:
		/bin/rm -f $(BUILD) *.o core
		cd sql_files; make clean

subs:
		cd sql_files; make

sql: sql.o