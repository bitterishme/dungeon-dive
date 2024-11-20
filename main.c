#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "roomManip.h"
#include "stringManip.h"
#include "commandProc.h"

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    // Check command line arguments
    if (argc != 2) {
        printf("Usage: %s <room_file>\n", argv[0]);
        return 1;
    }
    
    // Read rooms from file
    RoomList *roomList = readRoomFile(argv[1]);
    if (!roomList) {
        printf("Error reading room file\n");
        return 1;
    }
    
    // Get dungeon size from user
    int dungeonSize;
    char buffer[BUFFER_SIZE];
    printf("Enter desired dungeon size: ");
    fgets(buffer, sizeof(buffer), stdin);
    dungeonSize = atoi(buffer);
    
    if (dungeonSize <= 0) {
        printf("Invalid dungeon size\n");
        freeRoomList(roomList);
        return 1;
    }
    
    // Build the dungeon
    DungeonNode *dungeon = buildDungeon(roomList, dungeonSize);
    if (!dungeon) {
        printf("Error creating dungeon\n");
        freeRoomList(roomList);
        return 1;
    }
    
    // Start game loop
    DungeonNode *currentRoom = dungeon;
    int gameRunning = 1;
    
    // Welcome message and initial room description
    printf("\nWelcome to the Dungeon!\n");
    printf("Available commands: move [n/e/s/w], look, quit\n\n");
    
    // Show initial room
    Command lookCmd = {CMD_LOOK, ""};
    processCommand(lookCmd, currentRoom, &gameRunning);
    
    // Main game loop
    while (gameRunning) {
        printf("\nWhat would you like to do? ");
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            break;
        }
        
        Command cmd = parseCommand(buffer);
        currentRoom = processCommand(cmd, currentRoom, &gameRunning);
        
        // Show room description after each move
        if (cmd.type == CMD_MOVE && currentRoom && gameRunning) {
            char exits[100];
            printf("\nYou are in %s\n", currentRoom->room.name);
            printf("%s\n", currentRoom->room.description);
            getAvailableExits(currentRoom, exits);
            printf("Exits: %s\n", exits);
        }
    }
    
    // Cleanup
    freeDungeon(dungeon, dungeonSize);
    freeRoomList(roomList);
    
    return 0;
}