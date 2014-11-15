MODULES=shell_commands.o llist.o
svsh.o: lex.yy.c y.tab.c $(MODULES)
	gcc lex.yy.c y.tab.c $(MODULES) -lfl -o svsh.out
shell_commands.o: shell_commands.c shell_commands.h
	gcc -c shell_commands.c -o shell_commands.o
lex.yy.c: scan.l y.tab.h
	flex scan.l
y.tab.c y.tab.h: parse.y
	bison -y -d parse.y

llist.o: llist.h llist.c
	gcc -c llist.c -o llist.o
environvar.o: environvar.h environvar.c
	gcc -c environvar.c -o environvar.o

clean:
	rm -f lex.yy.c lex.yy.o y.tab.c y.tab.h y.tab.o  $(MODULES) svsh.out
