#include <cos_component.h>
#include <print.h>
#include <stdlib.h>
#include <sched.h>
#include <micro_pong.h>
#include <cbuf.h>

#define ITER 2000
#define MAX_SZ 4096
#define NCBUF 100

// using threadpool_max
static void
cbuf_tests(void)
{
	u64_t start, end, start_tmp, end_tmp;
	int i, k, prev_sz = 1;

	cbuf_t cbt[NCBUF];
	memset(cbt, 0 , NCBUF*sizeof(cbuf_t));
	void *mt[NCBUF];
	unsigned int sz[NCBUF];

	for (i = 0; i < NCBUF ; i++){
		cbt[i] = cbuf_null();
		sz[i] = 0;
	}

        /* RDTSCLL */
	printc("\n<<< RDTSCLL MICRO-BENCHMARK TEST >>>\n");
	rdtscll(start_tmp);
	for (i = 0 ; i < ITER ; i++) {
		rdtscll(start);
	}
	rdtscll(end_tmp);
	printc("%d rdtscll avg %lld cycs\n", ITER, (end_tmp-start_tmp)/ITER);

        /* CACHING */
	printc("\n<<< WARM UP CBUF CACHE.......");
	for (i = 0; i < NCBUF ; i++){
		prev_sz += 4;
		prev_sz &= PAGE_SIZE-1;
		sz[i] = prev_sz;		
		mt[i] = cbuf_alloc(sz[i], &cbt[i]);
	}

	for (i = 0; i < NCBUF ; i++){
		simple_call_buf2buf(cbt[i], sz[i]);
	}

	for (i = 0; i < NCBUF ; i++){
		cbuf_free(mt[i]);
	}
	printc(" Done! >>>\n");

        /* CBUF_ALLOC  */
	printc("\n<<< CBUF_ALLOC MICRO-BENCHMARK TEST >>>\n");
	rdtscll(start);
	for (i = 0; i < NCBUF ; i++){
		prev_sz += 4;
		prev_sz &= PAGE_SIZE-1;
		sz[i] = prev_sz;
		mt[i] = cbuf_alloc(sz[i], &cbt[i]); 
	}
	rdtscll(end);
	printc("%d alloc_cbuf %llu cycs\n", NCBUF, (end-start)/NCBUF);
	printc("<<< CBUF_ALLOC MICRO-BENCHMARK TEST DONE >>>\n");

        /* CBUF2BUF  */
	printc("\n<<< CBUF2BUF MICRO-BENCHMARK TEST >>>\n");
	call_buf2buf(cbt[0], sz[0]);
	printc("<<< CBUF2BUF MICRO-BENCHMARK TEST DONE >>>\n");

        /* CBUF_FREE  */
	printc("\n<<< CBUF_FREE MICRO-BENCHMARK TEST >>>\n");
	rdtscll(start);
	for (i = 0; i < NCBUF ; i++){
		cbuf_free(mt[i]);                
	}
	rdtscll(end);
	printc("%d free_cbuf %llu cycs avg\n", NCBUF, (end-start)/NCBUF);
	printc("<<< CBUF_FREE MICRO-BENCHMARK TEST DONE >>>\n");

        /* PINGPONG */
	printc("\n<<< PINGPONG MICRO-BENCHMARK TEST >>>\n");
	for (i = 0 ; i < ITER ; i++) {
		call();
	}
	rdtscll(start);
	for (i = 0 ; i < ITER ; i++) {
		call();
	}
	rdtscll(end);
	printc("%d inv w/o cbuf avg %lld cycs\n", ITER, (end-start)/ITER);
	printc("<<< PINGPONG BENCHMARK TEST DONE >>>\n");

        /* CBUF_ALLOC-CBUF2BUF-CBUF_FREE */
	printc("\n<<< CBUF_ALLOC-CBUF2BUF-CBUF_FREE MICRO-BENCHMARK TEST >>>\n");
	prev_sz += 4;
	prev_sz &= PAGE_SIZE-1;
	sz[0] = prev_sz;
	rdtscll(start);
	for (i = 0; i < ITER ; i++){
		mt[0] = cbuf_alloc(sz[0], &cbt[0]);
		simple_call_buf2buf(cbt[0], sz[0]);
		cbuf_free(mt[0]);
	}
	rdtscll(end);
	printc("%d alloc-cbuf2buf-free %llu cycles avg\n", ITER, (end-start)/ITER);

	printc("<<< CBUF_ALLOC-CBUF2BUF-CBUF_FREE MICRO-BENCHMARK TEST DONE >>>\n");

	return;
}

