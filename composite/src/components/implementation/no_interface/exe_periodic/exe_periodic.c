/*
 * Copyright 2009 by Gabriel Parmer, gparmer@gwu.edu.  All rights
 * reserved.
 *
 * Redistribution of this file is permitted under the GNU General
 * Public License v2.
 */

#include <stdlib.h>

#include <cos_component.h>
#include <print.h>

#include <periodic_wake.h>
#include <sched.h>
//#include <sched_conf.h>
#include <exe_synth_hier.h>
#include <timed_blk.h>

/* #include <cos_alloc.h> */
/* #include <cbuf.h> */
/* #include <cos_list.h> */
/* #include <heap.h> */
/* #include <limits.h> */

#define SZ 2048

int period = 100, num_invs = 1;
int start_time = 0, duration_time = 120;

#define US_PER_TICK 10000

int exe_t = 80;  /* in us,less than 2^32/(2.33*10^9/1000) which is 1843 ms on 2.33GHz machine */

#define NUM_LOOPS 1000

volatile unsigned long kkk = 0;
unsigned long loop_cost = 0;
unsigned long get_loop_cost(unsigned long loop_num)
{
	u64_t start,end;
	unsigned long int i;

	kkk = 0;
	rdtscll(start);
	for (i=0;i<loop_num;i++) kkk++;   /* Make sure that -O3 is on to get the best result */
	rdtscll(end);

	return (end-start)/loop_num;  /* avg is 6 per loop */
}

char *parse_step(char *d)
{
	char *s = strchr(d, ' ');
	if (!s) {
		if ('\0' == d) return d;
		s = d + strlen(d);
	} else {
		*s = '\0';
		s++;
	}

	switch(*d) {
	case 'p':		/* spin */
		period = atoi(++d);
		break;
	case 'n':		/* num of invocations */
		num_invs = atoi(++d);
		break;
	case 'e':		/* execution time in us */
		exe_t = atoi(++d);
		break;
	case 's':		/* start running time in sec */
		start_time = atoi(++d);
		break;
	case 'd':		/* duration time in sec */
		duration_time = atoi(++d);
		break;
	}

	return s;
}

int parse_initstr(void)
{
//	struct cos_array *data;
	char *c;

	c = cos_init_args();
	while ('\0' != *c) c = parse_step(c);

	/* data = cos_argreg_alloc(sizeof(struct cos_array) + 52); */
	/* assert(data); */
	/* data->sz = 52; */
	
	/* if (sched_comp_config_initstr(cos_spd_id(), data)) { */
	/* 	printc("No initstr found.\n"); */
	/* 	return -1; */
	/* } */

	/* c = data->mem; */
	/* while ('\0' != *c) c = parse_step(c); */
	
	/* cos_argreg_free(data); */

	return 0;
}

volatile u64_t touch;
volatile int k;

void cos_init(void *arg)
{


	int start_time_in_ticks = 0;
	int duration_time_in_ticks = 0;

	int local_period = 0;

	parse_initstr();

	local_period = period;

	unsigned long cyc_per_tick;
	cyc_per_tick = sched_cyc_per_tick();

	unsigned long exe_cycle;
	exe_cycle = cyc_per_tick/US_PER_TICK;
	exe_cycle = exe_cycle*exe_t;

	start_time_in_ticks = start_time*100;
	duration_time_in_ticks = duration_time*100;

	printc("In spd %ld Thd %d, period %d ticks, execution time %d us in %lu cycles\n", cos_spd_id(),cos_get_thd_id(), local_period, exe_t, exe_cycle);

	int event_thd = 0;
	if (local_period <= 0){/* Create all non-periodic tasks */
		event_thd = cos_get_thd_id();
		/* pub_duration_time_in_ticks = duration_time_in_ticks; */
		timed_event_block(cos_spd_id(), start_time_in_ticks);
	}
	else {/* Create all periodic tasks */
		if (local_period == 0 || (exe_t > local_period*US_PER_TICK)) BUG();
		periodic_wake_create(cos_spd_id(), local_period);

		int i = 0;
		int waiting = 0;

		if(start_time_in_ticks <= 0)
			waiting = 50 / local_period;
		else
			waiting = start_time_in_ticks / local_period;

		do {
			periodic_wake_wait(cos_spd_id());
		} while (i++ < waiting); /* wait 50 ticks */

	}

/* Let all tasks run */
	unsigned long exe_cyc_remained = 0;

	int refill_number = 0;

	unsigned long exe_cyc_event_remained = 0;

	while (1) {
		if(local_period <= 0){			/* used for transient non-periodic tasks only */
			exe_cyc_event_remained = exe_cycle;  /* refill */
			while(1) {
				exe_cyc_event_remained = exe_cycle;  /* refill */
				exe_cyc_event_remained = left(exe_cyc_event_remained,exe_cycle);
				unsigned long t = sched_timestamp();
				if ( t > (unsigned long)(start_time_in_ticks + duration_time_in_ticks)) timed_event_block(cos_spd_id(), 10000);
					/* printc("time elapsed is %llu  cyccs and duration ticks is %d, cyc_per_tick is %lu\n", (end-start), duration_time_in_ticks, cyc_per_tick); */
			}
		}
		else{
			/* rdtscll(start); */
			/* used for transient periodic tasks only */
			if (start_time_in_ticks > 0 && (local_period*refill_number > duration_time_in_ticks)){
				for(;;) periodic_wake_wait(cos_spd_id());
			}

			exe_cyc_remained = exe_cycle;  /* refill */
			while(exe_cyc_remained) {
				exe_cyc_remained = left(exe_cyc_remained,exe_cycle);	  
			}
			
			/* rdtscll(end); */
			/* printc("%d, times : %d\n", cos_get_thd_id(), times); */
			periodic_wake_wait(cos_spd_id());
			refill_number++;	  
		}
	}
	return;
}

void bin (void)
{
	sched_block(cos_spd_id(), 0);
}
