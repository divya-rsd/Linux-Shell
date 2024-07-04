CC = gcc
CFLAGS = -g
SRC_FILES = main.c prompt.c past.c sp4details.c spec2.c spec3.c spec4.c spec6.c spec7.c spec8.c neonate.c manpage.c spec13.c ping.c helper.c
EXE = a.out

all: $(EXE)

$(EXE): $(SRC_FILES)
	$(CC) $(CFLAGS) -o $@ $(SRC_FILES)

clean:
	rm -f $(EXE)

.PHONY: all clean
