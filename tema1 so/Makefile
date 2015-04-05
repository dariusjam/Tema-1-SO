CC = cl
CFLAGS = /W3 /nologo /wd4127 -D_CRT_SECURE_NO_DEPRECATE
BIN = tema1.exe

build: tema1.obj functions.obj hash.obj
	$(CC) $(CFLAGS) /Fe$(EXE) $**

tema1.obj: tema1.c
	$(CC) -c $(CFLAGS) tema1.c

hash.obj: hash.c hash.h
	$(CC) -c $(CFLAGS) hash.c

functions.obj: functions.c functions.h
	$(CC) -c $(CFLAGS) functions.c

.PHONY: clean

clean:
	del /Q /F *.obj
	del /Q /F $(BIN)
