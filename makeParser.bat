set CURDIR=%~dp0

set CLASSPATH=%CURDIR%\external\antlr\antlr-3.4-complete.jar;%CLASSPATH%
java org.antlr.Tool -fo %ANTLR_OUT% -make %ANTLR_SRC%/Donut.g %ANTLR_SRC%/Compiler.g

for /F "usebackq" %%s in (`echo %ANTLR_OUT% ^| sed s_/_\\_g`) do set ANTLR_OUT_W=%%s
ren %ANTLR_OUT_W%\*.c *.cpp

