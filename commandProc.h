#ifndef COMMAND_PROC_H
#define COMMAND_PROC_H

#include "roomManip.h"

typedef enum {
    CMD_MOVE,
    CMD_LOOK,
    CMD_QUIT,
    CMD_INVALID
} CommandType;

typedef struct {
    CommandType type;
    char argument[256];
} Command;

Command parseCommand(char *input);
Room* processCommand(Command cmd, Room *currentRoom, int *gameRunning);

#endif