# uth_2018a_c_programming_wan_security
# A coding exercise for the lesson C programming from me and my classmate George Mitrou for our Master in IT Security and Big Data of University of Thessaly Semester A 2018  (an extra security measure for wan perimeter ) 
# Developers
# Panagiotis Pantelakis panagiotis.pantelakis@gmail.com
# George Mitrou georgebitzi@gmail.com
# On bash run as root user
# make
# after that command a file was.o will be created which is the executable
# In linux bash shell you can run
# sudo killall was.o;make clean;make;sudo ./was.o
# and then from bash shell you can run
# echo "" > /dev/tcp/localhost/43657;echo "" > /dev/tcp/localhost/55981
# you can replace localhost with the ip of the server where is running the daemon
