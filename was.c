#include <stdio.h>
#include <time.h>
#include "was_globals.h"
#include "was_structs.h"
#include "was_functions.h"

main()
{
    char buff_time[100];
    time_t now = time (0);
    strftime (buff_time, 100, "%Y-%m-%d %H:%M:%S", localtime (&now));
    printf("Port sequence first:%d, second:%d\n",PORT1,PORT2);
    printf("Daemon has started at:%s\n",buff_time);
    return 0;
}
