#ifndef __Q3_COMMON_H
#define __Q3_COMMON_H

#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>     /* for variadic function */
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <pthread.h>
#include <queue>

#define SERVER_PORT 6969
#define MAXLINE 4096
#define DICTIONARY_SIZE 101
#define SA struct sockaddr

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

#endif
