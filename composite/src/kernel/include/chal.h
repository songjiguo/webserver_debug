/**
 * Redistribution of this file is permitted under the GNU General
 * Public License v2.
 *
 * Copyright The George Washington University, Gabriel Parmer,
 * gparmer@gwu.edu, 2012
 */

/* 
 * The Composite Hardware Abstraction Layer, or Hijack Abstraction
 * Layer (cHAL) is the layer that defines the platform-specific
 * functionality that requires specific implementations not only for
 * different architectures (e.g. x86-32 vs. -64), but also when
 * booting from the bare-metal versus using the Hijack techniques.
 * This file documents the functions that must be implemented within
 * the platform code, and how they interact with the _Composite_
 * kernel proper.
 */

#ifndef CHAL_H
#define CHAL_H

#include "shared/cos_types.h"

/* 
 * Namespacing in the cHAL: chal_<family>_<operation>(...).  <family>
 * is the family of operations such as pgtbl or addr operations, and
 * <operation> is the operation to perform on that family of
 * manipulations.
 */

/*************************************
 * Platform page-table manipulations *
 *************************************/

/* 
 * Switch to the specified page-tables.  This will not only switch the
 * loaded page tables on the current cpu, but also any backing
 * data-structures that are tracked in the platform code.
 * 
 * This function must be specified in the chal_plat.h file.
 */
static inline void chal_pgtbl_switch(paddr_t pt);
/* 
 * Switch any backing data-structures for the "current" page-table,
 * but _not_ the actual loaded page-tables.
 * 
 * This function must be specified in the chal_plat.h file.
 */
static inline void __chal_pgtbl_switch(paddr_t pt);

/* Add a page to pgtbl at address. 0 on success */
int     chal_pgtbl_add(paddr_t pgtbl, vaddr_t vaddr, paddr_t paddr);
/* Translate a vaddr to an addressable address via pgtbl */
vaddr_t chal_pgtbl_vaddr2kaddr(paddr_t pgtbl, unsigned long addr);
/* Remove mapping for a vaddr from pgtbl. != 0 if mapping doesn't exist */
paddr_t chal_pgtbl_rem(paddr_t pgtbl, vaddr_t va);
int     chal_pgtbl_entry_absent(paddr_t pt, unsigned long addr);
void    chal_pgtbl_copy_range(paddr_t pt_to, paddr_t pt_from,
			      unsigned long lower_addr, unsigned long size);
void    chal_pgtbl_copy_range_nocheck(paddr_t pt_to, paddr_t pt_from,
				      unsigned long lower_addr, unsigned long size);
void    chal_pgtbl_zero_range(paddr_t pt, unsigned long lower_addr, unsigned long size);
/* can we switch the current page tables right now? */
int     chal_pgtbl_can_switch(void);

/* operations on the page directory (as opposed to on page-table entries) */
int chal_pgtbl_add_middledir(paddr_t pt, unsigned long vaddr);
int chal_pgtbl_rem_middledir(paddr_t pt, unsigned long vaddr);
int chal_pgtbl_rem_middledir_range(paddr_t pt, unsigned long vaddr, long size);
int chal_pgtbl_add_middledir_range(paddr_t pt, unsigned long vaddr, long size);

/*********************************
 * Address translation functions *
 *********************************/

void *chal_va2pa(void *va);
void *chal_pa2va(void *pa);

/************************************
 * Page allocation and deallocation *
 ************************************/

void *chal_alloc_page(void);
void chal_free_page(void *page);

/*******************
 * Other functions *
 *******************/

int chal_attempt_brand(struct thread *brand);
/* static const struct cos_trans_fns *trans_fns = NULL; */
void chal_idle(void);

/* int cos_syscall_idle(void); */
/* int cos_syscall_switch_thread(void); */
/* void cos_syscall_brand_wait(int spd_id, unsigned short int bid, int *preempt); */
/* void cos_syscall_brand_upcall(int spd_id, int thread_id_flags); */
/* int cos_syscall_buff_mgmt(void); */
/* void cos_syscall_upcall(void); */

#include "../../platform/include/chal_plat.h"

#endif	/* CHAL_H */
