# NetworkSecurity
We are trying to write our own protocol and backend system to learn and protect against different network attacks, such as SQL-injection, DOS, Buffer Overflow, D-DOS and more.

This requires us to create our own raw socket, and then writing up our own protocol.

Here are more infomation about our project. 

As of right now, please use 'lo' as your interface for the socket (loopback device). Because to send things over real network, we need to set up the Ethernet header, IP header, UDP header and then the payload(your data). Due to the fact that we are on a time frame to test and fix certain network attacks, and limited actual hardware (server), we will not implement the headers for now. We will just implement the payload and uses a single machine feedig on the loopback device. This should not affect the effect of various network attacks, because we are still acting as a network, but just not wired to multiple machines. All logics are still the same for various network attacks to work.

IS IT CURRENTLY WIP

# Envorinment
This project is developed on Ubuntu 18.04.2 LTS bionic

# Dependency
 * make
 * g++

# Tools
In tools/ you will find some quick src file that will help you find important information in order for program to run. The function of each file are self explanatory from the filename. I would suggest worrying about these file only when the problem comes up when you try to run our codes.

# Testing

cd to test/

Here you will find modular tests for specific class under test/class
You will also find modular tests for specific interaction under test/interaction --> this is meant for test like DOS attack, protocol, sql-injection etc.

In each of the directory that is meant for testing, it will have test_*_main.cpp, a make file, and any other nesscary file.

To do the automated testing, run the following command:

run "make all"

run "sudo ./test_*"     --> will need sudo command, there is another way to run this without sudo, but need code refactor i think, or some sort of permission granted user.

And to clean the directory, run:

run "make clean"