#ifndef was_tcp_functions
    #include "was_tcp_functions.h"
    #define was_tcp_functions 1
#endif

char* was_tcp_iptables_add_rule( char * ip )
{
    static char cmd[ IPTABLES_CMD_SIZE ];
    //iptables -I INPUT 2 -p tcp -m state --state NEW -m tcp -s 8.8.8.8 -m comment --comment \"was opened port at 12:23 28/02/2018\"  -j ACCEPT
    sprintf( cmd, "iptables -I INPUT %d -p tcp -m state --state NEW -m tcp -s %s -m comment --comment \"was opened port at %s\"  -j ACCEPT", tcp_IPTABLES_CMD_LINE, ip,  was_get_curr_time());
    return was_system_exec_command(cmd);
}

struct tcp_iplog_t * was_tcp_iplog_get_last()
{
    struct tcp_iplog_t *ret = NULL;
    if(tcp_iplog_ptr)
    {
        ret = tcp_iplog_ptr;
        while(ret->next)
        {
            ret=ret->next;
        }
    }
    return ret;
}

long was_tcp_iplog_get_total()
{
    long ret = 0;
    struct tcp_iplog_t * tmp;
    if(tcp_iplog_ptr)
    {
        tmp = tcp_iplog_ptr;
        while(tmp->next)
        {
            tmp = tmp->next;
            ret++;
        }
        ret++;
    }
    return ret;
}

struct tcp_iplog_t * was_tcp_iplog_find(struct in_addr addr)
{
    struct tcp_iplog_t * ret = NULL;
    if(tcp_iplog_ptr)
    {
        ret = tcp_iplog_ptr;
        while(ret->next)
        {
            if(ret->addr.s_addr = addr.s_addr)
            {
                break;
            }
            else
            {
                ret = ret->next;
            }

        }
        if(ret->addr.s_addr != addr.s_addr)
        {
            ret = NULL;
        }
    }
    return ret;
}

int was_tcp_iplog_is_blocked(struct tcp_iplog_t *l_iplog_ptr)
{
    int ret = true;
    if(!l_iplog_ptr || l_iplog_ptr->blocked_until_time<time( NULL ))
    {
        ret = false;
    }
    return ret;
}

void was_tcp_iplog_block(struct tcp_iplog_t *l_iplog_ptr)
{
    char in_ipaddr[ 16 ];
    if(l_iplog_ptr)
    {
        strcpy( in_ipaddr, inet_ntoa( l_iplog_ptr->addr ) );
        l_iplog_ptr->blocked_until_time = time( NULL ) + tcp_PORT_SCANNING_WAIT;
        syslog( LOG_ERR, "The ip %s has been blocked due to suspicius operations!", in_ipaddr);
    }
    return;
}



void was_tcp_iplog_remove(struct tcp_iplog_t *l_iplog_ptr)
{
    if(l_iplog_ptr)
    {
        if(l_iplog_ptr->next)
        {
            l_iplog_ptr->next->prev = l_iplog_ptr->prev;
        }
        if(l_iplog_ptr->prev)
        {
            l_iplog_ptr->prev->next = l_iplog_ptr->next;
        }
        syslog( LOG_WARNING, "Freeing address %p",(void*) &l_iplog_ptr);
        free(l_iplog_ptr);
        if(l_iplog_ptr==tcp_iplog_ptr)//Only one element points to first
        {
            tcp_iplog_ptr = NULL;
        }
        l_iplog_ptr = NULL;
    }
    return;
}

struct tcp_iplog_t * was_tcp_iplog_alloc()
{
    struct tcp_iplog_t * ret = NULL;
    struct tcp_iplog_t *l_iplog_ptr, *last_ptr;

    l_iplog_ptr = (struct tcp_iplog_t *) malloc(sizeof(struct tcp_iplog_t));
    //Check if malloc or realloc Succeeded
    if(!l_iplog_ptr)
    {
        syslog( LOG_ERR, "Could not allocate memory on array of ips for new ip!!!");
    }
    else
    {
        l_iplog_ptr->next = NULL;
        l_iplog_ptr->prev = NULL;

        last_ptr = was_tcp_iplog_get_last();

        //If array not initialized
        if (!last_ptr)
        {
            tcp_iplog_ptr = l_iplog_ptr;
        }
        else
        {
            last_ptr->next = l_iplog_ptr;
            l_iplog_ptr->prev = last_ptr;
        }
        ret = l_iplog_ptr;
    }
    return ret;

}

