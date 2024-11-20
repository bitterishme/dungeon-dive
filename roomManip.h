#ifndef ROOM_MANIP_H
#define ROOM_MANIP_H

// Forward declare the Room struct
typedef struct Room Room;

// Structure for room in file
struct Room {
    char name[256];
    char code[20];
    char description[1024];
};

// Forward declare the DungeonNode struct
typedef struct DungeonNode DungeonNode;

// Structure for node in dungeon
struct DungeonNode {
    Room room;                // Room data
    DungeonNode *north;      // Pointer to north room
    DungeonNode *east;       // Pointer to east room
    DungeonNode *south;      // Pointer to south room
    DungeonNode *west;       // Pointer to west room
};

typedef struct {
    Room *rooms;         // Array of rooms from file
    int numRooms;        // Number of rooms in array
} RoomList;

RoomList* readRoomFile(char *filename);
void freeRoomList(RoomList *list);
DungeonNode* createDungeonNode(Room *room);
DungeonNode* buildDungeon(RoomList *roomList, int size);
void freeDungeon(DungeonNode *start, int size);
void getAvailableExits(DungeonNode *node, char *buffer);

#endif