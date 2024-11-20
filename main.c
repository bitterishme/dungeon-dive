#include <stdio.h>
#include <stdlib.h>
#include "roomManip.h"
#include "stringManip.h"
#include "commandProc.h"

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <room_file>\n", argv[0]);
        return 1;
    }
    
    // Read room file
    int size;
    RoomNode *roomArray = readRoomFile(argv[1], &size);
    if (roomArray == NULL) {
        printf("Error reading room file\n");
        return 1;
    }
    
    // Get dungeon size
    int dungeonSize;
    char buffer[BUFFER_SIZE];
    printf("Enter desired dungeon size: ");
    fgets(buffer, sizeof(buffer), stdin);
    dungeonSize = atoi(buffer);
    
    if (dungeonSize <= 0) {
        printf("Invalid dungeon size\n");
        free(roomArray);
        return 1;
    }
    
    // Create dungeon grid using linked list
    RoomNode *dungeonHead = createDungeonGrid(roomArray, size, dungeonSize);
    if (dungeonHead == NULL) {
        printf("Error creating dungeon\n");
        free(roomArray);
        return 1;
    }
    
    // Start game loop
    RoomNode *currentRoom = dungeonHead;
    int gameRunning = 1;
    
    printf("\nWelcome to the Dungeon!\n");
    printf("Commands: move [n/e/s/w], look, quit\n\n");
    
    // Initial room description
    printf("You are in %s\n", currentRoom->name);
    printf("%s\n", currentRoom->description);
    char exits[100];
    getAvailableExits(currentRoom, exits);
    printf("Exits: %s\n", exits);
    
    while (gameRunning) {
        printf("\nWhat would you like to do? ");
        fgets(buffer, sizeof(buffer), stdin);
        
        Command cmd = parseCommand(buffer);
        switch(cmd.type) {
            case CMD_MOVE:
                switch(cmd.argument[0]) {
                    case 'n':
                    case 'N':
                        if (currentRoom->north) {
                            currentRoom = currentRoom->north;
                            printf("Moving north...\n");
                        } else {
                            printf("You bump into a wall trying to go north.\n");
                        }
                        break;
                    case 'e':
                    case 'E':
                        if (currentRoom->east) {
                            currentRoom = currentRoom->east;
                            printf("Moving east...\n");
                        } else {
                            printf("You bump into a wall trying to go east.\n");
                        }
                        break;
                    case 's':
                    case 'S':
                        if (currentRoom->south) {
                            currentRoom = currentRoom->south;
                            printf("Moving south...\n");
                        } else {
                            printf("You bump into a wall trying to go south.\n");
                        }
                        break;
                    case 'w':
                    case 'W':
                        if (currentRoom->west) {
                            currentRoom = currentRoom->west;
                            printf("Moving west...\n");
                        } else {
                            printf("You bump into a wall trying to go west.\n");
                        }
                        break;
                    default:
                        printf("Invalid direction. Use n, e, s, or w.\n");
                }
                // Show new room after movement
                printf("\nYou are in %s\n", currentRoom->name);
                printf("%s\n", currentRoom->description);
                getAvailableExits(currentRoom, exits);
                printf("Exits: %s\n", exits);
                break;
                
            case CMD_LOOK:
                printf("\nRoom: %s [%s]\n", currentRoom->name, currentRoom->code);
                printf("Description: %s\n", currentRoom->description);
                getAvailableExits(currentRoom, exits);
                printf("Exits: %s\n", exits);
                break;
                
            case CMD_QUIT:
                gameRunning = 0;
                printf("Thanks for playing!\n");
                break;
                
            case CMD_INVALID:
                printf("I don't understand that command.\n");
                printf("Available commands: move [n/e/s/w], look, quit\n");
                break;
        }
    }
    
    // Cleanup
    deleteDungeonGrid(dungeonHead, dungeonSize);
    free(roomArray);
    
    return 0;
}