char* iptables_add_rule( char * ip )
{
    static char cmd[ CMD_SIZE ];
    //iptables -I INPUT 2 -p tcp -m state --state NEW -m tcp -s 8.8.8.8 -m comment --comment \"was opened port at 12:23 28/02/2018\"  -j ACCEPT
}