#include "common.h"
#include "classes/Group.h"
#include "classes/Person.h"
#include "classes/Zone.h"
#include "classes/GoalChance.h"
#include "classes/Team.h"

llint spectating_time;
llint num_groups;
llint num_goal_chances;

Zone *zone_H, *zone_A, *zone_N;
Group **all_groups;
GoalChance **all_goal_chances;

Team *home_team, *away_team;

sem_t seats_waiting_sem[3];

pthread_t goal_t;

struct timespec start_time;


llint time_elapsed()
{
    struct timespec curr_time;
    clock_gettime(CLOCK_REALTIME, &curr_time);
    return (curr_time.tv_sec - start_time.tv_sec);
}


#if DEBUG > 0
void print_seats()
{
    int sem_values_g_1;
    int sem_values_g_2;
    int sem_values_g_3;
    int sem_values_h;
    int sem_values_a;
    int sem_values_n;
    sem_getvalue(&seats_waiting_sem[HOME_TEAM_SUPPORTERS], &sem_values_g_1);
    sem_getvalue(&seats_waiting_sem[AWAY_TEAM_SUPPORTERS], &sem_values_g_2);
    sem_getvalue(&seats_waiting_sem[NEUTRAL_SUPPORTERS], &sem_values_g_3);
    sem_getvalue(&zone_H->seats_left, &sem_values_h);
    sem_getvalue(&zone_A->seats_left, &sem_values_a);
    sem_getvalue(&zone_N->seats_left, &sem_values_n);

    printf(\
            "G_H: %d, G_A: %d, G_N: %d, "\
            "Z_H: %d, Z_A: %d, Z_N: %d\n",\
            sem_values_g_1,\
            sem_values_g_2,\
            sem_values_g_3,\
            sem_values_h,\
            sem_values_a,\
            sem_values_n\
          );
}
#endif

void* people_thread(void *arg)
{
    Person *person = (Person*)arg;
    sleep(person->arrival_time_delay);

    printf(\
            COLOR_RED\
            "t=%lld: %s has reached the stadium"\
            COLOR_RESET"\n",\
            time_elapsed(),\
            person->name\
          );

    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
        err_n_die("Failed to get current time");

    llint rt;
    ts.tv_sec += person->patience_time;

    sem_t *seat_waiting_sem_p = NULL;
    if (person->team == 'A') {
        seat_waiting_sem_p = &seats_waiting_sem[AWAY_TEAM_SUPPORTERS];
    }
    else if (person->team == 'H') {
        seat_waiting_sem_p = &seats_waiting_sem[HOME_TEAM_SUPPORTERS];
    }
    else {
        seat_waiting_sem_p = &seats_waiting_sem[NEUTRAL_SUPPORTERS];
    }

    while (person->zone == NULL)
    {
        if (person->team == 'A') {
            if (sem_trywait(&zone_A->seats_left) == 0) {
                person->zone = zone_A;
                continue;
            }
        }
        else if (person->team == 'H') {
            if (sem_trywait(&zone_H->seats_left) == 0) {
                person->zone = zone_H;
                continue;
            }
            else if (sem_trywait(&zone_N->seats_left) == 0) {
                person->zone = zone_N;
                continue;
            }
        }
        else {
            if (sem_trywait(&zone_H->seats_left) == 0) {
                person->zone = zone_H;
                continue;
            }
            else if (sem_trywait(&zone_N->seats_left) == 0) {
                person->zone = zone_N;
                continue;
            }
            else if (sem_trywait(&zone_A->seats_left) == 0) {
                person->zone = zone_A;
                continue;
            }
        }

        int sem_wait_return_value = sem_timedwait(seat_waiting_sem_p, &ts);
        if (sem_wait_return_value != 0)
        {
            printf(\
                    COLOR_MAGENTA\
                    "t=%lld: %s could not get a seat"\
                    COLOR_RESET"\n",\
                    time_elapsed(),\
                    person->name\
                  );
            goto to_gate;
        }
    }

    printf(\
            COLOR_MAGENTA\
            "t=%lld: %s has got a seat in zone %c"\
            COLOR_RESET"\n",\
            time_elapsed(),\
            person->name,\
            person->zone->name\
          );

#if DEBUG > 0
    print_seats();
#endif

    if (person->team == 'N')
    {
        sleep(spectating_time);
        printf(\
                COLOR_GREEN\
                "t=%lld: %s watched the match for %lld seconds and is leaving"\
                COLOR_RESET"\n",\
                time_elapsed(),\
                person->name,\
                spectating_time\
              );
    }

    else {
        Team *opponent_team = (person->team == 'H') ? away_team : home_team;
        pthread_mutex_t *goal_lock_p = &opponent_team->lock;
        pthread_cond_t *goal_cond_p = &opponent_team->goal_cond;

        if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
            err_n_die("Failed to get current time");

        ts.tv_sec += spectating_time;
        pthread_mutex_lock(goal_lock_p);

        while (opponent_team->goals < person->goal_threshold && !person->finished_watching)
        {
            rt = pthread_cond_timedwait(goal_cond_p, goal_lock_p, &ts);

            if (rt != 0){
                printf(\
                        COLOR_GREEN\
                        "t=%lld: %s watched the match for %lld seconds and is leaving"\
                        COLOR_RESET"\n",\
                        time_elapsed(),\
                        person->name,\
                        spectating_time\
                      );
                person->finished_watching = true;
            }

        }

        if (!person->finished_watching && opponent_team->goals >= person->goal_threshold)
        {
            printf(\
                    COLOR_GREEN\
                    "t=%lld: %s is leaving due to bad performance of his team"\
                    COLOR_RESET"\n",\
                    time_elapsed(),\
                    person->name\
                  );
        }

        pthread_mutex_unlock(&opponent_team->lock);
    }

    sem_post(&person->zone->seats_left);
    sem_post(seat_waiting_sem_p);

to_gate:

    printf(\
            COLOR_BLUE\
            "t=%lld: %s is waiting for their friends at the exit"\
            COLOR_RESET "\n",\
            time_elapsed(),\
            person->name\
          );

    Group *group = all_groups[person->group_number];
    pthread_mutex_lock(&group->lock);
    group->num_not_at_gate --;
    if (group->num_not_at_gate == 0)
    {
        printf(\
                COLOR_YELLOW\
                "t=%lld: Group %lld is leaving for dinner"\
                COLOR_RESET "\n",\
                time_elapsed(),\
                group->id+1\
              );
    }
    pthread_mutex_unlock(&group->lock);

    return NULL;
}

