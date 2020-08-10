#ifndef STREAMFILER_TIMESTAMPUTILS_H
#define STREAMFILER_TIMESTAMPUTILS_H

#include <chrono>
#include <iomanip>
#include <string>

static std::string getTimestampedNameForFolder()
{
   using namespace std::chrono;

   // get current time
   auto now = system_clock::now();

   // get number of milliseconds for the current second
   // (remainder after division into seconds)
   auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

   // convert to std::time_t in order to convert to std::tm (broken time)
   auto timer = system_clock::to_time_t(now);

   // convert to broken time
   std::tm bt = *std::localtime(&timer);

   std::ostringstream oss;

   oss << std::put_time(&bt, "%Y%m%d_%H_%M_%S");
   oss << '.' << std::setfill('0') << std::setw(3) << ms.count();

   return oss.str();
}

#endif  // STREAMFILER_TIMESTAMPUTILS_H
