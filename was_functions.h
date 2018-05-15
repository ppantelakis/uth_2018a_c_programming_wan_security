//Function for getting current timestamp in milliseconds
//https://stackoverflow.com/questions/12722904/how-to-use-struct-timeval-to-get-the-execution-time
long int was_get_curr_time_ms();

//Function for getting current timestamp in human format
char* was_get_curr_time();

//Function for insert iptables rule to iptables firewall
char* was_iptables_add_rule( char * ip );

//Function for application signals
//http://www.gnu.org/software/libc/manual/html_node/Signal-Handling.html
//https://www.thegeekstuff.com/2012/03/catch-signals-sample-c-code/
void was_app_signals( int sig );

void was_enable_signals();

//Created function for Daemon
//https://stackoverflow.com/questions/17954432/creating-a-daemon-in-linux

void was_daemon();

//Check if a specific struct in position i exists to the array of ips
//Return true if exists, false if not
int was_iplog_find_from_pos(long);

//Check if IP is blocked
int was_iplog_is_blocked(long);

//Block IP for PORT_SCANNING_WAIT
void was_iplog_block(long);

//Check if a specific ip exists to the array of ips
//Return -1 if does not exist this ip
long was_iplog_find(struct in_addr);

//Remove ip from array of ips
void was_iplog_remove(long);

//Add an ip to array of ips
void was_iplog_add(struct in_addr,long);

int was_listen();
