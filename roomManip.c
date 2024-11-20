#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "roomManip.h"
#include "stringManip.h"

#define BUFFER_SIZE 2048
#define INITIAL_ROOM_CAPACITY 10

RoomList* readRoomFile(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return NULL;
    
    RoomList *list = malloc(sizeof(RoomList));
    if (!list) {
        fclose(fp);
        return NULL;
    }
    
    // Initialize room array
    list->numRooms = 0;
    int capacity = INITIAL_ROOM_CAPACITY;
    list->rooms = malloc(sizeof(Room) * capacity);
    if (!list->rooms) {
        free(list);
        fclose(fp);
        return NULL;
    }
    
    char buffer[BUFFER_SIZE];
    Room currentRoom = {0};
    char descBuffer[1024] = {0};
    int isReadingDesc = 0;
    
    while (fgets(buffer, sizeof(buffer), fp)) {
        char *line = str_trim(buffer);
        
        if (strncmp(line, "ROOM", 4) == 0) {
            // Save previous room if exists
            if (strlen(currentRoom.code) > 0) {
                if (list->numRooms >= capacity) {
                    capacity *= 2;
                    Room *temp = realloc(list->rooms, sizeof(Room) * capacity);
                    if (!temp) {
                        freeRoomList(list);
                        fclose(fp);
                        return NULL;
                    }
                    list->rooms = temp;
                }
                list->rooms[list->numRooms++] = currentRoom;
            }
            
            // Start new room
            memset(&currentRoom, 0, sizeof(Room));
            strcpy(currentRoom.code, str_trim(line + 5));
            isReadingDesc = 0;
            memset(descBuffer, 0, sizeof(descBuffer));
        }
        else if (strncmp(line, "NAME", 4) == 0) {
            strcpy(currentRoom.name, str_trim(line + 5));
        }
        else if (strncmp(line, "DESC", 4) == 0) {
            isReadingDesc = 1;
        }
        else if (isReadingDesc) {
            if (strncmp(line, "ENDROOM", 7) == 0) {
                strcpy(currentRoom.description, descBuffer);
                isReadingDesc = 0;
            } else {
                if (strlen(descBuffer) > 0) strcat(descBuffer, "\n");
                strcat(descBuffer, line);
            }
        }
    }
    
    // Save last room if exists
    if (strlen(currentRoom.code) > 0) {
        if (list->numRooms >= capacity) {
            capacity *= 2;
            Room *temp = realloc(list->rooms, sizeof(Room) * capacity);
            if (!temp) {
                freeRoomList(list);
                fclose(fp);
                return NULL;
            }
            list->rooms = temp;
        }
        list->rooms[list->numRooms++] = currentRoom;
    }
    
    fclose(fp);
    return list;
}

void freeRoomList(RoomList *list) {
    if (list) {
        if (list->rooms) free(list->rooms);
        free(list);
    }
}

DungeonNode* createDungeonNode(Room *room) {
    DungeonNode *node = malloc(sizeof(DungeonNode));
    if (!node) return NULL;
    
    node->room = *room;
    node->north = NULL;
    node->east = NULL;
    node->south = NULL;
    node->west = NULL;
    
    return node;
}

DungeonNode* buildDungeon(RoomList *roomList, int size) {
    if (!roomList || size <= 0) return NULL;
    
    srand(time(NULL));
    
    // Create 2D array to store pointers to nodes temporarily
    DungeonNode ***grid = malloc(sizeof(DungeonNode**) * size);
    for (int i = 0; i < size; i++) {
        grid[i] = malloc(sizeof(DungeonNode*) * size);
    }
    
    // Create all nodes
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            Room *randomRoom = &roomList->rooms[rand() % roomList->numRooms];
            grid[i][j] = createDungeonNode(randomRoom);
            if (!grid[i][j]) {
                // Cleanup on failure
                for (int x = 0; x <= i; x++) {
                    for (int y = 0; y < (x == i ? j : size); y++) {
                        free(grid[x][y]);
                    }
                }
                for (int x = 0; x < size; x++) {
                    free(grid[x]);
                }
                free(grid);
                return NULL;
            }
        }
    }
    
    // Link nodes
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            // Link north
            if (i > 0) {
                grid[i][j]->north = grid[i-1][j];
            }
            // Link south
            if (i < size-1) {
                grid[i][j]->south = grid[i+1][j];
            }
            // Link west
            if (j > 0) {
                grid[i][j]->west = grid[i][j-1];
            }
            // Link east
            if (j < size-1) {
                grid[i][j]->east = grid[i][j+1];
            }
        }
    }
    
    // Save start node
    DungeonNode *start = grid[0][0];
    
    // Free temporary grid
    for (int i = 0; i < size; i++) {
        free(grid[i]);
    }
    free(grid);
    
    return start;
}

void freeDungeon(DungeonNode *start, int size) {
    if (!start) return;
    
    // Create visited array
    int visited[size][size];
    memset(visited, 0, sizeof(visited));
    
    // Free nodes using stack-based approach to avoid recursion
    typedef struct {
        DungeonNode *node;
        int row;
        int col;
    } StackNode;
    
    StackNode *stack = malloc(sizeof(StackNode) * size * size);
    int stackTop = 0;
    
    // Push start node
    stack[stackTop].node = start;
    stack[stackTop].row = 0;
    stack[stackTop].col = 0;
    stackTop++;
    
    while (stackTop > 0) {
        // Pop node
        stackTop--;
        StackNode current = stack[stackTop];
        
        // Skip if already visited
        if (visited[current.row][current.col]) continue;
        
        // Mark as visited
        visited[current.row][current.col] = 1;
        
        // Push unvisited neighbors
        if (current.node->east && current.col < size-1 && !visited[current.row][current.col+1]) {
            stack[stackTop].node = current.node->east;
            stack[stackTop].row = current.row;
            stack[stackTop].col = current.col + 1;
            stackTop++;
        }
        if (current.node->south && current.row < size-1 && !visited[current.row+1][current.col]) {
            stack[stackTop].node = current.node->south;
            stack[stackTop].row = current.row + 1;
            stack[stackTop].col = current.col;
            stackTop++;
        }
        
        // Free current node
        free(current.node);
    }
    
    free(stack);
}

void getAvailableExits(DungeonNode *node, char *buffer) {
    buffer[0] = '\0';
    if (node->north) strcat(buffer, "N ");
    if (node->east) strcat(buffer, "E ");
    if (node->south) strcat(buffer, "S ");
    if (node->west) strcat(buffer, "W ");
}