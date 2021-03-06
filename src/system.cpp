#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric
criteria "The student will be able to extract and display basic data about the
system."

You need to properly format the uptime. Refer to the comments mentioned in
format. cpp for formatting the uptime.*/

// TODO: Return the system's CPU -> No action needed here
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  processes_.clear();  // clear process vector so that it does not keep growing
                       // endlessly
  for (int Pid : LinuxParser::Pids()) {
    Process newProcess;
    newProcess.Pid(Pid);
    newProcess.CpuUtilization(Pid);
    processes_.emplace_back(newProcess);
  }
  std::sort(processes_.begin(), processes_.end());

  return processes_;
}

// TODO: Return the system's kernel identifier (string) -> DONE
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization -> DONE
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name -> DONE
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system -> DONE
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system -> DONE
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running -> DONE
long int System::UpTime() { return LinuxParser::UpTime(); }
