#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "format.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID -> DONE
int Process::Pid() { return pid_; }

void Process::Pid(int pid) { pid_ = pid; }

// TODO: Return this process's CPU utilization -> DONE
float Process::CpuUtilization() const { return cpu_; }

void Process::CpuUtilization(int pid) {
  cpu_ = LinuxParser::CpuUtilization(pid);
}

// TODO: Return the command that generated this process -> DONE
string Process::Command() { return LinuxParser::Command(Pid()); }

// TODO: Return this process's memory utilization -> DONE
string Process::Ram() { return LinuxParser::Ram(Pid()); }

// TODO: Return the user (name) that generated this process -> DONE
string Process::User() { return LinuxParser::User(Pid()); }

// TODO: Return the age of this process (in seconds) -> DONE
long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  // the other way around to sort in descending order later
  return (a.CpuUtilization() < this->CpuUtilization());
}