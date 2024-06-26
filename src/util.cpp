#include <iostream>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <sys/wait.h>
#include <unistd.h>
#include <util.hpp>

int execute_cmd(std::string command) {
  char *temp = (char *)command.c_str();
  char *argv[200];
  int argc = 0;
  argv[argc] = std::strtok(temp, " ");
  while (argv[argc]) {
    argc++;
    argv[argc] = std::strtok(0, " ");
  }

  int status;
  if (fork() == 0) {
    execvp(argv[0], argv);
  } else {
    wait(&status);
  }
  return status;
}

bool replace(std::string &str, const std::string &from, const std::string &to) {
  size_t start_pos = str.find(from);
  if (start_pos == std::string::npos)
    return false;
  str.replace(start_pos, from.length(), to);
  return true;
}

std::string GetEnv(std::string variable) {
  const char *value = getenv(variable.c_str());
  if (value == nullptr) {
    return "";
  }
  return value;
}

bool file_exist(std::string filename) {
  std::ifstream file;
  file.open(filename);
  if (!file) {
    file.close();
    return false;
  }
  file.close();
  return true;
}

bool folder_exist(std::string folderFromFile) {
  std::filesystem::path my_path = folderFromFile;
  auto dir = my_path.parent_path();
  return std::filesystem::is_directory(dir);
}

std::string get_folder_name(std::string fullPath) {
  std::filesystem::path my_path = fullPath;
  return my_path.parent_path().string();
}

std::string get_file(std::ifstream& is) {
  std::string contents;
  for (char ch; is.get(ch); contents.push_back(ch)) {}
  return contents;
}

void create_folder(std::string folderPath) {
  std::string mkdirCmd = "mkdir -p " + folderPath;
  execute_cmd(mkdirCmd);
}

void set_env(std::string var, std::string value){
  setenv(var.c_str(), value.c_str(), 1);
}