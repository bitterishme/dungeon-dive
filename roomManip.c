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
    if (!fp) {
        printf("Could not open file %s\n", filename);
        return NULL;
    }
    
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
    int isReadingDesc = 0;
    
    while (fgets(buffer, sizeof(buffer), fp)) {
        char *line = str_trim(buffer);
        
        // Skip empty lines
        if (strlen(line) == 0) {
            if (isReadingDesc && strlen(currentRoom.name) > 0) {
                // End of a room description
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
                memset(&currentRoom, 0, sizeof(Room));
                isReadingDesc = 0;
            }
            continue;
        }
        
        if (strncmp(line, "Room Name:", 10) == 0) {
            strcpy(currentRoom.name, str_trim(line + 10));
        }
        else if (strncmp(line, "Room Code:", 10) == 0) {
            strcpy(currentRoom.code, str_trim(line + 10));
        }
        else if (strncmp(line, "Room Description:", 16) == 0) {
            isReadingDesc = 1;
            currentRoom.description[0] = '\0';
            // Get the rest of this line if there is any
            char *descStart = str_trim(line + 16);
            if (strlen(descStart) > 0) {
                strcpy(currentRoom.description, descStart);
            }
        }
        else if (isReadingDesc) {
            // If we already have some description, add a space
            if (strlen(currentRoom.description) > 0) {
                strcat(currentRoom.description, " ");
            }
            strcat(currentRoom.description, line);
        }
    }
    
    // Save last room if exists
    if (strlen(currentRoom.name) > 0) {
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
    
    // Debug output
    printf("Loaded %d rooms:\n", list->numRooms);
    for (int i = 0; i < list->numRooms; i++) {
        printf("%d. [%s] %s\n", i+1, list->rooms[i].code, list->rooms[i].name);
    }
    
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
    // Add validation
    if (!roomList || size <= 0 || roomList->numRooms <= 0) {
        printf("Invalid parameters for dungeon creation\n");
        return NULL;
    }
    
    srand(time(NULL));
    
    // Create 2D array to store pointers to nodes temporarily
    DungeonNode ***grid = malloc(sizeof(DungeonNode**) * size);
    if (!grid) {
        printf("Memory allocation failed for grid\n");
        return NULL;
    }
    
    for (int i = 0; i < size; i++) {
        grid[i] = malloc(sizeof(DungeonNode*) * size);
        if (!grid[i]) {
            // Clean up previously allocated memory
            for (int j = 0; j < i; j++) {
                free(grid[j]);
            }
            free(grid);
            printf("Memory allocation failed for grid row\n");
            return NULL;
        }
    }
    
    // Create all nodes
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            // Ensure we have rooms to select from
            if (roomList->numRooms <= 0) {
                printf("No rooms available in room list\n");
                // Clean up
                for (int x = 0; x < size; x++) {
                    free(grid[x]);
                }
                free(grid);
                return NULL;
            }
            
            int randomIndex = rand() % roomList->numRooms;
            Room *randomRoom = &roomList->rooms[randomIndex];
            
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
                printf("Failed to create dungeon node\n");
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