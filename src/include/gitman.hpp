#pragma once

#include <iostream>
#include <util.hpp>
#include <cJSON.h>

#define DEFAULT_CLONE_LOCATION "/tmp/gitman/"
#define DEFAULT_ROOT_LOCATION GetEnv("HOME") + "/.local/share/gitman"
#define DEFAULT_LOCK_LOCATION DEFAULT_ROOT_LOCATION + "/gitman.lock"
#define DEFAULT_PACKAGE_JSON GetEnv("HOME") + "/.config/gitman/packages.json"


typedef enum {
    Commit = 0,
    Release = 1
} type_t;

class pkg{
public:
    std::string name;
    std::string repo;
    std::string branch;
    std::string id = "null";
    type_t type;
};


void acquire_lock();
void release_lock();
void syncPackages();
void syncPackagesUpdate();
bool is_installed(std::string package_name);
cJSON* getPackageJSON();
