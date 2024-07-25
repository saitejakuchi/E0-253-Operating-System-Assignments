#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>

#define ACTION_INIT	0
#define ACTION_VERIFY	1

unsigned long num_elements;
// int num_elements = (1UL << 10);
int timeout = 10;
int syscall_num = 548;

int *continue_work;

// void init_params(int argc, char *argv[])
// {
// 	int c;

// 	while ((c = getopt(argc, argv, "n:t:s:")) != -1) {
// 		switch(c) {
// 			case 'n':
// 				num_elements = (1UL << atoi(optarg));
// 				break;
// 			case 't':
// 				timeout = atoi(optarg);
// 				break;
// 			case 's':
// 				syscall_num=atoi(optarg);
// 				break;
// 			default:
// 				printf("Usage: %s [-n num_elements(power of 2)] [-t timeout] [-s syscall_num]\n", argv[0]);
// 				exit(0);
// 		}
// 	}
// }

void handle_timer(int signal)
{
	*continue_work = 0;
}

static void do_good(int *buff, int action)
{
        int i;

	if (action == ACTION_INIT)
		for (i = 0; i < num_elements; i++)
			buff[i] = i;

	if (action == ACTION_VERIFY)
		for (i = 0; i < num_elements; i++)
			assert(buff[i] == i);
}

static inline void do_evil(int *buff)
{
        int i, *ptr;

        for (i = 0; i < num_elements; i++) {
                buff[i] = 0 - buff[i];
		break;
	}
}

int run_fork(int index_value)
{

        int status, nr_calls = 0, run = 1, ret;
        int *buff;

	num_elements = (1UL << index_value);

        buff = malloc(sizeof(int) * num_elements);
        assert(buff != NULL);
        continue_work = &run;

        signal(SIGALRM, &handle_timer);
        alarm(timeout);

        do_good(buff, ACTION_INIT);

fork:
	ret = fork();
	assert(ret >= 0);

        if (ret == 0) {
                do_evil(buff);
                free(buff);
                exit(0);
        } else {
                nr_calls++;
                wait(&status);

                if (*continue_work)
                        goto fork;
        }
        do_good(buff, ACTION_VERIFY);
        free(buff);
        return nr_calls / timeout;
}

int run_context(int index_value)
{
        int nr_calls = 0, run = 1, ret;
        int *buff;

	num_elements = (1ULL << index_value);

        buff = malloc(sizeof(int) * num_elements);
        assert(buff != NULL);
        continue_work = &run;

        signal(SIGALRM, handle_timer);
        alarm(timeout);

        do_good(buff, ACTION_INIT);

        while (*continue_work) {
                nr_calls++;
                /***** save context *****/
                ret = syscall(syscall_num, 0);
//		printf("Ret Value is %d\n", ret);
		assert(ret == 0);
                /* suspicious code */
                do_evil(buff);
                /***** recover context *****/
                ret = syscall(syscall_num, 1);
		assert(ret == 0);
        }
        do_good(buff, ACTION_VERIFY);
        free(buff);
	return nr_calls / timeout;
}
