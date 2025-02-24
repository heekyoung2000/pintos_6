// #ifndef THREADS_THREAD_H
// #define THREADS_THREAD_H

// #include <debug.h>
// #include <list.h>
// #include <stdint.h>
// #include "threads/interrupt.h"
// #include "threads/synch.h"
// // #define USERPROG
// #ifdef VM
// #include "vm/vm.h"
// #endif


// /* States in a thread's life cycle. */
// enum thread_status {
// 	THREAD_RUNNING,     /* Running thread. */
// 	THREAD_READY,       /* Not running but ready to run. */
// 	THREAD_BLOCKED,     /* Waiting for an event to trigger. */
// 	THREAD_DYING        /* About to be destroyed. */
// };

// /* Thread identifier type.
//    You can redefine this to whatever type you like. */
// typedef int tid_t;
// #define TID_ERROR ((tid_t) -1)          /* Error value for tid_t. */

// /* Thread priorities. */
// #define PRI_MIN 0                       /* Lowest priority. */
// #define PRI_DEFAULT 31                  /* Default priority. */
// #define PRI_MAX 63                      /* Highest priority. */

// /*project2 - system_call*/
// #define FDT_PAGES 2
// #define FDT_COUNT_LIMIT 128

// /* A kernel thread or user process.
//  *
//  * Each thread structure is stored in its own 4 kB page.  The
//  * thread structure itself sits at the very bottom of the page
//  * (at offset 0).  The rest of the page is reserved for the
//  * thread's kernel stack, which grows downward from the top of
//  * the page (at offset 4 kB).  Here's an illustration:
//  *
//  *      4 kB +---------------------------------+
//  *           |          kernel stack           |
//  *           |                |                |
//  *           |                |                |
//  *           |                V                |
//  *           |         grows downward          |
//  *           |                                 |
//  *           |                                 |
//  *           |                                 |
//  *           |                                 |
//  *           |                                 |
//  *           |                                 |
//  *           |                                 |
//  *           |                                 |
//  *           +---------------------------------+
//  *           |              magic              |
//  *           |            intr_frame           |
//  *           |                :                |
//  *           |                :                |
//  *           |               name              |
//  *           |              status             |
//  *      0 kB +---------------------------------+
//  *
//  * The upshot of this is twofold:
//  *
//  *    1. First, `struct thread' must not be allowed to grow too
//  *       big.  If it does, then there will not be enough room for
//  *       the kernel stack.  Our base `struct thread' is only a
//  *       few bytes in size.  It probably should stay well under 1
//  *       kB.
//  *
//  *    2. Second, kernel stacks must not be allowed to grow too
//  *       large.  If a stack overflows, it will corrupt the thread
//  *       state.  Thus, kernel functions should not allocate large
//  *       structures or arrays as non-static local variables.  Use
//  *       dynamic allocation with malloc() or palloc_get_page()
//  *       instead.
//  *
//  * The first symptom of either of these problems will probably be
//  * an assertion failure in thread_current(), which checks that
//  * the `magic' member of the running thread's `struct thread' is
//  * set to THREAD_MAGIC.  Stack overflow will normally change this
//  * value, triggering the assertion. */
// /* The `elem' member has a dual purpose.  It can be an element in
//  * the run queue (thread.c), or it can be an element in a
//  * semaphore wait list (synch.c).  It can be used these two ways
//  * only because they are mutually exclusive: only a thread in the
//  * ready state is on the run queue, whereas only a thread in the
//  * blocked state is on a semaphore wait list. */
// struct thread {
// 	/* Owned by thread.c. */
// 	tid_t tid;                          /* Thread identifier. */
// 	enum thread_status status;          /* Thread state. */
// 	char name[16];                      /* Name (for debugging purposes). */
// 	int priority;                       /* Priority. */

// 	/* Shared between thread.c and synch.c. */
// 	struct list_elem elem;              /* List element. */
	
// 	/*새로 생성한 변수*/
// 	int64_t wakeup_tick;

// 	/*새로 생성한 변수-priority donation*/
// 	int init_priority; // thread의 priority는 donation에 의해 매번 바뀔 수 있음. 따라서 맨 처음에 할당받은 priority를 기억해둔다.
// 	struct lock *wait_on_lock; // 해당 스레드가 대기하고 있는 lock 자료구조 주소 저장
// 	struct list donations; // 만약 우선순위가 높은 thread의 우선순위를 기부했다면 우선순위가 낮은 thread에 높은 thread를 기억해둔다.
// 	struct list_elem donation_elem; // 우선순위가 낮은 thread에 우선순위가 높은 thread의 이름을 저장(기부자 목록같은 느낌)
	
