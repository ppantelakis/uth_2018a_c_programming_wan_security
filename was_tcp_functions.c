#include "was_tcp_functions.h"

int was_tcp_iplog_find_from_pos(long i)
{
    int ret = false;
    if(i>=0)
    {
        ret = true;
    }
    return ret;
}

int was_tcp_iplog_is_blocked(long i)
{
    int ret = true;
    if(was_tcp_iplog_find_from_pos(i)==false || tcp_iplog_ptr[i].blocked_until_time<time( NULL ))
    {
        ret = false;
    }
    return ret;
}

void was_tcp_iplog_block(long i)
{
    char in_ipaddr[ 16 ];
    if(was_tcp_iplog_find_from_pos(i)==true)
    {
        strcpy( in_ipaddr, inet_ntoa( tcp_iplog_ptr[i].addr ) );
        tcp_iplog_ptr[i].blocked_until_time = time( NULL ) + tcp_PORT_SCANNING_WAIT;
        syslog( LOG_AUTH, "The ip %s has been blocked due to suspicius operations!", in_ipaddr);
    }
    return;
}

long was_tcp_iplog_find(struct in_addr addr)
{
    long i, ret = -1;
    for(i=0;i<tcp_tot_iplog;i++)
    {
        if(tcp_iplog_ptr[i].addr.s_addr == addr.s_addr)
        {
            ret = i;
        }
    }
    return ret;
}

void was_tcp_iplog_remove(long i)
{
    if(was_tcp_iplog_find_from_pos(i)==true)
    {
        tcp_iplog_ptr[i] = tcp_iplog_ptr[tcp_tot_iplog-1];
        free(&tcp_iplog_ptr[tcp_tot_iplog-1]);
        tcp_tot_iplog--;
    }
    return;
}

int was_tcp_iplog_alloc()
{
    int ret = false;
    //If array not initialized
    if (tcp_tot_iplog==0)
    {
        tcp_iplog_ptr = malloc(sizeof(struct tcp_iplog_t));
    }
    else
    {
        tcp_iplog_ptr = realloc(tcp_iplog_ptr,tcp_tot_iplog+(sizeof(struct tcp_iplog_t)));
    }
    //Check if malloc or realloc Succeeded
    if(tcp_iplog_ptr==NULL)
    {
        syslog( LOG_ERR, "Could not allocate memory on array of ips for new ip!!!");
    }
    else
    {
        tcp_tot_iplog++;
        ret = true;
    }
    return ret;

}

void was_tcp_iplog_add(struct in_addr addr, long port)
{
    long i = was_tcp_iplog_find(addr);
    char in_ipaddr[ 16 ];
    strcpy( in_ipaddr, inet_ntoa( addr ) );
    if(was_tcp_iplog_is_blocked(i)==false)
    {
        if(port==tcp_PORT1)
        {
            //If exist the ip then remove from the array of ips and continue
            if(was_tcp_iplog_find_from_pos(i)==true)
            {
                was_tcp_iplog_remove(i);
            }

            if(was_tcp_iplog_alloc()==true)
            {
                tcp_iplog_ptr[tcp_tot_iplog-1].addr = addr;
                tcp_iplog_ptr[tcp_tot_iplog-1].blocked_until_time = 0;
                tcp_iplog_ptr[tcp_tot_iplog-1].first_time = time( NULL );
                tcp_iplog_ptr[tcp_tot_iplog-1].current_port = port;
                syslog( LOG_ERR, "Added no %ld ip %s on time %ld.",tcp_tot_iplog,inet_ntoa( tcp_iplog_ptr[tcp_tot_iplog-1].addr ),tcp_iplog_ptr[tcp_tot_iplog-1].first_time );
                syslog( LOG_ERR, "Size of array of ips %d.",tcp_tot_iplog );
            }
        }
        else if(port==tcp_PORT2)
        {
            //If exists the address that we are looking for then check if is valid the waiting time
            if(was_tcp_iplog_find_from_pos(i)==true)
            {
                //If
                if( time( NULL ) < tcp_iplog_ptr[i].first_time + tcp_MAX_WAIT_SECOND_HIT )
                {
                    //Go and open for specific ip
                    syslog( LOG_AUTH, "Success combination of ports hits for ip: %s !", in_ipaddr);
                    was_iptables_add_rule(in_ipaddr);
                }
                else
                {
                    syslog( LOG_AUTH, "Timeout limit passed on second hit for ip: %s !", in_ipaddr);
                }
                was_tcp_iplog_remove(i);
            }
            else
            {
                syslog( LOG_AUTH, "Second port hitted for ip %s but this ip does not exists in array of ips!", in_ipaddr);
            }
        }
        else
        {
            syslog( LOG_AUTH, "Port not found: %ld !", port);
        }
    }
    else
    {
        syslog( LOG_AUTH, "The ip %s has already been blocked due to suspicius operations!", in_ipaddr);
    }
    return;
}

//void

int was_tcp_listen()
{
    int ret = 0;
    //Open a socket for IPV4, raw socket, tcp protocol
    //http://man7.org/linux/man-pages/man2/socket.2.html
    int fd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    char read_buffer[tcp_BUFFER_SIZE];
    //http://www.tenouk.com/Module43a.html
    //Raw socket anatomy http://www.cs.binghamton.edu/~steflik/cs455/rawip.txt
    //The (simplified) link layer model looks like this:
    //Physical layer -> Device layer (Ethernet protocol) -> Network layer (IP) ->
    //Transport layer (TCP, UDP, ICMP) -> Session layer (application specific data)
    //Ip buffer
    struct ip_header *was_ip = (struct ip_header *) read_buffer;
    //Tcp buffer
    struct tcp_header *was_tcp = (struct tcp_header *) (read_buffer + sizeof(struct ip_header));
    tcp_tot_iplog = 0;

    printf("Port sequence first:%d, second:%d\n",tcp_PORT1,tcp_PORT2);

    while(1)
    {
        if(read(fd, read_buffer, tcp_BUFFER_SIZE) > 0)
        {
            //Check if tcp port is equal with port1 or port2
            if( htons(was_tcp->th_dport) == tcp_PORT1 || htons(was_tcp->th_dport) == tcp_PORT2 )
            {

                syslog( LOG_ERR, "Listened to a knock on port %ld.",htons(was_tcp->th_dport) );
                //Create a local variable of type in_addr
                struct in_addr addr;
                //In local variable addr.s_addr set the value of source ip
                addr.s_addr = was_ip->ip_src;

                was_tcp_iplog_add(addr, htons(was_tcp->th_dport));
                //Waiting for second port hit for this IP
                //was_listen_port2(addr);
            }
        }
        else
        {
            syslog( LOG_ERR, "Could not read from network while waiting for knock. Check if the user has rights to read from network!" );
            ret = 1;
            break;
        }
    }
    return ret;
}


