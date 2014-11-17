MODULES=shell_commands.o llist.o pidlist.o
svsh.o: lex.yy.c y.tab.c $(MODULES)
	gcc lex.yy.c y.tab.c $(MODULES) -g -lfl -o svsh.out
	
shell_commands.o: shell_commands.c shell_commands.h
	gcc -c shell_commands.c -g -o shell_commands.o
llist.o: llist.h llist.c
	gcc -c llist.c -g -o llist.o
pidlist.o: pidlist.h pidlist.c
	gcc -c pidlist.c -g -o pidlist.o	

noop: noop.c
	gcc noop.c -o noop
loop: loop.c
	gcc loop.c -o loop
	
lex.yy.c: scan.l y.tab.h
	flex scan.l
y.tab.c y.tab.h: parse.y
	bison -y -d parse.y
clean:
	rm -f lex.yy.c lex.yy.o y.tab.c y.tab.h y.tab.o  $(MODULES) svsh.out
