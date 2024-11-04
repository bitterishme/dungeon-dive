#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    
    fclose(file);
    return rooms;
}