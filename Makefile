CC = gcc
CFLAGS = -Wall -Werror
OBJS = main.o stringManip.o roomManip.o

dungeonmaster: $(OBJS)
	$(CC) $(OBJS) -o dungeonmaster

main.o: main.c roomManip.h
	$(CC) $(CFLAGS) -c main.c

stringManip.o: stringManip.c stringManip.h
	$(CC) $(CFLAGS) -c stringManip.c

roomManip.o: roomManip.c roomManip.h stringManip.h
	$(CC) $(CFLAGS) -c roomManip.c

clean:
	rm -f $(OBJS) dungeonmaster