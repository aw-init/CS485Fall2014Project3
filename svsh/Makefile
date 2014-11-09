svsh.o: lex.yy.c y.tab.c
	gcc lex.yy.c y.tab.c -lfl -o svsh.o
lex.yy.c: scan.l y.tab.h
	flex scan.l
y.tab.c y.tab.h: parse.y
	bison -y -d parse.y
clean:
	rm -f lex.yy.c lex.yy.o y.tab.c y.tab.h y.tab.o a.out
