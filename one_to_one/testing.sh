#!/bin/sh
gcc -c mythread.c list.c locks.c tests/create.c
gcc -o create mythread.o list.o locks.o create.o

gcc -c mythread.c list.c locks.c tests/join.c
gcc -o join mythread.o list.o locks.o join.o

gcc -c mythread.c list.c locks.c tests/kill.c
gcc -o kill_ mythread.o list.o locks.o kill.o

gcc -c mythread.c list.c locks.c tests/spin.c
gcc -o spin mythread.o list.o locks.o spin.o

gcc -c mythread.c list.c locks.c tests/search.c
gcc -o search mythread.o list.o locks.o search.o

echo "####### ONE-ONE TESTING STARTED #######";

./create
./join
./kill_
./spin
./search

echo "####### ONE-ONE TESTING FINISHED #######";