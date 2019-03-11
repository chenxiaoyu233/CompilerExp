main: lex.c
	lex lex.c
	gcc lex.yy.c -ll -o lex.yy.out
	cat test.c | ./lex.yy.out
