#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <syslog.h>
#include <signal.h>
#include "was_globals.h"
#include "was_structs.h"
#include "was_functions.h"
char* get_curr_time();
main()
{
    char *currdt = get_curr_time();
    printf("Port sequence first:%d, second:%d\n",PORT1,PORT2);
    printf("Daemon has started at:%s\n",get_curr_time());
    return 0;
}

