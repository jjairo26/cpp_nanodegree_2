#include "processor.h"

#include <vector>

#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  std::vector<std::string> CPU_vec_str = LinuxParser::CpuUtilization();
  CPU_util_aggr = std::stof(CPU_vec_str[0]);
  return CPU_util_aggr;
}