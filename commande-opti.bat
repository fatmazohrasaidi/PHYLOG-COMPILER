flex lexical.l
bison -d syntaxique.y
gcc lex.yy.c syntaxique.tab.c ts.c semantique.c -o pgm.exe -lfl -ly
pgm.exe test-opti.txt
