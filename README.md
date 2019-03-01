# Dependency
 * make
 * g++

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