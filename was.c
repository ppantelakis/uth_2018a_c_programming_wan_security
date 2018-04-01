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
    pid_t pid, sid; //pid: Main proccess id, sid:Child proccess id where is running as daemon
    printf("Port sequence first:%d, second:%d\n",PORT1,PORT2);
    printf("Daemon has started at:%s\n",get_curr_time());
    //Set the log level
    //http://www.gnu.org/software/libc/manual/html_node/setlogmask.html
    //https://linux.die.net/man/3/setlogmask
    setlogmask( LOG_UPTO(LOG_INFO) );
    //http://www.gnu.org/software/libc/manual/html_node/openlog.html
    openlog( "WAS", LOG_CONS, LOG_USER );
    //Initialize signals
    signal(SIGHUP, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGQUIT, signal_handler);
    //Start the Daemon process the core of the programm
    syslog( LOG_INFO, "WAS is starting up" ); 
    

    return 0;
}

