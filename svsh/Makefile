a.out: lex.yy.c y.tab.c
	gcc lex.yy.c y.tab.c -ll 
lex.yy.c: scan.l y.tab.h
	lex scan.l
y.tab.c y.tab.h: parse.y
	yacc -d parse.y

clean:
	rm -f lex.yy.c lex.yy.o y.tab.c y.tab.h y.tab.o a.out
