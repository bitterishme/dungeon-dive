CC = gcc
CFLAGS = -Wall -g
OBJS = main.o stringManip.o roomManip.o commandProc.o

all: dungeon-dive

dungeon-dive: $(OBJS)
	$(CC) $(CFLAGS) -o dungeon-dive $(OBJS)

main.o: main.c roomManip.h stringManip.h commandProc.h
	$(CC) $(CFLAGS) -c main.c

stringManip.o: stringManip.c stringManip.h
	$(CC) $(CFLAGS) -c stringManip.c

roomManip.o: roomManip.c roomManip.h stringManip.h
	$(CC) $(CFLAGS) -c roomManip.c

commandProc.o: commandProc.c commandProc.h roomManip.h stringManip.h
	$(CC) $(CFLAGS) -c commandProc.c

clean:
	rm -f dungeon-dive $(OBJS)