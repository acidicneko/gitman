#include <db.hpp>
#include <exit.hpp>
#include <ini.h>

std::vector<std::string> db::get_repos(){
    mINI::INIFile file(DEFAULT_DATABASE_LOCATION);
    mINI::INIStructure ini;
    file.read(ini);
    if(!ini.has("config")){
        std::cout << "\033[1;31mERROR\033[0m: Database file doesn't contain config section." << std::endl;
        exit_fail();
    }
    std::string repos_key = ini["config"]["repos"];
    std::vector <std::string> repos;
    std::stringstream check1(repos_key);
    std::string intermediate;
    while(getline(check1, intermediate, ' '))
    {
        repos.push_back(intermediate);
    }
    return repos;
}

bool db::has_repo(std::string repo_name){
    mINI::INIFile file(DEFAULT_DATABASE_LOCATION);
    mINI::INIStructure ini;
    file.read(ini);
    return ini.has(repo_name);
}

bool db::has_package(std::string package_name, std::string repository){
    mINI::INIFile file(DEFAULT_DATABASE_LOCATION);
    mINI::INIStructure ini;
    file.read(ini);
    if(!db::has_repo(repository)){
        std::cout << "\033[1;31mERROR\033[0m: No such repository found: " << repository << std::endl;
        exit_fail();
    }
    auto& collection = ini[repository];
    return collection.has(package_name);
}

db::package db::get_package(std::string package_name){
    mINI::INIFile file(DEFAULT_DATABASE_LOCATION);
    mINI::INIStructure ini;
    file.read(ini);
    std::vector<std::string> repos = db::get_repos();
    db::package pkg;
    for(int i = 0; i < repos.size(); i++){
        if(db::has_package(package_name, repos[i])){
            pkg.name = package_name;
            pkg.desc = ini[repos[i]][package_name];
            pkg.repo = repos[i];
            return pkg;
        }
    }
    std::cout << "\033[1;31mERROR\033[0m: No such package found: " << package_name << std::endl;
    exit_fail();
    return pkg;
}