#pragma once
#include <iostream>
#include <utils.h>

#define DEFAULT_ROOT_LOCATION GetEnv("HOME") + "/.local/share/gitman"
#define DEFAULT_LOCK_LOCATION DEFAULT_ROOT_LOCATION + "/gitman.lock"

typedef struct {
    std::string name;
    std::string git_repo;
    std::string commit_id;
} package_t;

void acquire_lock();
void release_lock();
void search_package(std::string package_name);
void install_package(std::string package_name);
void uninstall_package(std::string package_name);
bool is_installed(std::string package_name);