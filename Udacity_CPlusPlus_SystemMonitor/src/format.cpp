#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  int ss, mm, hh;
  std::string time;
  // Compute hours
  hh = seconds / 3600;
  // Compute minutes
  seconds = seconds % 3600;
  mm = seconds / 60;
  // Compute seconds
  ss = seconds % 60;
  // Format time string
  if (hh < 10) {
    time = "0";
  }
  time += std::to_string(hh) + ":";

  if (mm < 10) {
    time += "0";
  }
  time += std::to_string(mm) + ":";

  if (ss < 10) {
    time += "0";
  }
  time += std::to_string(ss);

  return time;
}