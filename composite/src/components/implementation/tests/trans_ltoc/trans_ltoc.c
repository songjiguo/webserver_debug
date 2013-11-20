/**
 * Copyright 2012 by The George Washington University.  All rights reserved.
 *
 * Redistribution of this file is permitted under the GNU General
 * Public License v2.
 *
 * Author: Qi Wang, interwq@gwu.edu, 2012
 */

#include <cos_component.h>
#include <print.h>
#include <cos_alloc.h>
#include <cos_list.h>
#include <cinfo.h>
#include <sched.h>
#include <mem_mgr_large.h>

#include <valloc.h>

#include <cos_vect.h>
#include <cos_synchronization.h>

#include <cringbuf.h>

#include <pong.h>

cos_lock_t l;
#define TAKE()    do { if (unlikely(lock_take(&l) != 0)) BUG(); }   while(0)
#define RELEASE() do { if (unlikely(lock_release(&l) != 0)) BUG() } while(0)
#define LOCK_INIT()    lock_static_init(&l);

struct channel_info {
	int exists, direction;
	/* if channel is read-only (direction = COS_TRANS_DIR_LTOC),
	   there is only one torrent, otherwise, NULL */
	struct cringbuf rb;
} channels[10];

#define ITER (1024)
unsigned int meas[ITER], idx = 0;

void report(void)
{
	int i,j, outlier = 0;
	unsigned long long sum = 0, sum2 = 0, dev = 0, avg;
	for (i = 0; i < ITER; i++) {
		sum += meas[i];
	}
	avg = (unsigned long)(sum / ITER);
	printc("avg %llu over %d meas\n",avg, ITER); 
	for (i = 0 ; i < ITER ; i++) {
		u64_t diff = (meas[i] > avg) ? 
			meas[i] - avg : 
			avg - meas[i];
		dev += (diff*diff);
	}
	dev /= ITER;
	printc("deviation^2 = %llu\n", dev);

	for (i = 0; i < ITER; i++) {
		if (meas[i] < 3*avg) {
			sum2 += meas[i];
		} else {
			outlier++;
			printc("outlier %u\n", meas[i]);
		}
	}
	assert(ITER > outlier);
	printc("avg %d w/o %d outliers\n", 
	       (int)(sum2/(ITER-outlier)), outlier);
}

void read_ltoc(void) 
{
	char *addr, *start;
	unsigned long i, sz;
	unsigned short int bid;
	int direction;
	int channel = COS_TRANS_SERVICE_PONG;
	char buf[512];

	direction = cos_trans_cntl(COS_TRANS_DIRECTION, channel, 0, 0);
	if (direction < 0) {
		channels[channel].exists = 0;
		return;
	}  
	channels[channel].exists = 1;
	channels[channel].direction = direction;

	sz = cos_trans_cntl(COS_TRANS_MAP_SZ, channel, 0, 0);
	assert(sz <= (4*1024*1024)); /* current 8MB max */
	start = valloc_alloc(cos_spd_id(), cos_spd_id(), sz/PAGE_SIZE);
	assert(start);
	for (i = 0, addr = start ; i < sz ; i += PAGE_SIZE, addr += PAGE_SIZE) {
		assert(!cos_trans_cntl(COS_TRANS_MAP, channel, (unsigned long)addr, i));
	}
	cringbuf_init(&channels[channel].rb, start, sz);

	assert(direction == COS_TRANS_DIR_LTOC);

	bid = cos_brand_cntl(COS_BRAND_CREATE, 0, 0, cos_spd_id());
	assert(bid > 0);
	assert(!cos_trans_cntl(COS_TRANS_BRAND, channel, bid, 0));
	if (sched_add_thd_to_brand(cos_spd_id(), bid, cos_get_thd_id())) BUG();

	while (1) {
		int ret, i;
		char *p;
		struct channel_info *info;
		unsigned long long *t, local_t;
//		printc("going to wait for input...\n");
		if (-1 == (ret = cos_brand_wait(bid))) BUG();
		rdtscll(local_t);
		ret = cringbuf_consume(&channels[channel].rb, buf, 512);
		p = buf;
//		while (*p != '\0') {
		t = (unsigned long long *)p;
		meas[idx++] = (local_t - *t);
		assert(local_t > *t);
//		printc("local t %llu, start %llu, diff %u\n", local_t, *t, meas[idx-1]);
		*p = '\0';
//			p++;
//		}

		if (idx == ITER) break;
	}
	report();


	return;
}

/**
 * cos_init
 */
void
cos_init(void *arg)
{
	static int first = 1;

	if (first) {
		union sched_param sp;
		first = 0;

		LOCK_INIT();
		sp.c.type = SCHEDP_PRIO;
		sp.c.value = 10;
		if (sched_create_thd(cos_spd_id(), sp.v, 0, 0) == 0) BUG();
		return;
	}

	read_ltoc();
//	memset(all_tmem_mgr, 0, sizeof(struct tmem_mgr *) * MAX_NUM_SPDS);

	return;
}
