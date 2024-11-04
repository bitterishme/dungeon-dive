#ifndef ROOM_MANIP_H
#define ROOM_MANIP_H

typedef struct Room {
    char* name;
    char* code;
    char* description;
    struct Room *north;
    struct Room *east;
    struct Room *south;
    struct Room *west;
} Room;

Room* roomCreate(Room* original);
Room** readRoomFile(const char* filename, int* roomCount);

#endif