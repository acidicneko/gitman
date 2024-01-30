#pragma once

#include <string>

void execute_cmd(std::string command);
bool replace(std::string &str, const std::string &from, const std::string &to);
std::string GetEnv(std::string variable);
bool file_exist(std::string filename);
bool folder_exist(std::string folderFromFile);
std::string get_folder_name(std::string fullPath);
std::string get_file(std::ifstream& is);
void create_folder(std::string folderPath);