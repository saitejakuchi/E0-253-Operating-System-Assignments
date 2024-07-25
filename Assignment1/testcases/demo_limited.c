#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sched.h>

void func(){
        long long ans = 0;
        for(int i=0; i<1000; i++){
                for(int j=0; j<1000; j++){
                        int res = i * j;
                        ans += res;
                }
        }
}

int main(){
        int a;
        struct sched_param sp;
        cpu_set_t set;
        sp.sched_priority = 0;
        CPU_ZERO( &set);
	CPU_SET(1, &set);
        sched_setaffinity(0, sizeof(set), &set);
        setpriority(PRIO_PROCESS, 0, -20);
        sched_setscheduler(0, 7, &sp);
        printf("Parent Pid: {%d}\n", getpid());
        for(int i=0; i<10; i++){
                int r = fork();
                if(r != 0){
                        continue;
                }
                if(sched_setaffinity(0, sizeof(set), &set) == -1){
                        printf("failed to set affinity of task\n");
                        return 0;
                }
                if(setpriority(PRIO_PROCESS, 0, -10) == -1){
                        printf("failed to set priority of process\n");
                        return 0;
                }
                printf("Priority of the process is {%d}, pid: {%d}\n", getpriority(PRIO_PROCESS, 0), getpid());
                a = sched_setscheduler(0, 7, &sp);
                int policy = sched_getscheduler(0);
                printf("Policy of the process: {%d}\n", policy);
                if(a == -1){
                        printf("error setting the policy of the process\n");
                        printf("Setscheduler returned\n");
                        return 0;
                }
                printf("Child Pid : {%d}\n", getpid());
                //while(1);
                func();
                return 0;
        }

        return 0;
}

                

