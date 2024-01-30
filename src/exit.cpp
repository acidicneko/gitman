#include <exit.hpp>
#include <gitman.hpp>
#include <iostream>

void exit_seq(){
    release_lock();
}

void signal_callback_handler(int signum){
   exit_seq();
   exit(signum);
}

void exit_fail(){
    exit_seq();
    exit(EXIT_FAILURE);
}