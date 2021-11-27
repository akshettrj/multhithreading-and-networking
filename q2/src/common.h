#ifndef __Q2_COMMON_H
#define __Q2_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include "utils.h"

typedef long long int llint;

#define PERSON_NAME_LEN 50

#define HOME_TEAM_SUPPORTERS 0
#define AWAY_TEAM_SUPPORTERS 1
#define NEUTRAL_SUPPORTERS 2

#define DEBUG 0
#define INFO_LEVEL 0

// Ascii codes for 256-bit colors
#define COLOR_BLACK   "\033[0;30m"
#define COLOR_RED     "\033[0;31m"
#define COLOR_GREEN   "\033[0;32m"
#define COLOR_YELLOW  "\033[0;33m"
#define COLOR_BLUE    "\033[0;34m"
#define COLOR_MAGENTA "\033[0;35m"
#define COLOR_CYAN    "\033[0;36m"
#define COLOR_WHITE   "\033[0;37m"
#define COLOR_RESET   "\033[0m"

#define COLOR_GREEN_BOLD   "\033[1;32m"
#define COLOR_RED_BOLD     "\033[1;31m"
#define TEXT_UNDERLINE     "\033[4m"
#define TEXT_BOLD          "\033[1m"

#endif
