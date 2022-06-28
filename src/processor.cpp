#include "processor.h"
#include "linux_parser.h"
#include <vector>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    std::vector<std::string> CPU_vec_str = LinuxParser::CpuUtilization();
    return std::stof(CPU_vec_str[0]);
}