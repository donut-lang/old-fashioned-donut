path=path;..\bin\
set CC65_HOME=..\
cc65 -Oi game.c --add-source
ca65 crt0.s
ca65 game.s
ld65 -C nes.cfg -o Chase.nes crt0.o game.o runtime.lib
pause
del *.o
del game.s
Chase.nes