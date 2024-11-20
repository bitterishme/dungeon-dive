CC = gcc
CFLAGS = -Wall -g
OBJS = main.o stringManip.o roomManip.o commandProc.o

all: dungeon-dive

# Link all object files into final executable
dungeon-dive: $(OBJS)
	$(CC) $(CFLAGS) -o dungeon-dive $(OBJS)

# Compile main.c
main.o: main.c roomManip.h stringManip.h commandProc.h
	$(CC) $(CFLAGS) -c main.c

# Compile stringManip.c
stringManip.o: stringManip.c stringManip.h
	$(CC) $(CFLAGS) -c stringManip.c

# Compile roomManip.c
roomManip.o: roomManip.c roomManip.h stringManip.h
	$(CC) $(CFLAGS) -c roomManip.c

# Compile commandProc.c
commandProc.o: commandProc.c commandProc.h roomManip.h stringManip.h
	$(CC) $(CFLAGS) -c commandProc.c

# Clean up compiled files
clean:
	rm -f dungeon-dive $(OBJS)