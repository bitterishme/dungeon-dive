#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "roomManip.h"

void printDungeon(Room* start) {
    if (!start) {
        printf("Error: Cannot print NULL dungeon\n");
        return;
    }
    Room* rowStart = start;
    
    while (rowStart) {
        Room* current = rowStart;
        while (current) {
            printf("%s: %s\n", current->code, current->name);
            current = current->east;
        }
        rowStart = rowStart->south;
    }
}

void deleteDungeon(Room* start) {
    if (!start) return;
    
    Room* rowStart = start;
    while (rowStart) {
        Room* current = rowStart;
        Room* nextRow = rowStart->south;
        
        while (current) {
            Room* nextInRow = current->east;
            free(current->name);
            free(current->code);
            free(current->description);
            free(current);
            current = nextInRow;
        }
        rowStart = nextRow;
    }
}

Room* createDungeon(Room** roomList, int roomCount, int size) {
    printf("Creating dungeon with size %d and %d rooms available\n", size, roomCount);
    if (!roomList || roomCount == 0 || size <= 0) {
        printf("Invalid parameters for dungeon creation\n");
        return NULL;
    }
    
    srand(time(NULL));
    Room* start = NULL;
    Room* above = NULL;
    
    for (int row = 0; row < size; row++) {
        printf("Creating row %d\n", row);
        Room* rowStart = NULL;
        Room* prevInRow = NULL;
        
        for (int col = 0; col < size; col++) {
            int idx = rand() % roomCount;
            printf("Using room template %d for position [%d,%d]\n", idx, row, col);
            if (!roomList[idx]) {
                printf("Error: NULL room template at index %d\n", idx);
                continue;
            }
            Room* newRoom = roomCreate(roomList[idx]);
            if (!newRoom) {
                printf("Failed to create room copy at position [%d,%d]\n", row, col);
                continue;
            }
            
            if (!start) start = newRoom;
            if (!rowStart) rowStart = newRoom;
            
            if (prevInRow) {
                prevInRow->east = newRoom;
                newRoom->west = prevInRow;
            }
            
            if (above) {
                above->south = newRoom;
                newRoom->north = above;
                above = above->east;
            }
            
            prevInRow = newRoom;
        }
        
        above = rowStart;
    }
    
    return start;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <room_file>\n", argv[0]);
        return 1;
    }
    
    int roomCount = 0;
    printf("Reading room file: %s\n", argv[1]);
    Room** roomList = readRoomFile(argv[1], &roomCount);
    if (!roomList) {
        printf("Error reading room file\n");
        return 1;
    }
    printf("Successfully read %d rooms\n", roomCount);
    
    char buffer[100];
    printf("Enter dungeon size: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    int dungeonSize = atoi(buffer);
    
    if (dungeonSize <= 0) {
        printf("Invalid dungeon size\n");
        // Clean up roomList
        for (int i = 0; i < roomCount; i++) {
            if (roomList[i]) {
                free(roomList[i]->name);
                free(roomList[i]->code);
                free(roomList[i]->description);
                free(roomList[i]);
            }
        }
        free(roomList);
        return 1;
    }
    
    Room* dungeon = createDungeon(roomList, roomCount, dungeonSize);
    if (!dungeon) {
        printf("Failed to create dungeon\n");
    } else {
        printf("\nDungeon layout:\n");
        printDungeon(dungeon);
        deleteDungeon(dungeon);
    }
    
    // Clean up roomList
    for (int i = 0; i < roomCount; i++) {
        if (roomList[i]) {
            free(roomList[i]->name);
            free(roomList[i]->code);
            free(roomList[i]->description);
            free(roomList[i]);
        }
    }
    free(roomList);
    
    return 0;
}