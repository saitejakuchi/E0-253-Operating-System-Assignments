#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/resource.h>
#include<sched.h>

void func(){
        long long ans = 0;
        for(int i=0; i<1000; i++){
                for(int j=0; j<1000; j++){
                int res = i * j ;
                ans += res;
                }
        }
}


int main()
{
        int a;
        struct sched_param sp;
        cpu_set_t set;
	CPU_ZERO(&set);
        CPU_SET(1,&set);
        sched_setaffinity(0, sizeof(set), &set);
        setpriority(PRIO_PROCESS, 0, -20);
        sched_setscheduler(0,7,&sp);
        printf("Parent Pid: {%d}\n", getpid());
       	
       	func();
        //sleep(5);
        //printf("Sleep Ended\n");
        printf("Hello World\n");
        sched_yield();
        printf("Yield calle and completed\n");
        func();
        return 0;
}


