#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <syslog.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "was_globals.h"
#include "was_structs.h"
#include "was_functions.h"

main()
{
    //Open a socket for IPV4, raw socket, tcp protocol
    //http://man7.org/linux/man-pages/man2/socket.2.html
    int fd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    char read_buffer[BUFFER_SIZE];

    printf("Port sequence first:%d, second:%d\n",PORT1,PORT2);
    printf("Daemon has started at:%s\n",was_get_curr_time());
    was_enable_signals();
    was_daemon();
    while(1)
    {
        //http://codewiki.wikidot.com/c:system-calls:read
        if(read(fd, read_buffer, BUFFER_SIZE) > 0)
        {
            //IPV4 size 16
            char in_ipaddr[ 16 ];
        }
        else
        {
            syslog( LOG_ERR, "Could not read from network. Check if the user has rights to read from network!" );
            break;
        }
    }

    syslog( LOG_INFO, "WAS is exiting!" );
    return 0;
}

