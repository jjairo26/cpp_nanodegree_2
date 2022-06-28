#include <string>
#include <cmath>

#include "format.h"

using std::string;


string Format::InsertPrecedingZeroIfNecessary(int time){
    if (time < 10)
        return "0" + std::to_string(time);
    else
        return std::to_string(time);
}

// TODO: Complete this helper function -> DONE
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
     float sec_float = static_cast<float>(seconds);
     string hour_str;
     string min_str;
     string sec_str;
     int hours = static_cast<int>(std::floor(sec_float/3600));
     int minutes = (std::floor((int)(sec_float/60)%60));
     int sec = static_cast<int>(seconds%60);
     hour_str = InsertPrecedingZeroIfNecessary(hours);
     min_str = InsertPrecedingZeroIfNecessary(minutes);
     sec_str = InsertPrecedingZeroIfNecessary(sec);
    
     return hour_str + ":" + min_str + ":" + sec_str;
}