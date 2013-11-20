#ifndef CBUF_VECT_H
#define CBUF_VECT_H

/* 
 * Since cbuf uses cvect data structure, reuse the code from cvect.h
 * as much as possible the only difference is in expand when request the
 * page from manager
 */

#include <cbuf_c.h>
#include <cbufp.h>
#include <cos_debug.h>
#include <cos_component.h>

#include <cos_alloc.h>

#ifndef CVECT_ALLOC
#define CVECT_ALLOC() alloc_page()
#define CVECT_FREE(x) free_page(x)
#endif
#include <cvect.h>

vaddr_t cbuf_c_register(spdid_t spdid, long cbid);
vaddr_t cbufp_register(spdid_t spdid, long cbid);

static inline int
__cbuf_vect_expand_rec(struct cvect_intern *vi, const long id, const int depth, int tmem)
{
	struct cvect_intern *new;

	if (depth > 1) {
		long n = id >> (CVECT_SHIFT * (depth-1));
		if (vi[n & CVECT_MASK].c.next == NULL) {
			if (tmem) {
				new = (struct cvect_intern *)cbuf_c_register(cos_spd_id(), meta_to_cbid_idx(id));
			} else {
				new = (struct cvect_intern *)cbufp_register(cos_spd_id(), meta_to_cbid_idx(id));			
			}
			if (!new) return -1;
			vi[n & CVECT_MASK].c.next = new;
		}
		return __cbuf_vect_expand_rec(vi[n & CVECT_MASK].c.next, id, depth-1, tmem);
	}
	return 0;
}

static inline int 
cbuf_vect_expand(cvect_t *v, long id, int tmem)
{
	return __cbuf_vect_expand_rec(v->vect, id, CVECT_DEPTH, tmem);
}

#endif /* CBUF_VECT_H */
