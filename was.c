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

main()
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

    printf("Port sequence first:%d, second:%d\n",PORT1,PORT2);
    printf("Daemon has started at:%s\n",was_get_curr_time());
    was_enable_signals();
    was_daemon();
    while(1)
    {
        //TODO : Check for port scanning
        //http://codewiki.wikidot.com/c:system-calls:read
        //Waiting for first port hit
        if(read(fd, read_buffer, BUFFER_SIZE) > 0)
        {
            //IPV4 size 16
            char in_ipaddr[ 16 ];
            //Check if tcp port is equal with port1
            if( htons(was_tcp->th_dport) == PORT1 )
            {
                //Get current timestamp
                time_t cur_timestamp = time( NULL );
                //Create a local variable of type in_addr
                struct in_addr addr;
                //In local variable addr.s_addr set the value of source ip
                addr.s_addr = was_ip->ip_src;

                //Waiting for second port hit
                while(1)
                {

                    if(read(fd, read_buffer, BUFFER_SIZE) > 0)
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
                    else
                    {
                        syslog( LOG_ERR, "Could not read from network while waiting for second hit. Check if the user has rights to read from network!" );
                        break;
                    }
                }
            }
        }
        else
        {
            syslog( LOG_ERR, "Could not read from network while waiting for first hit. Check if the user has rights to read from network!" );
            break;
        }
    }

    syslog( LOG_INFO, "WAS is exiting!" );
    return 0;
}