void was_tcp_iplog_add(struct in_addr addr, long port)
{
    struct tcp_iplog_t *l_iplog_ptr;
    char in_ipaddr[ 16 ];
    strcpy( in_ipaddr, inet_ntoa( addr ) );
    l_iplog_ptr = was_tcp_iplog_find(addr);
    syslog( LOG_WARNING, "Found addr %p.",(void*) &l_iplog_ptr );
    if(was_tcp_iplog_is_blocked(l_iplog_ptr)==false)
    {
        if(port==tcp_PORT1)
        {
            //If exist the ip then remove from the array of ips and continue
            syslog( LOG_WARNING, "Removing addr %p.",(void*) &l_iplog_ptr );
            was_tcp_iplog_remove(l_iplog_ptr);
            syslog( LOG_WARNING, "Removed addr %p.",(void*) &l_iplog_ptr );
            syslog( LOG_WARNING, "Before was_tcp_iplog_alloc %p.",(void*) &l_iplog_ptr );
            if((l_iplog_ptr=was_tcp_iplog_alloc()))
            {
                syslog( LOG_WARNING, "Adding addr %p ip %s.",(void*) &l_iplog_ptr, inet_ntoa( addr ) );
                syslog( LOG_WARNING, "Total added %ld.",was_tcp_iplog_get_total() );
                l_iplog_ptr->addr = addr;
                l_iplog_ptr->blocked_until_time = 0;
                l_iplog_ptr->first_time = time( NULL );
                l_iplog_ptr->current_port = port;
                syslog( LOG_WARNING, "Added addr %p ip %s on time %ld.",(void*) &l_iplog_ptr,inet_ntoa( l_iplog_ptr->addr ),l_iplog_ptr->first_time );
            }
            syslog( LOG_WARNING, "After was_tcp_iplog_alloc. Total ip objects : %ld.",was_tcp_iplog_get_total() );
        }
        else if(port==tcp_PORT2)
        {
            //If exists the address that we are looking for then check if is valid the waiting time
            if(l_iplog_ptr)
            {
                //If
                if( time( NULL ) < l_iplog_ptr->first_time + tcp_MAX_WAIT_SECOND_HIT )
                {
                    //Go and open for specific ip
                    syslog( LOG_WARNING, "Success combination of ports hits for ip: %s !", in_ipaddr);
                    was_tcp_iptables_add_rule(in_ipaddr);
                }
                else
                {
                    syslog( LOG_WARNING, "Timeout limit passed on second hit for ip: %s !", in_ipaddr);
                }
                was_tcp_iplog_remove(l_iplog_ptr);
            }
            else
            {
                syslog( LOG_ERR, "Second port hitted for ip %s but this ip does not exists in array of ips!", in_ipaddr);
            }
        }
        else
        {
            syslog( LOG_ERR, "Port not found: %ld !", port);
        }
    }
    else
    {
        syslog( LOG_ERR, "The ip %s has already been blocked due to suspicius operations!", in_ipaddr);
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
    //Create a local variable of type in_addr
    struct in_addr addr;

    tcp_iplog_ptr = NULL;

    printf("Port sequence first:%d, second:%d\n",tcp_PORT1,tcp_PORT2);

    while(1)
    {
        //TODO: I want to read the data that came with tcp
        if(read(fd, read_buffer, tcp_BUFFER_SIZE) > 0)
        {
            //Check if tcp port is equal with port1 or port2
            if( htons(was_tcp->th_dport) == tcp_PORT1 || htons(was_tcp->th_dport) == tcp_PORT2 )
            {
                syslog( LOG_ERR, "Listened to a knock on port %ld.",htons(was_tcp->th_dport) );
                //In local variable addr.s_addr set the value of source ip
                addr.s_addr = was_ip->ip_src;
                was_tcp_iplog_add(addr, htons(was_tcp->th_dport));
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
