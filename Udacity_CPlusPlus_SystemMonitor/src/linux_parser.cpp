#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

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
  return "";
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
    return kernel;
  }
  return "";
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line, line2;
  string key;
  string value;
  float memTotal;
  float memFree;
  int number_line = 2;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    for (int i = 0; i < number_line; i++) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal:") {
        memTotal = std::stof(value);
      };
      if (key == "MemFree:") {
        memFree = std::stof(value);
      };
    }
    return (memTotal - memFree) / memTotal;
  }
  return 0.0;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  string uptime;
  string idle;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idle;
    return std::stol(uptime);
  }
  return 0;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  //Total = user + nice + system + irq + softirq + steal + idle + iowait
  long Total = 0;
  std::vector<std::string> cpu_data = LinuxParser::CpuUtilization();
  for(int i = kUser_;i <= kSteal_;i++){
    Total += std::stol(cpu_data[i]);
  }
  return Total;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
//long LinuxParser::ActiveJiffies(int pid) { }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  //NonIdle = user + nice + system + irq + softirq + steal
  return LinuxParser::Jiffies() - LinuxParser::IdleJiffies(); 
  }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  //Idle = idle + iowait
  long Idle = 0;
  std::vector<std::string> cpu_data = LinuxParser::CpuUtilization();
  for(int i = kIdle_;i <= kIOwait_;i++){
    Idle += std::stol(cpu_data[i]);
  }
  return Idle; 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpu_data;
  string line;
  string value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    //std::cout << line << std::endl;
    while (linestream >> value) {
      if (value != "cpu") 
        cpu_data.emplace_back(value);
    }
    return cpu_data;
  };
  return {};
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return std::stol(value);
        }
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return std::stol(value);
        }
      }
    }
  }
  return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
   string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
	return line; 
  }
  return ""; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  string ram;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> ram) {
        if (key == "VmSize:") {
          return std::to_string(std::stol(ram)/1024);
        }
      }
    }
  }
  return "0";
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
string line;
  string key;
  string uid;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> uid) {
        if (key == "Uid:") {
          return uid;
        }
      }
    }
  }
  return "";
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line;
  string key;
  string uid;
  string user;
  string x;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> uid) {
        if (uid == LinuxParser::Uid(pid)) {
          return user;
        }
      }
    }
  }
  return "";
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line;
  long starttime;
  long Hz = sysconf(_SC_CLK_TCK);
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename); 
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0; i<22; i++)
    {
      string val;
      linestream >> val;
      if(i == 21) {
        starttime = std::stol(val);
        return (starttime / Hz);
      }
    }
  }
  return 0; 
}

float LinuxParser::CpuUtilization(int pid) { 
  float sysUptime = 0; 
  float utime = 0;//#14- CPU time spent in user code in clock ticks
  float stime = 0; //#15- CPU time spent in kernel code in clock ticks
  float cutime = 0; //#16- Waited-for children's CPU time spent in user code in clock ticks
  float cstime = 0; //#17- Waited-for children's CPU time spent in kernel code in clock ticks
  float starttime = 0; //#22- Time when the process started in clock ticks
  string val;
  float total_time = 0.0;
  float seconds = 0.0;
  float cpuUtilization = 0.0;
  string line = "";
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename); //"/proc/"+"pid"+"/stat"
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int n = 1; n < 23; n++)
    {
      linestream >> val;
      if(n==14) utime = stof(val);
      if(n==15) stime = stof(val);
      if(n==16) cutime = stof(val);
      if(n==17) cstime = stof(val);
      if(n==22) starttime = stof(val);
    }
  }
  total_time = utime + stime + cutime + cstime;
  sysUptime = LinuxParser::UpTime();
  seconds =  sysUptime - (starttime / sysconf(_SC_CLK_TCK));
  if(seconds){
  cpuUtilization = (total_time / sysconf(_SC_CLK_TCK)) / seconds;
    return cpuUtilization;
  }
  return 0.0;
}