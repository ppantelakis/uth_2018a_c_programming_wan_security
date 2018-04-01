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
    syslog( LOG_INFO, "WAS is trying to start Daemon" ); 
    //http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/create.html
    pid = fork();
    if (pid < 0) 
    {
        //Couldn't create a child proccess
        exit(EXIT_FAILURE);
    }
    else if (pid > 0) 
    {
        //We are in the main proccess, the operation succeded to create a child proccess so we can exit the main proccess
        exit(EXIT_SUCCESS);
    }
    else
    {
        // Create a new SID for the child process
        //https://linux.die.net/man/2/setsid
        sid = setsid();
        if (sid < 0) {
            // could not create a new session
            exit(EXIT_FAILURE);
        }
    }
    

    

    return 0;
}

