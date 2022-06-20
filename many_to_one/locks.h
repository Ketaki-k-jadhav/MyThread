


typedef struct spinlock
{
    int flag; //Tells if the lock is locked=1 or unlocked=0
} spinlock;

void spinlock_init(spinlock *lock);
int spinlock_lock(spinlock *sl);
int  spinlock_unlock(spinlock *sl);