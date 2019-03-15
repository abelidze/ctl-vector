#define BENCHPRESS_FILE_OUTPUT

#include <chrono>
#include <thread>
#include <vector>

#include "benchpress_edited.hpp"
// #include "allocator.hpp"
#include "vector.hpp"

#ifndef BENCHPRESS_CONFIG_MAIN
benchpress::registration* benchpress::registration::d_this;
#endif


using std_v_std_a = std::vector<int, std::allocator<int>>;
using std_v_ctl_a = std::vector<int, ctl::Allocator<int>>;
using ctl_v_std_a = ctl::Vector<int, std::allocator<int>>;
using ctl_v_ctl_a = ctl::Vector<int, ctl::Allocator<int>>;


BENCHMARK("push_back -> std::vector && std::allocator", [](benchpress::context* ctx) {
  for (auto k = 1; k < ctx->num_iterations(); ++k) {
    std_v_std_a v;
    for (int i = 0; i < k && i < FIXED_SIZE; ++i) {
      v.push_back(i);
    }
  }
});

BENCHMARK("push_back -> std::vector && ctl::Allocator", [](benchpress::context* ctx) {
  for (auto k = 1; k < ctx->num_iterations(); ++k) {
    std_v_ctl_a v;
    for (auto i = 0; i < k && i < FIXED_SIZE; ++i) {
      v.push_back(i);
    }
  }
});

BENCHMARK("push_back -> ctl::Vector && std::allocator", [](benchpress::context* ctx) {
  for (auto k = 1; k < ctx->num_iterations(); ++k) {
    ctl_v_std_a v;
    for (auto i = 0; i < k && i < FIXED_SIZE; ++i) {
      v.push_back(i);
    }
  }
});

BENCHMARK("push_back -> ctl::Vector && ctl::Allocator", [](benchpress::context* ctx) {
  for (auto k = 1; k < ctx->num_iterations(); ++k) {
    ctl_v_ctl_a v;
    for (auto i = 0; i < k && i < FIXED_SIZE; ++i) {
      v.push_back(i);
    }
  }
});

BENCHMARK("complex -> std::vector && std::allocator", [](benchpress::context* ctx) {
  for (auto k = 1; k < ctx->num_iterations(); ++k) {
    std_v_std_a v;
    for (auto i = 0; i < 250 * k && i < FIXED_SIZE; ++i) {
      v.push_back(i);
    }
    for (auto i = 0; i < 150 * k && i < FIXED_SIZE; ++i) {
      v.pop_back();
    }
    v.shrink_to_fit();
    v.clear();
    v.insert(v.begin(), k, 345);
    v.erase(v.begin() + v.size() / 2, v.end());
  }
});

BENCHMARK("complex -> std::vector && ctl::Allocator", [](benchpress::context* ctx) {
  for (auto k = 1; k < ctx->num_iterations(); ++k) {
    std_v_ctl_a v;
    for (auto i = 0; i < 250 * k && i < FIXED_SIZE; ++i) {
      v.push_back(i);
    }
    for (auto i = 0; i < 150 * k && i < FIXED_SIZE; ++i) {
      v.pop_back();
    }
    v.shrink_to_fit();
    v.clear();
    v.insert(v.begin(), k, 345);
    v.erase(v.begin() + v.size() / 2, v.end());
  }
});

BENCHMARK("complex -> ctl::Vector && std::allocator", [](benchpress::context* ctx) {
  for (auto k = 1; k < ctx->num_iterations(); ++k) {
    ctl_v_std_a v;
    for (auto i = 0; i < 250 * k && i < FIXED_SIZE; ++i) {
      v.push_back(i);
    }
    for (auto i = 0; i < 150 * k && i < FIXED_SIZE; ++i) {
      v.pop_back();
    }
    v.shrink_to_fit();
    v.clear();
    v.insert(v.begin(), k, 345);
    v.erase(v.begin() + v.size() / 2, v.end());
  }
});

BENCHMARK("complex -> ctl::Vector && ctl::Allocator", [](benchpress::context* ctx) {
  for (auto k = 1; k < ctx->num_iterations(); ++k) {
    ctl_v_ctl_a v;
    for (auto i = 0; i < 250 * k && i < FIXED_SIZE; ++i) {
      v.push_back(i);
    }
    for (auto i = 0; i < 150 * k && i < FIXED_SIZE; ++i) {
      v.pop_back();
    }
    v.shrink_to_fit();
    v.clear();
    v.insert(v.begin(), k, 345);
    v.erase(v.begin() + v.size() / 2, v.end());
  }
});


int main(int argc, char** argv)
{
  std::cout << "Benchmark started..." << std::endl;

  benchpress::options bench_opts;
  bench_opts.cpu(1);

  float timeTaken = 0.f;
  int seconds = 4;

  for (auto i = 1; i <= seconds; ++i) {
    std::chrono::high_resolution_clock::time_point bp_start = std::chrono::high_resolution_clock::now();
    
    bench_opts.benchtime(i);

    benchpress::out_stream << "SECTION for benchtime(" << i << ")" << std::endl;

    benchpress::run_benchmarks(bench_opts);

    float duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - bp_start
    ).count() / 1000.f;

    std::cout << "Processed " << i << " of " << seconds << std::endl;

    benchpress::out_stream << std::endl;
    benchpress::out_stream << "Time taken: " << duration << "s" << std::endl;
    benchpress::out_stream << std::endl << std::endl;

    timeTaken += duration;
  }

  std::cout << "Benchmark finished in " << timeTaken << "s" << std::endl;

  return 0;
}