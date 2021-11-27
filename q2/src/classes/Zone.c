#include "Zone.h"

Zone* new_zone_from_input(char name)
{
    Zone *zone = malloc(sizeof(Zone));
    if(zone == NULL)
        err_n_die("Failed to create a Zone");

    scanf("%lld", &zone->capacity);

    sem_init(&zone->seats_left, 0, zone->capacity);
    zone->name = name;

    return zone;
}

void print_zone(Zone *zone)
{
    printf("\n");
    printf(COLOR_BLUE "Zone Name: " COLOR_YELLOW "%c" COLOR_RESET "\n", zone->name);
    printf(COLOR_BLUE "Zone Capacity: " COLOR_YELLOW "%lld" COLOR_RESET "\n", zone->capacity);
    printf("\n");
}
