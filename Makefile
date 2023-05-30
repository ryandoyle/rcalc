CC   = i686-w64-mingw32-gcc
WINDRES = i686-w64-mingw32-windres
RES  = 
OBJ  = rcalc.o $(RES)
LINKOBJ  = rcalc.o $(RES)
LIBS =  -mwindows # -mwindows needed so this isn't a console application
BIN  = rcalc.exe
CFLAGS = $(INCS) -s
RM = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before rcalc.exe all-after


clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LINKOBJ) -o "rcalc.exe" $(LIBS)

rcalc.o: rcalc.c
	$(CC) -c rcalc.c -o rcalc.o $(CFLAGS)

copy: rcalc.exe
	cp rcalc.exe ~/mnt/data/software/win98/dev/