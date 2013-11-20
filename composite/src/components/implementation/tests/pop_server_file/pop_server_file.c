#include <cos_component.h>
#include <print.h>
#include <sched.h>
#include <cbuf.h>
#include <evt.h>
#include <torrent.h>

#include <timed_blk.h>

char buffer[1024];

void pop_cgi(void)
{
	td_t t1;
	long evt1;
	char *params = "test";
	char *data1 = "hello_world";
	unsigned int ret1, ret2;

	printc("pop the file on the server\n");
	evt1 = evt_split(cos_spd_id(), 0, 0);
	assert(evt1 > 0);

	t1 = tsplit(cos_spd_id(), td_root, params, strlen(params), TOR_ALL, evt1);
	if (t1 < 1) {
		printc("split failed\n");
		return;
	}

	ret1 = twrite_pack(cos_spd_id(), t1, data1, strlen(data1));
	/* ret2 = twrite_pack(cos_spd_id(), t2, data2, strlen(data2)); */
	trelease(cos_spd_id(), t1);

	printc("unsigned long long length %d\n", sizeof(unsigned long long));
	printc("unsigned long length %d\n", sizeof(unsigned long));
	printc("unsigned int length %d\n", sizeof(unsigned int));
	printc("unsigned short int length %d\n", sizeof(unsigned short int));
	printc("\n<<<pop done!!>>>\n\n");
	return;
}

void cos_init(void)
{
	static int first = 0, second = 0;
	union sched_param sp;
	
	if(first == 0){
		first = 1;

		sp.c.type = SCHEDP_PRIO;
		sp.c.value = 8;
		sched_create_thd(cos_spd_id(), sp.v, 0, 0);
	} else {
		if (second == 0) {
			second = 1;
			timed_event_block(cos_spd_id(), 1);
			pop_cgi();			
		}
	}	
	return;
}
