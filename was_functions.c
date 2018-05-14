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
#include "was_globals.h"
#include "was_structs.h"
#include "was_functions.h"
long int was_get_curr_time_ms()
{
    struct timeval now;
    int rc;

    rc=gettimeofday(&now, NULL);
    return ((now.tv_sec*1000000L)+now.tv_usec);
}
char* was_get_curr_time()
{
    //We need to use pointer due to return the address of variable, otherwise the variable is local
    char *buff_time = malloc(100 * sizeof(char));
    time_t now = time (0);
    strftime (buff_time, 100, "%Y-%m-%d %H:%M:%S", localtime (&now));
    return buff_time;
}
char* was_iptables_add_rule( char * ip )
{
    static char cmd[ CMD_SIZE ];
    //iptables -I INPUT 2 -p tcp -m state --state NEW -m tcp -s 8.8.8.8 -m comment --comment \"was opened port at 12:23 28/02/2018\"  -j ACCEPT
    sprintf( cmd, "iptables -I INPUT %d -p tcp -m state --state NEW -m tcp -s %s -m comment --comment \"was opened port at %s\"  -j ACCEPT", CMD_LINE, ip,  was_get_curr_time());
    syslog( LOG_AUTH, "Executed command : %s ", cmd);
    system( cmd );
    return cmd;
}
void was_app_signals( int sig )
{
    if(sig==SIGHUP)
    {
        syslog(LOG_WARNING, "Received SIGHUP signal.");
        exit(EXIT_FAILURE);
    }
    else if(sig==SIGTERM)
    {
        syslog(LOG_WARNING, "Received SIGTERM signal.");
        exit(EXIT_FAILURE);
    }
    else if(sig==SIGINT)
    {
        syslog(LOG_WARNING, "Received SIGINT signal.");
        exit(EXIT_FAILURE);
    }
    else if(sig==SIGINT)
    {
        syslog(LOG_WARNING, "Received SIGQUIT signal.");
        exit(EXIT_FAILURE);
    }
    else
    {
        syslog(LOG_WARNING, "Unhandled signal (%d) %s", sig, strsignal(sig) );
    }

}

void was_enable_signals()
{
    //Set the log level
    //http://www.gnu.org/software/libc/manual/html_node/setlogmask.html
    //https://linux.die.net/man/3/setlogmask
    setlogmask( LOG_UPTO(LOG_INFO) );
    //http://www.gnu.org/software/libc/manual/html_node/openlog.html
    openlog( "WAS", LOG_CONS, LOG_USER );
    //Initialize signals
    signal(SIGHUP, was_app_signals);
    signal(SIGTERM, was_app_signals);
    signal(SIGINT, was_app_signals);
    signal(SIGQUIT, was_app_signals);
    //Start the Daemon process the core of the programm
    syslog( LOG_INFO, "WAS is starting up" );
    syslog( LOG_INFO, "WAS is trying to start Daemon" );
}
void was_daemon()
{
    pid_t pid, sid; //pid: Main proccess id, sid:Child proccess id where is running as daemon
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
}

void was_listen_port2(struct in_addr addr)
{
    int fd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    char read_buffer[BUFFER_SIZE];
    struct ipheader *was_ip = (struct ipheader *) read_buffer;
    struct tcpheader *was_tcp = (struct tcpheader *) (read_buffer + sizeof(struct ipheader));
    //Get current timestamp
    time_t cur_timestamp = time( NULL );
    //IPV4 size 16
    char in_ipaddr[ 16 ];
    while(1)
    {
        if(read(fd, read_buffer, BUFFER_SIZE) > 0)
        {
            //Check if this packet is from the same source. If not then ignore the following checks
            if( addr.s_addr == was_ip->ip_src )
            {
                //If current timestamp > wait timeout then break and wait for first port hit again
                if( time( NULL ) > cur_timestamp + MAX_WAIT_SECOND_HIT )
                {
                    syslog( LOG_AUTH, "Timeout limit passed on second hit for ip: %s !", in_ipaddr);
                    break;
                }
                //Detect serial port scanning
                //https://en.wikipedia.org/wiki/Port_scanner
                else if( htons(was_tcp->th_dport) == PORT1 + 1 || htons(was_tcp->th_dport) == PORT1 - 1 )
                {
                    syslog( LOG_AUTH, "Port scanning detected on second port hit from ip: %s !", in_ipaddr);
                    time_t cur_timestamp2 = time( NULL );
                    while(1)
                    {
                        if( time( NULL ) > cur_timestamp2 + PORT_SCANNING_WAIT )
                        {
                            break;
                        }
                    }
                    break;
                }
                else
                {
                    //https://linux.die.net/man/3/inet_ntoa
                    //Copy internet address to string
                    strcpy( in_ipaddr, inet_ntoa( addr ) );
                    //Check if second port is hitted
                    if( htons(was_tcp->th_dport) == PORT2 )
                    {
                        syslog( LOG_AUTH, "Success combination of ports hits for ip: %s !", in_ipaddr);
                        was_iptables_add_rule(in_ipaddr);
                        break;
                    }
                }
            }
        }
        else
        {
            syslog( LOG_ERR, "Could not read from network while waiting for second hit. Check if the user has rights to read from network!" );
            break;
        }
    }
}

void was_listen()
{
    //Open a socket for IPV4, raw socket, tcp protocol
    //http://man7.org/linux/man-pages/man2/socket.2.html
    int fd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    char read_buffer[BUFFER_SIZE];
    //http://www.tenouk.com/Module43a.html
    //Raw socket anatomy http://www.cs.binghamton.edu/~steflik/cs455/rawip.txt
    //The (simplified) link layer model looks like this:
    //Physical layer -> Device layer (Ethernet protocol) -> Network layer (IP) ->
    //Transport layer (TCP, UDP, ICMP) -> Session layer (application specific data)
    //Ip buffer
    struct ipheader *was_ip = (struct ipheader *) read_buffer;
    //Tcp buffer
    struct tcpheader *was_tcp = (struct tcpheader *) (read_buffer + sizeof(struct ipheader));
    while(1)
    {
        //TODO : Check for port scanning
        //http://codewiki.wikidot.com/c:system-calls:read
        //Waiting for first port hit
        if(read(fd, read_buffer, BUFFER_SIZE) > 0)
        {
            //Check if tcp port is equal with port1
            if( htons(was_tcp->th_dport) == PORT1 )
            {

                //Create a local variable of type in_addr
                struct in_addr addr;
                //In local variable addr.s_addr set the value of source ip
                addr.s_addr = was_ip->ip_src;
                //Waiting for second port hit for this IP
                was_listen_port2(addr);
            }
        }
        else
        {
            syslog( LOG_ERR, "Could not read from network while waiting for first hit. Check if the user has rights to read from network!" );
            break;
        }
    }
}
