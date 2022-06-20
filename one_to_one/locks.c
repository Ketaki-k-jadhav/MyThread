#include <sys/syscall.h>
#include <linux/futex.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <asm/prctl.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <errno.h>
#include "locks.h"
#define gettid() syscall(SYS_gettid)

static int test_and_set(int *flag)
{
    int result, set = 1;
    asm("lock xchgl %0, %1"
        : "+m"(*flag), "=a"(result)
        : "1"(set)
        : "cc");
    return result;
}

void spinlock_init(spinlock *lock)
{
    lock->flag = 0;
}

int spinlock_lock(spinlock *sl)
{
    while (1)
    {
        int flag_status = test_and_set(&sl->flag);
        if (flag_status == 0)
        {
            break;
        }
    }
    return 1;
}

int spinlock_unlock(spinlock *sl)
{
    sl->flag = 0;
    return 1;
}