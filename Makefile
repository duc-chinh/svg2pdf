EXEC = svg2pdf.exe
SOURCES = main.c menu.c args.c xml.c svg.c pdf.c
OBJECTS = $(SOURCES:.c=.o)
CC = gcc
CFLAGS = -W -Wall -D_GNU_SOURCE -g

.PHONY: default clean

default: $(EXEC)

xml.o: xml.c xml.h
pdf.o: pdf.c pdf.h
svg.o: svg.c svg.h xml.h pdf.h
args.o: args.c args.h
menu.o: menu.c menu.h
main.o: main.c main.h xml.o pdf.o svg.o args.o menu.o

%o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^ -lm

clean:
	rm -rf $(EXEC) $(OBJECTS) $(SOURCES:.c=.c~) $(SOURCES:.c=.h~) Makefile~