#define CBUFP_NUM 32
cbufp_t p[CBUFP_NUM];
char *buf[CBUFP_NUM];
cbufp_t p3[CBUFP_NUM];
char *buf3[CBUFP_NUM];

static void
cbufp_tests(void)
{
	unsigned long long start, end;
	struct cbuf_alloc_desc *d;
	int i;

	d = &cbufp_alloc_freelists[0];
	assert(EMPTY_LIST(d, next, prev));
	for (i = 0 ; i < CBUFP_NUM ; i++) {
		buf[i] = cbufp_alloc(4096, &p[i]);
		cbufp_send(p[i]);
		call_cbufp2buf(p[i], 4096);
		assert(buf[i]);

		buf3[i] = cbufp_alloc(4096*3, &p3[i]);
		cbufp_send(p3[i]);
		call_cbufp2buf(p3[i], 4096*3);
		assert(buf3[i]);
	}
	for (i = 0 ; i < CBUFP_NUM ; i++) {
		cbufp_deref(p[i]);
		cbufp_deref(p3[i]);
	}

	rdtscll(start);
	buf[0] = cbufp_alloc(4096, &p[0]);
	assert(buf[0]);
	
	buf3[0] = cbufp_alloc(4096*3, &p3[0]);
	assert(buf3[0]);
	rdtscll(end);
	printc("CBUFP:  garbage collection of %d cbufps: %llu cycles\n", CBUFP_NUM*2, (end-start)/2);

	rdtscll(start);
	for (i = 1 ; i < CBUFP_NUM ; i++) {
		buf[i] = cbufp_alloc(4096, &p[i]);
		assert(buf[i]);

		buf3[i] = cbufp_alloc(4096*3, &p3[i]);
		assert(buf3[i]);
	}
	rdtscll(end);
	printc("CBUFP:  %d alloc %llu cycles avg\n", (CBUFP_NUM-1)*2, (end-start)/((CBUFP_NUM-1)*2));

	for (i = 0 ; i < CBUFP_NUM ; i++) {
		cbufp_send(p[i]);
		call_cbufp2buf(p[i], 4096);

		cbufp_send(p3[i]);
		call_cbufp2buf(p3[i], 4096*3);
	}
	rdtscll(start);
	for (i = 0 ; i < CBUFP_NUM ; i++) {
		cbufp_send(p[i]);
		call_cbufp2buf(p[i], 4096);

		cbufp_send(p3[i]);
		call_cbufp2buf(p3[i], 4096*3);
	}
	rdtscll(end);
	printc("CBUFP:  %d cbuf2buf %llu cycles avg\n", CBUFP_NUM*2, (end-start)/(CBUFP_NUM*2));

	rdtscll(start);
	for (i = 0 ; i < CBUFP_NUM ; i++) {
		cbufp_deref(p[i]);
		cbufp_deref(p3[i]);
	}
	rdtscll(end);
	printc("CBUFP:  %d free %llu cycles avg\n", CBUFP_NUM*2, (end-start)/(CBUFP_NUM*2));

	rdtscll(start);
	for (i = 0 ; i < 1 ; i++) {
		buf[i] = cbufp_alloc(4096, &p[i]);
		cbufp_send_deref(p[i]);
		call_cbufp2buf(p[i], 4096);
		assert(buf[i]);

		buf3[i] = cbufp_alloc(4096*3, &p3[i]);
		cbufp_send_deref(p3[i]);
		call_cbufp2buf(p3[i], 4096*3);
		assert(buf3[i]);
	}
	rdtscll(end);

	printc("CBUFP:  %d UNCACHED alloc-cbuf2buf-send/free %llu cycles avg\n", 2, (end-start)/(2));

	rdtscll(start);
	for (i = 1 ; i < CBUFP_NUM ; i++) {
		buf[i] = cbufp_alloc(4096, &p[i]);
		cbufp_send_deref(p[i]);
		call_cbufp2buf(p[i], 4096);
		assert(buf[i]);

		buf3[i] = cbufp_alloc(4096*3, &p3[i]);
		cbufp_send_deref(p3[i]);
		call_cbufp2buf(p3[i], 4096*3);
		assert(buf3[i]);
	}
	rdtscll(end);

	printc("CBUFP:  %d alloc-cbuf2buf-send/free %llu cycles avg\n", (CBUFP_NUM-1)*2, (end-start)/((CBUFP_NUM-1)*2));
}

void 
cos_init(void)
{
	printc("\nMICRO BENCHMARK TEST (PINGPONG WITH CBUF & CBUFP)\n");

	/* cbuf_tests(); */
	cbufp_tests();

	printc("\nMICRO BENCHMARK TEST (PINGPONG WITH CBUF & CBUFP) DONE!\n\n");
	return;
}


