#ifndef EFFECTIVE_BROCCOLI_TIME
#define EFFECTIVE_BROCCOLI_TIME

#include <chrono>

using Duration  = std::chrono::nanoseconds;
using Clock     = std::chrono::steady_clock;
using TimeStamp = std::chrono::time_point<Clock>;

double DurationToDouble(const Duration& dur) {
  return dur.count() * 1.0 / Duration::period::num * Duration::period::den;
}

class StopWatch {

public:
  StopWatch() : start_time_(Now()) {
  }

  Duration Elapsed() const {
    return Now() - start_time_;
  }

  Duration Restart() {
    const auto elapsed = Elapsed();
    start_time_ = Now();
    return elapsed;
  }

private:
  static TimeStamp Now() {
    return Clock::now();
  }

private:
  TimeStamp start_time_;
};


#endif