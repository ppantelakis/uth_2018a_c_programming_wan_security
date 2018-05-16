#include "was_functions.h"
#include "was_tcp_globals.h"
#include "was_tcp_structs.h"

//Check if a specific struct in position i exists to the array of ips
//Return true if exists, false if not
int was_tcp_iplog_find_from_pos(long);

//Check if IP is blocked
int was_tcp_iplog_is_blocked(long);

//Block IP for PORT_SCANNING_WAIT
void was_tcp_iplog_block(long);

//Check if a specific ip exists to the array of ips
//Return -1 if does not exist this ip
long was_tcp_iplog_find(struct in_addr);

//Remove ip from array of ips
void was_tcp_iplog_remove(long);

//Allocate memory for new ip
int was_tcp_iplog_alloc();

//Add an ip to array of ips
void was_tcp_iplog_add(struct in_addr,long);

int was_tcp_listen();

