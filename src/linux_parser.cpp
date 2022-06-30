#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include <unistd.h>

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

// TODO: Read and return the system uptime -> DONE
long LinuxParser::UpTime() { 
  string line;
  long uptime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()){
      std::getline(stream, line); // Only 1 line
      std::istringstream linestream(line);
      linestream >> uptime;
      return uptime; //value in seconds
  }
  return 0;
 }

// TODO: Read and return the number of jiffies for the system -> DONE
long LinuxParser::Jiffies() { 
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  long sum {0};
  if (stream.is_open()){
      std::getline(stream, line); //first line
      string delimiter = "  ";
      line = line.substr(line.find(delimiter) + delimiter.length());
      std::istringstream linestream(line);
      long temp {0};
      while(linestream >> temp)
        sum += temp;
  }
  return sum;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
   string line, temp;
   string pid_str = std::to_string(pid);
   vector<string> line_elements;
   std::ifstream stream(kProcDirectory + pid_str + kStatFilename);

   if (stream.is_open()){
     std::getline(stream, line);
     std::istringstream linestream(line);
     while (linestream >> temp){ // Get all elements as strings and save in vector
       line_elements.emplace_back(temp);
     }
   }
   // utime + stime + cutime + cstime(child process time included)
   return std::stol(line_elements[13]) + std::stol(line_elements[14]) + std::stol(line_elements[15]) + std::stol(line_elements[16]);
}

// TODO: Read and return the number of active jiffies for the system -> DONE
long LinuxParser::ActiveJiffies() { // NonIdle = user (0) + nice (1) + system (2) + irq (5) + softirq (6) + steal (7)
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  vector<long> jiffie_vec;
  if (stream.is_open()){
      std::getline(stream, line); //first line
      string delimiter = "  ";
      line = line.substr(line.find(delimiter) + delimiter.length());
      std::istringstream linestream(line);
      long temp {0};
      while(linestream >> temp)
        jiffie_vec.emplace_back(temp);
         
      return jiffie_vec[0]+jiffie_vec[1]+jiffie_vec[2]+jiffie_vec[5]+jiffie_vec[6]+jiffie_vec[7];
   }
   return 0;
}

// TODO: Read and return the number of idle jiffies for the system -> DONE
long LinuxParser::IdleJiffies() { //Idle = idle (3) + iowait (4)
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  std::vector<long> jiffie_vec;
  if (stream.is_open()){
      std::getline(stream, line); //first line
      string delimiter = "  ";
      line = line.substr(line.find(delimiter) + delimiter.length());
      std::istringstream linestream(line);
      long temp {0};
      while(linestream >> temp)
        jiffie_vec.emplace_back(temp);

      return jiffie_vec[3]+jiffie_vec[4];
  }
  return 0;
}

// TODO: Read and return CPU utilization -> DONE
vector<string> LinuxParser::CpuUtilization() {
   vector<string> CPU_Util_vec;
   float active_jiffies = static_cast<float>(ActiveJiffies());
   float total_jiffies = static_cast<float>(Jiffies());
   CPU_Util_vec.emplace_back(std::to_string(active_jiffies/total_jiffies));
   return CPU_Util_vec;
}

float LinuxParser::CpuUtilization(int pid) {
    float HZ = sysconf(_SC_CLK_TCK);
    long sys_uptime = LinuxParser::UpTime();
    long starttime = LinuxParser::UpTime(pid);
    long totaltime = LinuxParser::ActiveJiffies(pid); 
    float seconds = static_cast<float>(sys_uptime) - (static_cast<float>(starttime)/HZ);
    return (static_cast<float>(totaltime)/HZ)/seconds; 
}


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

// TODO: Read and return the command associated with a process -> DONE
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line, command_str;
  string pid_str = std::to_string(pid);
  std::ifstream stream(kProcDirectory + pid_str + kCmdlineFilename);
  if (stream.is_open()){
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> command_str;
  }
   return command_str;
}

// TODO: Read and return the memory used by a process -> DONE
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line;
  string pid_str = std::to_string(pid);
  std::ifstream stream(kProcDirectory + pid_str + kStatusFilename);
  string temp1, temp2; //key & value
  string ram;
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::replace(line.begin(), line.end(), '\t', ' ');
      std::istringstream linestream(line);
      linestream >> temp1 >> temp2;
      if (temp1 == "VmSize:"){
        ram = std::to_string(std::stoi(temp2)/1024); //convert from KB to MB
        return ram;
      }
    }
  }

  return ram;
   
}

// TODO: Read and return the user ID associated with a process -> DONE
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
    //Read User ID
    string line;
    string pid_str = std::to_string(pid);
    string temp1, temp2; //key & value
    string Uid;
    std::ifstream stream(kProcDirectory + pid_str + kStatusFilename);
    if (stream.is_open()){
       while(std::getline(stream, line)){
          std::replace(line.begin(), line.end(), '\t', ' ');
          std::istringstream linestream(line);
          linestream >> temp1 >> temp2;
          if (temp1 == "Uid:"){
            Uid = temp2;
            break;
          }
       }
    }
    return Uid;
 }

// TODO: Read and return the user associated with a process -> DONE
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {

    //Extract the name which corresponds to the User ID
    string temp1, temp2, temp3, line, name;
    std::ifstream stream(kPasswordPath);
    if (stream.is_open()){
      while(std::getline(stream, line)){
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream linestream(line);
        linestream >> temp1 >> temp2 >> temp3;
        if (temp3 == LinuxParser::Uid(pid)){
          name = temp1;
          break;
        }
      }
    }

    return name;  
}

// TODO: Read and return the uptime of a process -> DONE
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string pid_str = std::to_string(pid);
  string temp, line;
  vector<string> line_elements;
  std::ifstream stream(kProcDirectory + pid_str + kStatFilename);

  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream >> temp) // Get all elements as strings and save in vector
      line_elements.emplace_back(temp);
      
    float time = std::stof(line_elements[21])/sysconf(_SC_CLK_TCK); //22nd value (ticks) divided by HZ to obtain seconds
    return static_cast<long>(time); //value in seconds
  }
  return 0; //process has no data (dead process?)
}
