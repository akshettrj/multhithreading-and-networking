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

#endif
