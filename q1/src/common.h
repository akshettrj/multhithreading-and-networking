#ifndef __Q1_COMMON_H
#define __Q1_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#define MAX_COURSES 1000
#define MAX_STUDENTS 1000
#define MAX_LABS 1000

#define COURSE_NAME_LEN 49
#define LAB_NAME_LEN 49

#define STUDENT_NUM_PREFERENCES 3

#define TUTORIAL_SEAT_ALLOTMENT_DELAY 2
#define TUTORIAL_DURATION 5
#define GAP_BETWEEN_TUTORIAL_SESSIONS 2

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

typedef long long int llint;

#endif
