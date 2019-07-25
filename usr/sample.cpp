#include "sys/sys.h"

void* user_task3(void* arg)
{
    (void) arg;
    write_sys_call("user task3 start \n");
    while(1) {
        write_sys_call("user task3-1 \n");
        write_sys_call("user task3-2 \n");
        write_sys_call("user task3-3 \n");
    }
    return 0;
}

void* user_task2(void* args)
{
    write_sys_call("user task2 start \n");
    fork_sys_call(user_task3, 0);
    (void) args;
    while(1) {
        write_sys_call("user task2-1 \n");
        write_sys_call("user task2-2 \n");
    }
    return 0;
}

void* user_task1(void* args)
{
    fork_sys_call(user_task2, 0);
    write_sys_call("user task1 start \n");
    (void) args;
    while(1) {
        write_sys_call("user task1-1 \n");
        write_sys_call("user task1-2 \n");
        write_sys_call("user task1-3 \n");
        write_sys_call("user task1-4 \n");
    }
    return 0;
}
