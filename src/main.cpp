#include <iostream>
#include <gitman.hpp>
#include <exit.hpp>
#include <db.hpp>
#include <getopt.h>
#include <signal.h>

int main(int argc, char* argv[]){
    signal(SIGINT, signal_callback_handler);
    acquire_lock();

     while (1) {
        option longopts[] = {
            {"search", required_argument, NULL, 's'},
            {"install", required_argument, NULL, 'i'},
            {0}};

        const int opt = getopt_long(argc, argv, "s:i:", longopts, 0);

        if (opt == -1) {
            break;
        }

        switch (opt) {
            case 's':
                search_package(optarg);
                break;
            case 'i':
                install_package(optarg);
                break;
        }
    }
    exit_seq();
    return 0;
}