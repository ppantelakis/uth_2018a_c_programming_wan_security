#ifndef was_globals
    #include "was_globals.h"
    #define was_globals 1
#endif

//First Port
#define tcp_PORT1 43657

//Second Port
#define tcp_PORT2 55981

//Read buffer size
#define tcp_BUFFER_SIZE      8192

//How many seconds wait for second hit from same IP
//Be carefully with this constant on slow networks
#define tcp_MAX_WAIT_SECOND_HIT 4

//Wait time when port scanning is in action
#define tcp_PORT_SCANNING_WAIT 55

//Command line in iptables usually 2-3 first lines are used for loopback and established connections
#define tcp_IPTABLES_CMD_LINE        3

//Array of ips knocked daemon
struct tcp_iplog_t *tcp_iplog_ptr;
