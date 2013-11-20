/**
 * Copyright 2010 by The George Washington University.  All rights reserved.
 *
 * Redistribution of this file is permitted under the GNU General
 * Public License v2.
 *
 * Author: Gabriel Parmer, gparmer@gwu.edu, 2010
 */

#ifndef   	MEM_MGR_H
#define   	MEM_MGR_H

//#define MEA_GET
//#define MEA_ALIAS
//#define MEA_REVOKE
//#define MEA_ADD_ROOT

#define TEN2TEN           	/* 1 to 2 */
//#define ONE2TEN

#define ONE2FIVE  		/* 2 to 3/4 */

/* Map a physical frame into a component. */
vaddr_t mman_get_page(spdid_t spd, vaddr_t addr, int flags);
/* 
 * remove this single mapping _and_ all descendents.  FIXME: this can
 * be called with the spdid of a dependent component.  We should also
 * pass in the component id of the calling component to ensure that it
 * is allowed to remove the designated page.
 */
int mman_release_page(spdid_t spd, vaddr_t addr, int flags);
/* int __mman_release_page(spdid_t spd, vaddr_t addr, int flags); */
/* remove all descendent mappings of this one (but not this one). */ 
int mman_revoke_page(spdid_t spd, vaddr_t addr, int flags); 
int __mman_revoke_page(spdid_t spd, vaddr_t addr, int flags);
/* The invoking component (s_spd) must own the mapping. */
vaddr_t mman_alias_page(spdid_t s_spd, vaddr_t s_addr, spdid_t d_spd, vaddr_t d_addr);
vaddr_t __mman_alias_page_rec(spdid_t s_spd, vaddr_t s_addr, spdid_t d_spd, vaddr_t d_addr);
void mman_print_stats(void);

#define CLI_UPCALL_ENABLE   	/* enable recovery thread to upcall into these spds to rebuild the mappings */

#endif 	    /* !MEM_MGR_H */
