#include <iostream>
#include <gitman.hpp>
#include <exit.hpp>
#include <db.hpp>
#include <getopt.h>
#include <signal.h>
#include <utils.h>

int main(int argc, char* argv[]){
    signal(SIGINT, signal_callback_handler);
    set_env("GITMAN_ROOT", GetEnv("HOME") + "/.local/share/gitman");
    set_env("GITMAN_BIN", GetEnv("HOME") + "/.local/share/gitman/bin");
    acquire_lock();

     while (1) {
        option longopts[] = {
            {"search", required_argument, NULL, 's'},
            {"install", required_argument, NULL, 'i'},
            {"remove", required_argument, NULL, 'r'},
            {"help", no_argument, NULL, 'h'},
            {0}};

        const int opt = getopt_long(argc, argv, "s:i:r:vu:", longopts, 0);

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
            case 'r':
                uninstall_package(optarg);
                break;
            case 'u':
                update_package(optarg);
                break;
            case 'v':
                std::cout << "GitMan v1.0.0; A github based source package manager.\n" <<
                                "Copyright acidicneko 2024 https://github.com/acidicneko" << std::endl;
                break;
        }
    }
    exit_seq();
    return 0;
}