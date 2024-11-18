#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "roomManip.h"
#include "stringManip.h"

#define INITIAL_CAPACITY 10
#define MAX_LINE_LENGTH 1000

Room* roomCreate(Room* original) {
    if (!original) return NULL;
    
    Room* newRoom = malloc(sizeof(Room));
    if (!newRoom) return NULL;
    
    newRoom->name = strdup(original->name);
    newRoom->code = strdup(original->code);
    newRoom->description = strdup(original->description);
    newRoom->north = NULL;
    newRoom->east = NULL;
    newRoom->south = NULL;
    newRoom->west = NULL;
    
    if (!newRoom->name || !newRoom->code || !newRoom->description) {
        free(newRoom->name);
        free(newRoom->code);
        free(newRoom->description);
        free(newRoom);
        return NULL;
    }
    
    return newRoom;
}

Room** readRoomFile(const char* filename, int* roomCount) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;
    
    Room** rooms = NULL;
    int capacity = 0;
    *roomCount = 0;
    char line[MAX_LINE_LENGTH];
    
    while (fgets(line, sizeof(line), file)) {
        char* colon = strchr(line, ':');
        if (!colon) continue;
        
        *colon = '\0';
        char* key = str_trim(line);
        char* value = str_trim(colon + 1);
        
        if (!key || !value) continue;
        
        if (*roomCount >= capacity) {
            capacity = (capacity == 0) ? INITIAL_CAPACITY : capacity * 2;
            Room** temp = realloc(rooms, capacity * sizeof(Room*));
            if (!temp) {
                // Handle memory allocation failure
                fclose(file);
                return rooms;
            }
            rooms = temp;
            if (*roomCount == 0) {
                rooms[0] = malloc(sizeof(Room));
                if (!rooms[0]) {
                    fclose(file);
                    free(rooms);
                    return NULL;
                }
                memset(rooms[0], 0, sizeof(Room));
            }
        }
        
        if (strcmp(key, "Room Name") == 0) {
            rooms[*roomCount]->name = strdup(value);
        } else if (strcmp(key, "Room Code") == 0) {
            rooms[*roomCount]->code = strdup(value);
        } else if (strcmp(key, "Room Description") == 0) {
            rooms[*roomCount]->description = strdup(value);
            (*roomCount)++;
            if (*roomCount < capacity) {
                rooms[*roomCount] = malloc(sizeof(Room));
                if (rooms[*roomCount]) {
                    memset(rooms[*roomCount], 0, sizeof(Room));
                }
            }
        }
    }
    void getAvailableExits(Room *room, char *buffer) {
    buffer[0] = '\0';
    if (room->north) strcat(buffer, "N ");
    if (room->east) strcat(buffer, "E ");
    if (room->south) strcat(buffer, "S ");
    if (room->west) strcat(buffer, "W ");
}

RoomNode* createDungeonGrid(RoomNode *roomArray, int arraySize, int gridSize) {
    if (arraySize == 0 || gridSize == 0) return NULL;
    
    srand(time(NULL));
    
    // Create first row, left to right
    RoomNode *head = roomCreate(&roomArray[rand() % arraySize]);
    RoomNode *currentRow = head;
    RoomNode *prevRow = NULL;
    
    for (int row = 0; row < gridSize; row++) {
        RoomNode *rowStart = currentRow;
        
        // Create nodes in this row
        for (int col = 1; col < gridSize; col++) {
            RoomNode *newNode = roomCreate(&roomArray[rand() % arraySize]);
            
            // Link east-west
            currentRow->east = newNode;
            newNode->west = currentRow;
            
            // Link north-south with previous row if it exists
            if (prevRow != NULL) {
                RoomNode *nodeAbove = prevRow;
                for (int i = 0; i < col; i++) {
                    nodeAbove = nodeAbove->east;
                }
                nodeAbove->south = newNode;
                newNode->north = nodeAbove;
            }
            
            currentRow = newNode;
        }
        
        // If not last row, create start of next row
        if (row < gridSize - 1) {
            prevRow = rowStart;
            
            // Create first node of next row
            RoomNode *nextRowStart = roomCreate(&roomArray[rand() % arraySize]);
            
            rowStart->south = nextRowStart;
            nextRowStart->north = rowStart;
            currentRow = nextRowStart;
        }
    }
    
    return head;
}

RoomNode* findNode(RoomNode *head, int row, int col, int size) {
    if (!head || row < 0 || col < 0 || row >= size || col >= size) {
        return NULL;
    }
    
    // Move to correct row
    RoomNode *current = head;
    for (int i = 0; i < row; i++) {
        current = current->south;
        if (!current) return NULL;
    }
    
    // Move to correct column
    for (int i = 0; i < col; i++) {
        current = current->east;
        if (!current) return NULL;
    }
    
    return current;
}

void deleteDungeonGrid(RoomNode *head, int size) {
    if (!head) return;
    
    RoomNode *currentRow = head;
    
    while (currentRow) {
        RoomNode *rowStart = currentRow;
        RoomNode *nextRow = currentRow->south;
        
        while (rowStart) {
            RoomNode *next = rowStart->east;
            free(rowStart);
            rowStart = next;
        }
        
        currentRow = nextRow;
    }
}

void getAvailableExits(RoomNode *room, char *buffer) {
    buffer[0] = '\0';
    if (room->north) strcat(buffer, "N ");
    if (room->east) strcat(buffer, "E ");
    if (room->south) strcat(buffer, "S ");
    if (room->west) strcat(buffer, "W ");
}