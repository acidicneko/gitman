#pragma once

#include <iostream>
#include <vector>
#include <gitman.hpp>

#define DEFAULT_DATABASE_LOCATION DEFAULT_ROOT_LOCATION + "/db.ini"

namespace db{
    class package{
        public:
        std::string name;
        std::string desc;
        std::string repo;
    };

    std::vector<std::string> get_repos();
    bool has_package(std::string package_name, std::string repository);
    bool has_repo(std::string repo_name);
    package get_package(std::string package_name);
}