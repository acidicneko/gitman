#include <iostream>
#include <filesystem>
#include <fstream>
#include <signal.h>
#include <gitman.hpp>
#include <ini.h>
#include <exit.hpp>
#include <db.hpp>
#include <utils.h>

void acquire_lock(){
    std::ifstream lock_file(DEFAULT_LOCK_LOCATION);
    if(lock_file){
        lock_file.close();
        std::cout << "\033[1;31mERROR\033[0m: Another gitman process is already running.\n" <<
            "Please wait for it to finish or end the process and try again.\n" <<
            "If you are sure no other Gitman process is running then delete " << DEFAULT_LOCK_LOCATION
            << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string msg = "Gitman lock acquired.\nPlease DONOT delete this file manually.\n";
    std::ofstream lock_file_out(DEFAULT_LOCK_LOCATION);
    lock_file_out << msg;
    lock_file_out.close();
}

void release_lock(){
    std::filesystem::remove(DEFAULT_LOCK_LOCATION);
}

void clone_repo(db::package pkg){
    execute_cmd("git clone " + pkg.repo + "/" + pkg.name + " /tmp/" + pkg.name);
}

void local_package_install(db::package pkg){
    std::string dir = "/tmp/" + pkg.name;
    int rc = chdir(dir.c_str());
    if (rc < 0) {
        std::cout << "\033[1;31mERROR\033[0m: Error occured while changing directory." << std::endl;
        exit_fail();
    }
    execute_cmd("./gitman_install.sh install");
    execute_cmd("cp gitman_install.sh " + DEFAULT_ROOT_LOCATION + "/packages/" + pkg.name);
}

void local_package_update(db::package pkg){
    std::string dir = "/tmp/" + pkg.name;
    int rc = chdir(dir.c_str());
    if (rc < 0) {
        std::cout << "\033[1;31mERROR\033[0m: Error occured while changing directory." << std::endl;
        exit_fail();
    }
    execute_cmd("./gitman_install.sh update");
    execute_cmd("cp gitman_install.sh " + DEFAULT_ROOT_LOCATION + "/packages/" + pkg.name);
}

void local_package_uninstall(db::package pkg){
    execute_cmd(DEFAULT_ROOT_LOCATION + "/packages/" + pkg.name + "/gitman_install.sh uninstall");
    execute_cmd("rm -rf " + DEFAULT_ROOT_LOCATION + "/packages/" + pkg.name);
}

void search_package(std::string package_name){
    db::package pkg = db::get_package(package_name);
    std::cout << "\033[1;37mPackage name\033[0m:\t " << pkg.name <<"\n"
              << "\033[1;37mDescription\033[0m:\t " << pkg.desc << "\n"
              << "\033[1;37mRepository\033[0m:\t " << pkg.repo << std::endl <<std::endl;
}


bool is_installed(std::string package_name){
    return folder_exist(DEFAULT_ROOT_LOCATION + "/packages/" + package_name + "/last_commit");
}

void install_package(std::string package_name){
    if(is_installed(package_name)){
        std::cout << "\033[1;31mERROR\033[0m: Package already installed: " << package_name <<std::endl;
        exit_fail();
    }
    db::package pkg = db::get_package(package_name);
    search_package(pkg.name);
    clone_repo(pkg);
    create_folder(DEFAULT_ROOT_LOCATION + "/packages/" + pkg.name);
    local_package_install(pkg);
    std::cout << "Generating hash..." << std::endl;
    if(execute_cmd(DEFAULT_ROOT_LOCATION + "/create_hash.sh " + pkg.name) != 0){
        std::cout << "\033[1;31mERROR\033[0m: Error while creating hash." << std::endl;
        exit_fail();
    }
    std::cout << "\033[1;32mINFO\033[0m: Package installed: " << pkg.name << std::endl;
}

void uninstall_package(std::string package_name){
    db::package pkg = db::get_package(package_name);
    if(!is_installed(pkg.name)){
        std::cout << "\033[1;31mERROR\033[0m: Package not installed: " << pkg.name <<std::endl;
        exit_fail();
    }
    search_package(pkg.name);
    local_package_uninstall(pkg);
    std::cout << "\033[1;32mINFO\033[0m: Package uninstalled: " << pkg.name << std::endl;
}

void update_package(std::string package_name){
    db::package pkg = db::get_package(package_name);
    if(execute_cmd(DEFAULT_ROOT_LOCATION + "/check_update.sh " + pkg.name) == 256){
        clone_repo(pkg);
        local_package_update(pkg);
        std::cout << "Generating hash..." << std::endl;
        if(execute_cmd(DEFAULT_ROOT_LOCATION + "/create_hash.sh " + pkg.name) != 0){
            std::cout << "\033[1;31mERROR\033[0m: Error while creating hash." << std::endl;
            exit_fail();
        }
        std::cout << "\033[1;32mINFO\033[0m: Package updated: " << pkg.name << std::endl;
    }
}