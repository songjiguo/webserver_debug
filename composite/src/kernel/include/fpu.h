#ifndef FPU_H
#define FPU_H

#include "thread.h"

#define ENABLE  1
#define DISABLE 0
#define FPU_DISABLED_MASK 0x8

extern int fpu_disabled;
extern struct thread *fpu_last_used;

/* fucntions called outside */
static inline int fpu_init(void);
static inline int fpu_disabled_exception_handler(void);
static inline void fpu_thread_init(struct thread *thd);
static inline int fpu_save(struct thread *next);

/* packed functions for FPU operation */
static inline void fpu_enable(void);
static inline void fpu_disable(void);
static inline int fpu_is_disabled(void);
static inline int fpu_thread_uses_fp(struct thread *thd);

/* packed low level (assemmbly) functions */
static inline void fxsave(struct thread*);
static inline void fxrstor(struct thread*);
static inline unsigned long fpu_read_cr0(void);
static inline void fpu_set(int);

#ifdef FPU_ENABLED
static inline int
fpu_init(void)
{
        fpu_set(DISABLE);
        fpu_disabled = 1;
        fpu_last_used = NULL;

        printk("fpu_init on core %d\n", get_cpuid());

        return 0;
}

static inline int 
fpu_disabled_exception_handler(void)
{
        struct thread *curr_thd;

        if ((curr_thd = core_get_curr_thd()) == NULL) return 1;

        assert(fpu_is_disabled());

        curr_thd->fpu.status = 1;
        fpu_save(curr_thd);

        return 1;
}

static inline void
fpu_thread_init(struct thread *thd)
{
        thd->fpu.status = 0;
        thd->fpu.saved_fpu = 0;

        return;
}

static inline int
fpu_save(struct thread *next)
{
        /* if next thread doesn't use fpu, then we just disable the fpu */
        if (!fpu_thread_uses_fp(next)) {
                fpu_disable();
                return 0;
        }

        /*
         * next thread uses fpu
         * if no thread used fpu before, then we set next thread as the fpu_last_used
         */
        if (unlikely(fpu_last_used == NULL)) {
                fpu_enable();
                fpu_last_used = next;
                return 0;
        }

        /*
         * next thread uses fpu
         * fpu_last_used exists
         * if fpu_last_used == next, then we simply re-enable the fpu for the thread
         */
        if (fpu_last_used == next) {
                fpu_enable();
                return 0;
        }

        /*
         * next thread uses fpu
         * fpu_last_used exists
         * if fpu_last_used != next, then we save current fpu states to fpu_last_used, restore next thread's fpu state
         */
        fpu_enable();
        fxsave(fpu_last_used);
        if (next->fpu.saved_fpu) fxrstor(next);
        fpu_last_used = next;

        return 0;
}

static inline void
fpu_enable(void)
{
        if (!fpu_is_disabled()) return;

        fpu_set(ENABLE);
        fpu_disabled = 0;

        return;
}

static inline void
fpu_disable(void)
{
        if (fpu_is_disabled()) return;

        fpu_set(DISABLE);
        fpu_disabled = 1;

        return;
}

static inline int
fpu_is_disabled(void)
{
        assert(fpu_read_cr0() & FPU_DISABLED_MASK ? fpu_disabled : !fpu_disabled);

        return fpu_disabled;
}

static inline int
fpu_thread_uses_fp(struct thread *thd)
{
        return thd->fpu.status;
}

static inline unsigned long
fpu_read_cr0(void)
{
        unsigned long val;
        asm volatile("mov %%cr0,%0" : "=r" (val));

        return val;
}

static inline void
fpu_set(int status)
{
        unsigned long val, cr0;

        cr0 = fpu_read_cr0();
        val = status ?  (cr0 & ~FPU_DISABLED_MASK) : (cr0 | FPU_DISABLED_MASK); // ENABLE(status == 1) : DISABLE(status == 0)
        asm volatile("mov %0,%%cr0" : : "r" (val));

        return;
}

static inline void
fxsave(struct thread *thd)
{
        asm volatile("fxsave %0" : "=m" (thd->fpu));
        thd->fpu.saved_fpu = 1;

        return;
}

static inline void
fxrstor(struct thread *thd)
{
        asm volatile("fxrstor %0" : : "m" (thd->fpu));

        return;
}
#else
/* if FPU_DISABLED is not defined, then we use these dummy functions */
static inline int fpu_init(void) { return 0;}
static inline int fpu_disabled_exception_handler(void) { return 1; }
static inline void fpu_thread_init(struct thread *thd) { return; }
static inline int fpu_save(struct thread *next) { return 0; }
static inline void fpu_enable(void) { return; }
static inline void fpu_disable(void) { return; }
static inline int fpu_is_disabled(void){ return 1; }
static inline int fpu_thread_uses_fp(struct thread *thd) { return 0; }
static inline void fxsave(struct thread *thd) { return; }
static inline void fxrstor(struct thread *thd) { return; }
static inline unsigned long fpu_read_cr0(void) { return 0x0; }
static inline void fpu_set(int status) { return; }
#endif

#endif
