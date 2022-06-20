#include"locks.h"


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