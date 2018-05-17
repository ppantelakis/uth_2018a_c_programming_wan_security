#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <syslog.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
//http://pubs.opengroup.org/onlinepubs/7908799/xns/arpainet.h.html
#include <arpa/inet.h>
#include <string.h>
#include <stdbool.h>

#ifndef was_globals
    #include "was_globals.h"
    #define was_globals 1
#endif

//Function for getting current timestamp in milliseconds
//https://stackoverflow.com/questions/12722904/how-to-use-struct-timeval-to-get-the-execution-time
long int was_get_curr_time_ms();

//Function for getting current timestamp in human format
char* was_get_curr_time();

//Function for execute a command to system
char* was_system_exec_command( char * );

//Function for application signals
//http://www.gnu.org/software/libc/manual/html_node/Signal-Handling.html
//https://www.thegeekstuff.com/2012/03/catch-signals-sample-c-code/
void was_app_signals( int sig );

void was_enable_signals();

//Created function for Daemon
//https://stackoverflow.com/questions/17954432/creating-a-daemon-in-linux

void was_daemon();
