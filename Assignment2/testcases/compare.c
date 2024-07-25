#include "header.h"

int main(int argc, char *argv[]) {
  int start_index = 1, end_index = 30;
  int var_fixer = 5;
  for (int index = start_index; index < end_index; index++) {
    int loop_count = var_fixer;
    unsigned long long perf_fork = 0, perf_context = 0;
    // init_params(argc, argv);
    while (loop_count--) {
      perf_fork += run_fork(index);
      perf_context += run_context(index);
    }
    printf("Index = %d\n", index);
    printf("Baseline(fork) Throughput:%15lld\n", perf_fork / var_fixer);
    printf("Optimized(context) Throughput:%12lld\n", perf_context / var_fixer);
    printf("Speedup:%33.2f\n", (double)perf_context / perf_fork);
  }
  return 0;
}
