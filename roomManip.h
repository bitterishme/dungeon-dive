typedef struct RoomNode RoomNode;

struct RoomNode {
    char name[256];
    char code[20];
    char description[1024];
    RoomNode *north;
    RoomNode *east;
    RoomNode *south;
    RoomNode *west;
};

RoomNode* roomCreate(RoomNode *room);
RoomNode* readRoomFile(char *filename, int *size);
void getAvailableExits(RoomNode *room, char *buffer);
// Returns the head (top-left) node of the dungeon
RoomNode* createDungeonGrid(RoomNode *roomArray, int arraySize, int gridSize);
void deleteDungeonGrid(RoomNode *head, int size);
// Helper function to find a node in the grid
RoomNode* findNode(RoomNode *head, int row, int col, int size);

#endif