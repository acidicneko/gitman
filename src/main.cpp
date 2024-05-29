#include <iostream>
#include <gitman.hpp>
#include <exit.hpp>
#include <getopt.h>
#include <signal.h>
#include <util.hpp>

int main(int argc, char** argv){
    signal(SIGINT, signal_callback_handler);
    set_env("GITMAN_ROOT", DEFAULT_ROOT_LOCATION);
    set_env("GITMAN_BIN", DEFAULT_ROOT_LOCATION + "/bin");
    acquire_lock();

    option longopts[] = {
        {"sync", no_argument, NULL, 's'},
        {"update", no_argument, NULL, 'u'},
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'v'},
        {0}
    };
     while (1) {
        const int opt = getopt_long(argc, argv, "svu", longopts, 0);

        if (opt == -1) {
            break;
        }

        switch (opt) {
            case 's':
                syncPackages();
                break;
            case 'u':
                syncPackagesUpdate();
                break;
            case 'v':
                std::cout << "GitMan v2.0.0; A github based source package manager.\n" <<
                                "Copyright acidicneko 2024 https://github.com/acidicneko" << std::endl;
                break;
            default:
                std::cout << "I'm bored." << std::endl;
                break;
        }
    }
    exit_seq();
    return 0;
}