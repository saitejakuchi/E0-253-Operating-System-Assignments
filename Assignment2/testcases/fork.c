#include "header.h"

int main(int argc, char *argv[])
{
	int perf_fork;

	init_params(argc, argv);
	perf_fork = run_fork();
	printf("Fork per second:\t%d\n", perf_fork);
        return 0;
}
