/* Toolkit of transient memory manager */

/* Interfaces of tmem_base and tmem_mgr */

#ifndef TMEM_TK_H
#define TMEM_TK_H

#include <tmem_conf.h>

/* some static inline functions are implemented in tmem.h */

/* implemented in transient mem base and need to call manager */
inline struct spd_tmem_info *get_spd_info(spdid_t spdid);
int tmem_wait_for_mem_no_dependency(struct spd_tmem_info *sti);
int tmem_wait_for_mem(struct spd_tmem_info *sti);
inline int tmem_set_concurrency(spdid_t spdid, int concur_lvl, int remove_spare);
/* 
 * When you're done with a piece of tmem, call this function to wake
 * any threads that were blocked waiting for tmem.
 */
void return_tmem(struct spd_tmem_info *sti);
/* 
 * When we need a piece of tmem, this is appropriate.
 */
tmem_item *tmem_grant(struct spd_tmem_info *sti);
void event_waiting();

/* need to be implemented in manager */
void mgr_map_client_mem(tmem_item *tmi, struct spd_tmem_info *info);
tmem_item *mgr_get_client_mem(struct spd_tmem_info *sti);
void tmem_mark_relinquish_all(struct spd_tmem_info *sti);
void tmem_unmark_relinquish_all(struct spd_tmem_info *sti);
int resolve_dependency(struct spd_tmem_info *sti, int skip_stk);
tmem_item *alloc_item_data_struct(void *l_addr);
void free_item_data_struct(tmem_item *tmi);
tmem_item *free_mem_in_local_cache(struct spd_tmem_info *sti);
void mgr_clear_touched_flag(struct spd_tmem_info *sti);

#endif
