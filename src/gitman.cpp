#include <cJSON.h>
#include <gitman.hpp>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <exit.hpp>
#include <unistd.h>

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

void cleanup(){
    std::string rm = "rm -rf ";
    execute_cmd(rm + DEFAULT_CLONE_LOCATION);
}

void clone_repo(pkg temp){
    execute_cmd("git clone https://github.com/" + temp.repo + " " + DEFAULT_CLONE_LOCATION + temp.name);
}

bool is_installed(std::string package_name){
    return folder_exist(DEFAULT_ROOT_LOCATION + "/packages/" + package_name + "/last_commit");
}

void local_change_dir(std::string dir){
    int rc = chdir(dir.c_str());
    if (rc < 0) {
        std::cout << "\033[1;31mERROR\033[0m: Error occured while changing directory." << std::endl;
        exit_fail();
    }
}

void local_package_install(pkg temp){
    local_change_dir(DEFAULT_CLONE_LOCATION + temp.name);
    execute_cmd("./gitman.sh install");
    execute_cmd("cp gitman.sh " + DEFAULT_ROOT_LOCATION + "/packages/" + temp.name);
    local_change_dir(DEFAULT_ROOT_LOCATION);
}

void local_package_update(pkg temp){
    local_change_dir(DEFAULT_CLONE_LOCATION + temp.name);
    execute_cmd("./gitman.sh update");
    execute_cmd("cp gitman.sh " + DEFAULT_ROOT_LOCATION + "/packages/" + temp.name);
    local_change_dir(DEFAULT_ROOT_LOCATION);
}

void local_package_uninstall(pkg temp){
    execute_cmd(DEFAULT_ROOT_LOCATION + "/packages/" + temp.name + "/gitman.sh uninstall");
    execute_cmd("rm -rf " + DEFAULT_ROOT_LOCATION + "/packages/" + temp.name);
}

cJSON* getPackageJSON(){
    std::ifstream jsonFile(DEFAULT_PACKAGE_JSON);
    std::string jsonString((std::istreambuf_iterator<char>(jsonFile)),
                 std::istreambuf_iterator<char>());

    
    cJSON* packageJSON = cJSON_Parse(jsonString.c_str());
    if (packageJSON== NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        exit(EXIT_FAILURE);
    }
    cJSON *packages = NULL;

    packages = cJSON_GetObjectItemCaseSensitive(packageJSON, "packages");
    //cJSON_Delete(packageJSON);
    return packages;
}

void syncPackages(){
    bool synced = false;
    int synced_number = 0;
    cJSON* package = NULL;
    cJSON* packages = getPackageJSON();
    cJSON_ArrayForEach(package, packages){
        cJSON *name = cJSON_GetObjectItemCaseSensitive(package, "name");
        cJSON *repo = cJSON_GetObjectItemCaseSensitive(package, "repo");
        cJSON *branch = cJSON_GetObjectItemCaseSensitive(package, "branch");
        cJSON *type = cJSON_GetObjectItemCaseSensitive(package, "type");
        pkg temp;
        temp.name = name->valuestring;
        temp.repo = repo->valuestring;
        temp.branch = branch->valuestring;
        if(is_installed(temp.name)){
            std::cout << "\033[1;32mINFO\033[0m: Skipping: " << temp.name << std::endl;
        } else {
            clone_repo(temp);
            if(!file_exist(DEFAULT_CLONE_LOCATION + temp.name + "/gitman.sh")){
                std::cout << "\033[1;31mERROR\033[0m: [" << temp.repo << "] isn't GitMan compatible! Skipping..." << std::endl;
            } else {
                create_folder(DEFAULT_ROOT_LOCATION + "/packages/" + temp.name);
                local_package_install(temp);
                std::cout << "Generating hash..." << std::endl;
                if(execute_cmd(DEFAULT_ROOT_LOCATION + "/create_commit_hash.sh " + temp.name + " " 
                                + temp.repo + " " + temp.branch) != 0){
                    std::cout << "\033[1;31mERROR\033[0m: Error while creating hash." << std::endl;
                    exit_fail();
                }
                std::cout << "\033[1;32mINFO\033[0m: Package installed: " << temp.name << std::endl;
                synced = true;
            }
        }
    }
    if(!synced) std::cout << "Already synced. There\'s nothing to do" << std::endl;
    else std::cout << "Sync Complete!" << std::endl;
    std::cout << "Synced " << synced_number << " packages." << std::endl;
    cleanup();
}

void syncPackagesUpdate(){
    bool synced = false;
    int synced_number =  0;
    cJSON* package = NULL;
    cJSON* packages = getPackageJSON();
    cJSON_ArrayForEach(package, packages){
        cJSON *name = cJSON_GetObjectItemCaseSensitive(package, "name");
        cJSON *repo = cJSON_GetObjectItemCaseSensitive(package, "repo");
        cJSON *branch = cJSON_GetObjectItemCaseSensitive(package, "branch");
        cJSON *type = cJSON_GetObjectItemCaseSensitive(package, "type");
        pkg temp;
        temp.name = name->valuestring;
        temp.repo = repo->valuestring;
        temp.branch = branch->valuestring;
        if(!is_installed(temp.name)){
            std::cout << "\033[1;32mINFO\033[0m: Skipping non-installed: " << temp.name << std::endl;
        } else {
            int status = execute_cmd(DEFAULT_ROOT_LOCATION + "/check_commit_update.sh " + temp.name + " " 
                                + temp.repo + " " + temp.branch);
            if(status == 256){
                clone_repo(temp);
                if(!file_exist(DEFAULT_CLONE_LOCATION + temp.name + "/gitman.sh")){
                    std::cout << "\033[1;31mERROR\033[0m: [" << temp.repo << "] isn't GitMan compatible! Skipping..." << std::endl;
                } else {
                    local_package_update(temp);
                    std::cout << "Generating hash..." << std::endl;
                    if(execute_cmd(DEFAULT_ROOT_LOCATION + "/create_commit_hash.sh " + temp.name + " " 
                                    + temp.repo + " " + temp.branch) != 0){
                        std::cout << "\033[1;31mERROR\033[0m: Error while creating hash." << std::endl;
                        exit_fail();
                    }
                    std::cout << "\033[1;32mINFO\033[0m: Package updated: " << temp.name << std::endl;
                    synced_number++;
                    synced = true;
                }
            }
        }
    }
    if(!synced) std::cout << "Already synced. There\'s nothing to do" << std::endl;
    else std::cout << "Sync Complete!" << std::endl;
    std::cout << "Synced " << synced_number << " packages." << std::endl;
    cleanup();
}