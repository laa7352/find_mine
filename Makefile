CFLAG := -lcurses

default: find_mine

clean:
	rm -rf find_mine

find_mine := find_mine.c
find_mine: find_mine.c
	gcc $(find_mine) $(CFLAG) -o $@

