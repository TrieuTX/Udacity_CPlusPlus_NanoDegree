#include "processor.h"
#include "linux_parser.h"
#include "unistd.h"
#include <iostream>
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    long pre_Total = LinuxParser::Jiffies();
    long pre_NonIdle = LinuxParser::ActiveJiffies();

    usleep(100000);

    long Total = LinuxParser::Jiffies();
    long NonIdle = LinuxParser::ActiveJiffies();

    if((Total - pre_Total)*(NonIdle - pre_NonIdle) > 0){
        return float(NonIdle - pre_NonIdle)/float(Total - pre_Total);
    }
  return 0;
 }