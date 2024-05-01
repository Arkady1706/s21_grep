SHELL := /bin/bash
FLAGS = -Wall -Werror -Wextra -g
FILE = file1.txt file2.txt
PATTERN = patterns.reg
CHECK = 2>&1 valgrind --tool=memcheck --leak-check=yes

all: s21_grep tests
s21_grep: clean Makefile s21_grep.h s21_grep.c
	gcc $(FLAGS) s21_grep.c -o s21_grep

tests:
	# -diff <(./s21_grep Why $(FILE)) <(grep Why $(FILE))
	# -diff <(./s21_grep -e Why $(FILE)) <(grep -e Why $(FILE))
	# -diff <(./s21_grep -i Why $(FILE)) <(grep -i Why $(FILE))
	# -diff <(./s21_grep -v Why $(FILE)) <(grep -v Why $(FILE))
	# -diff <(./s21_grep -s why nonexist) <(grep -s why nonexist)
	# -diff <(./s21_grep -f $(PATTERN) $(FILE)) <(grep -f $(PATTERN) $(FILE))
	# -diff <(./s21_grep -iv Why $(FILE)) <(grep -iv Why $(FILE))
	# -diff <(./s21_grep -in Why $(FILE)) <(grep -in Why $(FILE))
	# -diff <(./s21_grep -ie "^we" $(FILE)) <(grep -ie "^we" $(FILE))
	# -diff <(./s21_grep -ince "^we" $(FILE)) <(grep -ince "^we" $(FILE))
	# -diff <(./s21_grep -lh do $(FILE)) <(grep -lh do $(FILE))
	# -diff <(./s21_grep -o Why we $(FILE)) <(grep -o Why we $(FILE))
	# -diff <(./s21_grep -oi shoud $(FILE)) <(grep -oi shoud $(FILE))
	# -diff <(./s21_grep -ohf $(PATTERN) $(FILE)) <(grep -ohf $(PATTERN) $(FILE))
	# -diff <(./s21_grep -oinc we $(FILE)) <(grep -oinc we $(FILE))
	# -diff <(./s21_grep -oihlnc -e we -e do -f $(PATTERN) $(FILE)) <(grep -oihlnc -e we -e do -f $(PATTERN) $(FILE))

	# $(CHECK) ./s21_grep we $(FILE) | grep -E ERROR
	# $(CHECK) ./s21_grep -e We -e do -e we $(FILE) | grep -E ERROR
	# $(CHECK) ./s21_grep -f $(PATTERN) $(FILE) | grep -E ERROR

clean:
	rm -f s21_grep