// 	/*project2*/
// 	int exit_status;
// 	struct file **fdt;
// 	int next_fd;

// 	struct intr_frame parent_if;
// 	struct list child_list;
// 	struct list_elem child_elem;

// 	struct semaphore load_sema;
// 	struct semaphore exit_sema;
// 	struct semaphore wait_sema;

// 	struct file *running;
	
// #ifdef USERPROG
// 	/* Owned by userprog/process.c. */
// 	uint64_t *pml4;                     /* Page map level 4 */
// #endif
// #ifdef VM
// 	/* Table for whole virtual memory owned by thread. */
// 	struct supplemental_page_table spt;
// #endif

// 	/* Owned by thread.c. */
// 	struct intr_frame tf;               /* Information for switching */
// 	unsigned magic;                     /* Detects stack overflow. */
	
	

// };

// /* If false (default), use round-robin scheduler.
//    If true, use multi-level feedback queue scheduler.
//    Controlled by kernel command-line option "-o mlfqs". */
// extern bool thread_mlfqs;

// void thread_init (void);
// void thread_start (void);

// void thread_tick (void);
// void thread_print_stats (void);

// typedef void thread_func (void *aux);
// tid_t thread_create (const char *name, int priority, thread_func *, void *);

// void thread_block (void);
// void thread_unblock (struct thread *);

// struct thread *thread_current (void);
// tid_t thread_tid (void);
// const char *thread_name (void);

// void thread_exit (void) NO_RETURN;
// void thread_yield (void);

// int thread_get_priority (void);
// void thread_set_priority (int);

// int thread_get_nice (void);
// void thread_set_nice (int);
// int thread_get_recent_cpu (void);
// int thread_get_load_avg (void);

// void do_iret (struct intr_frame *tf);

// /*새로 생성한 함수-alarm*/
// void thread_sleep(int64_t);
// void thread_wakeup(int64_t);
// void update_next_tick_to_awake(int64_t);
// int64_t get_next_tick_to_awake(void);

// /*새로 생성한 함수- priority scheduling*/
// void test_max_priority(void);
// bool cmp_priority(const struct list_elem *a,
// 					const struct list_elem *b,
// 					void *aux UNUSED);

// /*새로생성한 함수 - donation*/
// bool
// thread_compare_donate_priority(const struct list_elem *l,const struct list_elem *s,void *aux UNUSED);
// bool cmp_sem_priority(const struct list_elem *a,
// 					const struct list_elem *b,
// 					void *aux UNUSED);
// void donate_priority(void);
// void remove_with_lock(struct lock *lock);
// void refresh_priority(void);
// void preempt_priority(void);
// #endif /* threads/thread.h */

#ifndef THREADS_THREAD_H
#define THREADS_THREAD_H

#include <debug.h>
#include <list.h>
#include <stdint.h>
#include "threads/interrupt.h"
#include "threads/synch.h"

#ifdef VM
#include "vm/vm.h"
#endif


/* States in a thread's life cycle. */
enum thread_status {
	THREAD_RUNNING,     /* Running thread. */
	THREAD_READY,       /* Not running but ready to run. */
	THREAD_BLOCKED,     /* Waiting for an event to trigger. */
	THREAD_DYING        /* About to be destroyed. */
};

/* Thread identifier type.
   You can redefine this to whatever type you like. */
typedef int tid_t;
#define TID_ERROR ((tid_t) -1)          /* Error value for tid_t. */

/* Thread priorities. */
#define PRI_MIN 0                       /* Lowest priority. */
#define PRI_DEFAULT 31                  /* Default priority. */
#define PRI_MAX 63                      /* Highest priority. */

#define FDT_PAGES 2
#define FDT_COUNT_LIMIT 128

/* A kernel thread or user process.
 *
 * Each thread structure is stored in its own 4 kB page.  The
 * thread structure itself sits at the very bottom of the page
 * (at offset 0).  The rest of the page is reserved for the
 * thread's kernel stack, which grows downward from the top of
 * the page (at offset 4 kB).  Here's an illustration:
 *
 *      4 kB +---------------------------------+
 *           |          kernel stack           |
 *           |                |                |
 *           |                |                |
 *           |                V                |
 *           |         grows downward          |
 *           |                                 |
 *           |                                 |
 *           |                                 |
 *           |                                 |
 *           |                                 |
 *           |                                 |
 *           |                                 |
 *           |                                 |
 *           +---------------------------------+
 *           |              magic              |
 *           |            intr_frame           |
 *           |                :                |
 *           |                :                |
 *           |               name              |
 *           |              status             |
 *      0 kB +---------------------------------+
 *
 * The upshot of this is twofold:
 *
 *    1. First, `struct thread' must not be allowed to grow too
 *       big.  If it does, then there will not be enough room for
 *       the kernel stack.  Our base `struct thread' is only a
 *       few bytes in size.  It probably should stay well under 1
 *       kB.
 *
 *    2. Second, kernel stacks must not be allowed to grow too
 *       large.  If a stack overflows, it will corrupt the thread
 *       state.  Thus, kernel functions should not allocate large
 *       structures or arrays as non-static local variables.  Use
 *       dynamic allocation with malloc() or palloc_get_page()
 *       instead.
 *
 * The first symptom of either of these problems will probably be
 * an assertion failure in thread_current(), which checks that
 * the `magic' member of the running thread's `struct thread' is
 * set to THREAD_MAGIC.  Stack overflow will normally change this
 * value, triggering the assertion. */