// Goals simulation ke liye thread
void* goals_thread(void *arg)
{
    GoalChance *goal = (GoalChance*)arg;
    Team *team = goal->team == 'H' ? home_team : away_team;

#if DEBUG > 0
    printf("Goal Chance at time = %lld is starting\n", goal->time);
#endif

    sleep(goal->time);

    if (goal->is_successful(goal))
    {
        pthread_mutex_lock(&team->lock);
        team->goals += 1;
        printf(\
                TEXT_BOLD\
                "t=%lld: Team %c has scored their %lldth goal"\
                COLOR_RESET"\n",\
                time_elapsed(),\
                team->name,\
                team->goals\
              );
        pthread_cond_broadcast(&team->goal_cond);
        pthread_mutex_unlock(&team->lock);
    }
    else
    {
        printf(\
                TEXT_BOLD\
                "t=%lld: Team %c has failed to score their %lldth goal"\
                COLOR_RESET"\n",\
                time_elapsed(),\
                team->name,\
                team->goals+1\
              );
    }

#if DEBUG > 0
    printf("Goal Chance at time = %lld is exiting\n", goal->time);
#endif

    return NULL;
}

int main(int argc, char **argv)
{
    /* Take input */
    zone_H = new_zone_from_input('H');
    zone_A = new_zone_from_input('A');
    zone_N = new_zone_from_input('N');

    scanf("%lld", &spectating_time);
    scanf("%lld", &num_groups);

#if DEBUG > 0
    print_zone(zone_H);
    print_zone(zone_A);
    print_zone(zone_N);
#endif

    all_groups = malloc(num_groups * sizeof(Group*));

    for (llint grp_num=0; grp_num<num_groups; grp_num++)
    {
        llint num_people;
        scanf("%lld", &num_people);
        Group *group = new_group_from_input(grp_num, num_people);
        all_groups[grp_num] = group;
        for (llint person_num=0; person_num<num_people; person_num++)
        {
            Person *person = new_person_from_input(grp_num);
#if DEBUG > 0
            print_person(person);
#endif
            group->people[person_num] = person;
        }
    }

    scanf("%lld", &num_goal_chances);

    all_goal_chances = malloc(num_goal_chances * sizeof(GoalChance*));

    for (llint goal_chance_num=0; goal_chance_num<num_goal_chances; goal_chance_num++)
    {
        all_goal_chances[goal_chance_num] = new_goal_chance_from_input();
#if DEBUG > 0
        print_goal_chance(all_goal_chances[goal_chance_num]);
#endif
    }

    away_team = new_team('A');
    home_team = new_team('H');

    sem_init(&seats_waiting_sem[HOME_TEAM_SUPPORTERS], 0, 0);
    sem_init(&seats_waiting_sem[AWAY_TEAM_SUPPORTERS], 0, 0);
    sem_init(&seats_waiting_sem[NEUTRAL_SUPPORTERS], 0, 0);

    clock_gettime(CLOCK_REALTIME, &start_time);

    for (llint group_num=0; group_num<num_groups; group_num++)
    {
        Group *group = all_groups[group_num];
        for (llint person_num=0; person_num<group->num_people; person_num++)
        {
            Person *person = group->people[person_num];
            pthread_create(&person->thread, NULL, people_thread, person);
        }
    }

    for (llint goal_chance_num=0; goal_chance_num<num_goal_chances; goal_chance_num++)
    {
        GoalChance *gc = all_goal_chances[goal_chance_num];
        pthread_create(&gc->thread, NULL, goals_thread, (void*)gc);
    }

    for (llint group_num=0; group_num<num_groups; group_num++)
    {
        Group *group = all_groups[group_num];
        for (llint person_num=0; person_num<group->num_people; person_num++)
        {
            Person *person = group->people[person_num];
            pthread_join(person->thread, NULL);
        }
    }

    for (llint goal_chance_num=0; goal_chance_num<num_goal_chances; goal_chance_num++)
    {
        GoalChance *gc = all_goal_chances[goal_chance_num];
        pthread_join(gc->thread, NULL);
    }

    return 0;
}
