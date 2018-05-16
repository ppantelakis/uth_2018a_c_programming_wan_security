#include <stdio.h>
//#include <stdlib.h>
//#include <time.h>
#include <syslog.h>
//#include <signal.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//http://pubs.opengroup.org/onlinepubs/7908799/xns/arpainet.h.html
//#include <arpa/inet.h>
#include <string.h>
//#include "was_tcp_globals.h"
//#include "was_structs.h"


main()
{
    printf("Daemon has started at:%s\n",was_get_curr_time());
    was_enable_signals();
    //was_daemon();
    was_tcp_listen();
    //was_listen_port1();
    syslog( LOG_INFO, "WAS is exiting!" );
    return 0;
}
