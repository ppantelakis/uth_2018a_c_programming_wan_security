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
    pid_t pid, sid; //pid: Main proccess id, sid:Child proccess id where is running as daemon
    printf("Port sequence first:%d, second:%d\n",PORT1,PORT2);
    printf("Daemon has started at:%s\n",was_get_curr_time());
    was_enable_signals();
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
    syslog( LOG_INFO, "Daemon was started successfully" ); 
    // Listen to the network and wait for someone to connect to the first port
    while( read(fd, read_buffer, BUFFER_SIZE) > 0 )
    {
        //IPV4 size 16
        char in_ipaddr[ 16 ];
    }
    
    syslog( LOG_INFO, "WAS is exiting!" ); 
    return 0;
}

