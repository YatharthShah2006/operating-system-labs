// Mutual exclusion lock.
struct spinlock {
  uint locked;       // Is the lock held?

  // For debugging:
  char *name;        // Name of lock.
  struct cpu *cpu;   // The cpu holding the lock.
#ifdef LAB_LOCK
  int nts;
  int n;
#endif
};

#ifdef LAB_LOCK
// Reader-writer lock.
struct rwspinlock {
  // Replace this with your implementation.
  struct spinlock guard;
  volatile uint writers_waiting;
  volatile uint writer_holding;
  volatile uint readers_using;
};
#endif
