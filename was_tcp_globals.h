#include "was_globals.h"
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

//Array of ips knocket daemon
struct tcp_iplog_t *tcp_iplog_ptr;

//Total longth of array of ips
long tcp_tot_iplog;

