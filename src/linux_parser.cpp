#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

#include <cassert>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization -> DONE
float LinuxParser::MemoryUtilization() { 
  string line;
  vector<string> memoryType; //memTotal, memFree
  vector<float> memoryKB; //corresponding memory in KB
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  string temp1;
  float temp2 {0};
  if (stream.is_open()){
    int i = 0;
    while (std::getline(stream, line) && i < 2){ //First relevant 2 lines: memTotal and memFree   
        std::istringstream linestream(line);
        linestream >> temp1 >> temp2; //temp1: Name, temp2: memory value in KB
        memoryKB.emplace_back(temp2);
        i++;
    }
  }
  
  return (memoryKB[0]-memoryKB[1])/memoryKB[0]; // (Total Mem. - Free Mem.)/Total Mem. = Used Memory relative to total
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  long uptime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()){
      std::getline(stream, line); // Only 1 line
      std::istringstream linestream(line);
      linestream >> uptime;
      return uptime;
  }
  return 0;
 }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes -> DONE
int LinuxParser::TotalProcesses() {
    string line;
    std::ifstream stream(kProcDirectory + kStatFilename);
    string temp1; //key
    int temp2; //value
    if (stream.is_open()){
      while (std::getline(stream, line)){
        std::istringstream linestream(line);
        linestream >> temp1 >> temp2;
        if (temp1 == "processes")
          return temp2;
      }
    }
   return 0;
    }

// TODO: Read and return the number of running processes -> DONE
int LinuxParser::RunningProcesses() {
    string line;
    std::ifstream stream(kProcDirectory + kStatFilename);
    string temp1; //key
    int temp2; //value
    if (stream.is_open()){
      while (std::getline(stream, line)){
        std::istringstream linestream(line);
        linestream >> temp1 >> temp2;
        if (temp1 == "procs_running")
          return temp2;
      }
    }
   return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }
