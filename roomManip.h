typedef struct Room Room;

struct Room {
    char name[256];
    char code[20];
    char description[1024];
    Room *north;
    Room *east;
    Room *south;
    Room *west;
};

Room* roomCreate(Room *room);
Room* readRoomFile(char *filename, int *size);
// New function to get available exits
void getAvailableExits(Room *room, char *buffer);
// New function to create 2D dungeon
Room** create2DDungeon(Room *roomArray, int arraySize, int dungeonSize);
// New function to free 2D dungeon
void delete2DDungeon(Room **dungeon, int size);

#endif