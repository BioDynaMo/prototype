#ifndef TIMING_H_
#define TIMING_H_

#include <chrono>
#include <string>

namespace bdm {

class Timing {
 public:
  Timing(const std::string& description = "")
      : start_{timestamp()}, text_{description} {}

  ~Timing() {
    std::cout << text_ << " " << (timestamp() - start_) << " ms" << std::endl;
  }

 private:
  long start_;
  std::string text_;

  long timestamp() {
    namespace sc = std::chrono;
    auto time = sc::system_clock::now();
    auto since_epoch = time.time_since_epoch();
    auto millis = sc::duration_cast<sc::milliseconds>(since_epoch);
    return millis.count();
  }
};

}  // namespace bdm

#endif  // TIMING_H_