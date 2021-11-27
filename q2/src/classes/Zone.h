#ifndef __Q2_ZONE_H__
#define __Q2_ZONE_H__

#include "../common.h"

typedef struct Zone
{
    char name;
    llint capacity;                     /* Done */
    sem_t seats_left;                   /* Done */
}
Zone;

Zone* new_zone_from_input(char name);
void print_zone(Zone *zone);

#endif
