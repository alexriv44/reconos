#include "reconos.h"
#include "reconos_app.h"
#include "mbox.h"
#include "timer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define LOAD_GET 0x00000010
#define LOAD_PUT 0x00000020
#define EVAL_GETSW 0x00000010
#define EVAL_GETHW 0x00000011
#define EVAL_PUTSW 0x00000020
#define EVAL_PUTHW 0x00000021

#define TIMER_DIFF(ts0,ts1) (ts0 < ts1 ? ts1 - ts0 : ts0 - ts1)

#define _LOAD1
#define _LOAD2

#define REPEAT 2000
#define SLEEP  1000

pthread_t load11, load12;
void *load_entry1 (void *data) {
#ifdef LOAD1
	volatile int i = 0;

	// hardware always performs mbox_get
	mbox_put(load_cmd_ptr, LOAD_GET);
	while(1) {
		// put to mbox
		mbox_put(load_mbox_ptr, 0xaffedead);
		// busy wait for system load
		//for (i = 0; i < 1000; i++);
	}
#endif

	return 0;
}

pthread_t load21, load22;
void *load_entry2 (void *data) {
#ifdef LOAD2
	volatile int i = 1;
	while (i);
#endif

	return 0;
}

int main(int argc, char **argv) {
	int i;
	uint32_t ts0, ts1, tsavg = 0;
	printf("Hello World\n");

	reconos_init();
	reconos_app_init();
	timer_init();

#ifdef LOAD1
	printf("Generating system load ...\n");
#endif
#ifdef LOAD2
	printf("Generating more system load ...\n");
#endif

	reconos_thread_create_hwt_load();
	pthread_create(&load11, NULL, load_entry1, NULL);
	//pthread_create(&load12, NULL, load_entry1, NULL);
	pthread_create(&load21, NULL, load_entry2, NULL);
	//pthread_create(&load22, NULL, load_entry2, NULL);

	usleep(10000);

	reconos_thread_create_hwt_eval();
	reconos_thread_create_hwt_eval();
	reconos_thread_create_swt_eval();
	reconos_thread_create_swt_eval();

	printf("Evaluating hw-sw communication ...\n");
	tsavg = 0;
	for (i = 0; i < REPEAT; i++) {
		timer_reset();
		mbox_put(eval_cmdsw_ptr, EVAL_GETSW);
		usleep(SLEEP);
		mbox_put(eval_cmdhw_ptr, EVAL_PUTSW);
		ts0 = mbox_get(eval_time_ptr);
		ts1 = mbox_get(eval_time_ptr);
		tsavg += TIMER_DIFF(ts0, ts1);
	}
	printf("  SwRes took %d cc\n", tsavg / REPEAT);
	tsavg = 0;
	for (i = 0; i < REPEAT; i++) {
		timer_reset();
		mbox_put(eval_cmdsw_ptr, EVAL_GETHW);
		usleep(SLEEP);
		mbox_put(eval_cmdhw_ptr, EVAL_PUTHW);
		ts0 = mbox_get(eval_time_ptr);
		ts1 = mbox_get(eval_time_ptr);
		tsavg += TIMER_DIFF(ts0, ts1);
	}
	printf("  HwRes took %d cc\n", tsavg / REPEAT);

	printf("Evaluating hw-hw communication ...\n");
	tsavg = 0;
	for (i = 0; i < REPEAT; i++) {
		timer_reset();
		mbox_put(eval_cmdhw_ptr, EVAL_GETSW);
		usleep(SLEEP);
		mbox_put(eval_cmdhw_ptr, EVAL_PUTSW);
		ts0 = mbox_get(eval_time_ptr);
		ts1 = mbox_get(eval_time_ptr);
		tsavg += TIMER_DIFF(ts0, ts1);
	}
	printf("  SwRes took %d cc\n", tsavg / REPEAT);
	tsavg = 0;
	for (i = 0; i < REPEAT; i++) {
		timer_reset();
		mbox_put(eval_cmdhw_ptr, EVAL_GETHW);
		usleep(SLEEP);
		mbox_put(eval_cmdhw_ptr, EVAL_PUTHW);
		ts0 = mbox_get(eval_time_ptr);
		ts1 = mbox_get(eval_time_ptr);
		tsavg += TIMER_DIFF(ts0, ts1);
	}
	printf("  HwRes took %d cc\n", tsavg / REPEAT);

	printf("Evaluating sw-sw communication ...\n");
	tsavg = 0;
	for (i = 0; i < REPEAT; i++) {
		timer_reset();
		mbox_put(eval_cmdsw_ptr, EVAL_GETSW);
		usleep(SLEEP);
		mbox_put(eval_cmdsw_ptr, EVAL_PUTSW);
		ts0 = mbox_get(eval_time_ptr);
		ts1 = mbox_get(eval_time_ptr);
		tsavg += TIMER_DIFF(ts0, ts1);
	}
	printf("  SwRes took %d cc\n", tsavg / REPEAT);
	tsavg = 0;
	for (i = 0; i < REPEAT; i++) {
		timer_reset();
		mbox_put(eval_cmdsw_ptr, EVAL_GETHW);
		usleep(SLEEP);
		mbox_put(eval_cmdsw_ptr, EVAL_PUTHW);
		ts0 = mbox_get(eval_time_ptr);
		ts1 = mbox_get(eval_time_ptr);
		tsavg += TIMER_DIFF(ts0, ts1);
	}
	printf("  HwRes took %d cc\n", tsavg / REPEAT);

	printf("Evaluating sw-hw communication ...\n");
	tsavg = 0;
	for (i = 0; i < REPEAT; i++) {
		timer_reset();
		mbox_put(eval_cmdhw_ptr, EVAL_GETSW);
		usleep(SLEEP);
		mbox_put(eval_cmdsw_ptr, EVAL_PUTSW);
		ts0 = mbox_get(eval_time_ptr);
		ts1 = mbox_get(eval_time_ptr);
		tsavg += TIMER_DIFF(ts0, ts1);
	}
	printf("  SwRes took %d cc\n", tsavg / REPEAT);
	tsavg = 0;
	for (i = 0; i < REPEAT; i++) {
		timer_reset();
		mbox_put(eval_cmdhw_ptr, EVAL_GETHW);
		usleep(SLEEP);
		mbox_put(eval_cmdsw_ptr, EVAL_PUTHW);
		ts0 = mbox_get(eval_time_ptr);
		ts1 = mbox_get(eval_time_ptr);
		tsavg += TIMER_DIFF(ts0, ts1);
	}
	printf("  HwRes took %d cc\n", tsavg / REPEAT);

	while(1)
		usleep(1000000);

	reconos_app_cleanup();
	reconos_cleanup();
	timer_cleanup();
	
	return 0;
}