/* The `elem' member has a dual purpose.  It can be an element in
 * the run queue (thread.c), or it can be an element in a
 * semaphore wait list (synch.c).  It can be used these two ways
 * only because they are mutually exclusive: only a thread in the
 * ready state is on the run queue, whereas only a thread in the
 * blocked state is on a semaphore wait list. */
struct thread {
	/* Owned by thread.c. */
	tid_t tid;                          /* Thread identifier. */
	enum thread_status status;          /* Thread state. */
	char name[16];                      /* Name (for debugging purposes). */
	int priority;                       /* Priority. */

	/* Shared between thread.c and synch.c. */
	struct list_elem elem;              /* List element. */
	
	/*새로 생성한 변수*/
	int64_t wakeup_tick;

	/*새로 생성한 변수-priority donation*/
	int init_priority; // thread의 priority는 donation에 의해 매번 바뀔 수 있음. 따라서 맨 처음에 할당받은 priority를 기억해둔다.
	struct lock *wait_on_lock; // 해당 스레드가 대기하고 있는 lock 자료구조 주소 저장
	struct list donations; // 만약 우선순위가 높은 thread의 우선순위를 기부했다면 우선순위가 낮은 thread에 높은 thread를 기억해둔다.
	struct list_elem donation_elem; // 우선순위가 낮은 thread에 우선순위가 높은 thread의 이름을 저장(기부자 목록같은 느낌)

	int exit_status;
	struct file **fdt;
	int next_fd;

	struct intr_frame parent_if;
	struct list child_list;
	struct list_elem child_elem;

	struct semaphore load_sema; // 현재 스레드가 load되는 동안 부모가 기다리게 하기 위한 semaphore
	struct semaphore exit_sema; // 종료 세마
	struct semaphore wait_sema; // 대기 세마

	struct file *running; // 현재 실행중인 파일
	
	
	
#ifdef USERPROG
	/* Owned by userprog/process.c. */
	uint64_t *pml4;                     /* Page map level 4 */
#endif
#ifdef VM
	/* Table for whole virtual memory owned by thread. */
	struct supplemental_page_table spt;
#endif

	/* Owned by thread.c. */
	struct intr_frame tf;               /* Information for switching */
	unsigned magic;                     /* Detects stack overflow. */
};

/* If false (default), use round-robin scheduler.
   If true, use multi-level feedback queue scheduler.
   Controlled by kernel command-line option "-o mlfqs". */
extern bool thread_mlfqs;

void thread_init (void);
void thread_start (void);

void thread_tick (void);
void thread_print_stats (void);

typedef void thread_func (void *aux);
tid_t thread_create (const char *name, int priority, thread_func *, void *);

void thread_block (void);
void thread_unblock (struct thread *);

struct thread *thread_current (void);
tid_t thread_tid (void);
const char *thread_name (void);

void thread_exit (void) NO_RETURN;
void thread_yield (void);

int thread_get_priority (void);
void thread_set_priority (int);

int thread_get_nice (void);
void thread_set_nice (int);
int thread_get_recent_cpu (void);
int thread_get_load_avg (void);

void do_iret (struct intr_frame *tf);

/*새로 생성한 함수-alarm*/
void thread_sleep(int64_t);
void thread_wakeup(int64_t);
void update_next_tick_to_awake(int64_t);
int64_t get_next_tick_to_awake(void);

/*새로 생성한 함수- priority scheduling*/
void test_max_priority(void);
bool cmp_priority(const struct list_elem *a,
					const struct list_elem *b,
					void *aux UNUSED);

/*새로생성한 함수 - donation*/
bool
thread_compare_donate_priority(const struct list_elem *l,const struct list_elem *s,void *aux UNUSED);
bool cmp_sem_priority(const struct list_elem *a,
					const struct list_elem *b,
					void *aux UNUSED);
void donate_priority(void);
void remove_with_lock(struct lock *lock);
void refresh_priority(void);
#endif /* threads/thread.h */
