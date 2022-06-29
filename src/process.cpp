#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"
#include "format.h"
#include <unistd.h>

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

void Process::Pid(int pid) {
    pid_ = pid;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    float HZ = sysconf(_SC_CLK_TCK);
    long sys_uptime = LinuxParser::UpTime();
    starttime_ = LinuxParser::UpTime(Pid());
    totaltime_ = LinuxParser::ActiveJiffies(Pid()); 
    float seconds = static_cast<float>(sys_uptime) - (static_cast<float>(starttime_)/HZ);
    return 100*((static_cast<float>(totaltime_)/HZ)/seconds); 
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Pid()); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Pid()); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Pid()); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }