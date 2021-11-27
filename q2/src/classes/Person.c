#include "Person.h"

Person* new_person_from_input(llint group_number)
{
    Person *person = malloc(sizeof(Person));
    if (person == NULL)
        err_n_die("Failed to create a new Person");

    person->group_number = group_number;

    // scanf("%s", person->name);
    // scanf("%c", &person->team);
    // scanf("%lld", &person->arrival_time_delay);
    // scanf("%lld", &person->patience_time);
    // scanf("%lld", &person->goal_threshold);

    // scanf("%s %c %lf %lf %d", persons[y]->name, &c, &persons[y]->wait_time, &persons[y]->patience_time, &persons[y]->enrage_goal_count);
    scanf("%s %c %lld %lld %lld", person->name, &person->team, &person->arrival_time_delay, &person->patience_time, &person->goal_threshold);

    person->finished_watching = false;
    person->zone = NULL;

    return person;
}

void print_person(Person *person)
{
    printf("\n");

    printf(COLOR_BLUE "Person Name: " COLOR_YELLOW "%s" COLOR_RESET "\n", person->name);
    printf(COLOR_BLUE "Person Group Number: " COLOR_YELLOW "%lld" COLOR_RESET "\n", person->group_number);
    printf(COLOR_BLUE "Person Team: " COLOR_YELLOW "%c" COLOR_RESET "\n", person->team);
    printf(COLOR_BLUE "Person Arrival Time: " COLOR_YELLOW "%lld" COLOR_RESET "\n", person->arrival_time_delay);
    printf(COLOR_BLUE "Person Patience Time: " COLOR_YELLOW "%lld" COLOR_RESET "\n", person->patience_time);
    printf(COLOR_BLUE "Person Goals Threshold: " COLOR_YELLOW "%lld" COLOR_RESET "\n", person->goal_threshold);

    printf("\n");
}
