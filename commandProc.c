#include <string.h>
#include <stdio.h>
#include "commandProc.h"
#include "stringManip.h"

Command parseCommand(char *input) {
    Command cmd = {CMD_INVALID, ""};
    char *trimmed = str_trim(input);
    
    // Split input into command and argument
    char *space = strchr(trimmed, ' ');
    if (space != NULL) {
        *space = '\0';  // Split string at space
        strcpy(cmd.argument, str_trim(space + 1));
    }
    
    if (strcmp(trimmed, "move") == 0) {
        cmd.type = CMD_MOVE;
    } else if (strcmp(trimmed, "look") == 0) {
        cmd.type = CMD_LOOK;
    } else if (strcmp(trimmed, "quit") == 0) {
        cmd.type = CMD_QUIT;
    }
    
    return cmd;
}

Room* processCommand(Command cmd, Room *currentRoom, int *gameRunning) {
    switch(cmd.type) {
        case CMD_MOVE:
            switch(cmd.argument[0]) {
                case 'n':
                case 'N':
                    if (currentRoom->north) {
                        return currentRoom->north;
                    }
                    printf("You bump into a wall trying to go north.\n");
                    break;
                case 'e':
                case 'E':
                    if (currentRoom->east) {
                        return currentRoom->east;
                    }
                    printf("You bump into a wall trying to go east.\n");
                    break;
                case 's':
                case 'S':
                    if (currentRoom->south) {
                        return currentRoom->south;
                    }
                    printf("You bump into a wall trying to go south.\n");
                    break;
                case 'w':
                case 'W':
                    if (currentRoom->west) {
                        return currentRoom->west;
                    }
                    printf("You bump into a wall trying to go west.\n");
                    break;
                default:
                    printf("Invalid direction. Use n, e, s, or w.\n");
            }
            break;
            
        case CMD_LOOK:
            printf("\nRoom: %s [%s]\n", currentRoom->name, currentRoom->code);
            printf("Description: %s\n", currentRoom->description);
            char exits[100];
            getAvailableExits(currentRoom, exits);
            printf("Exits: %s\n", exits);
            break;
            
        case CMD_QUIT:
            *gameRunning = 0;
            printf("Thanks for playing!\n");
            break;
            
        case CMD_INVALID:
            printf("I don't understand that command.\n");
            printf("Available commands: move [n/e/s/w], look, quit\n");
            break;
    }
    
    return currentRoom;
}