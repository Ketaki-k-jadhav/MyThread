#include <sys/syscall.h>
#include <unistd.h>
#include <linux/futex.h>

typedef struct spinlock
{
    int flag; 
} spinlock;

void spinlock_init(spinlock *lock);
int spinlock_lock(spinlock *sl);
int  spinlock_unlock(spinlock *sl);