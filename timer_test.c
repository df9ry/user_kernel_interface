/*
 *  Project user_kernel_interface
 *  list_test.c
 *  Copyright (C) 2019 - Tania Hagn - tania@df9ry.de
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "uki/timer.h"
#include "uki/stdlib.h"
#include "uki/jiffies.h"
#include "uki/delay.h"

#include <stdio.h>
#include <unistd.h>
#include <assert.h>

static volatile unsigned long testnum = 0;

static void timer_callback(unsigned long data) {
	testnum = data;
}

struct timer_list static_timer = TIMER_INITIALIZER(timer_callback, 0, 99);

static void do_test_timer(struct timer_list *timer, unsigned long num) {
	int i;
	testnum = 0;
	timer->expires = jiffies + 1*HZ;
	add_timer(timer);
	for (i = 0; (testnum != num) && (i < 20); ++i)
		msleep(100);
	//printf("i=%i\n", i);
	assert((i >= 8) && (i <= 12));
	int erc = mod_timer(timer, jiffies + 1*HZ);
	assert(erc == 0);
	erc = del_timer(timer);
	assert(erc == 1);
}

static void test_static_timer(void) {
	printf("Test static timer\n");
	do_test_timer(&static_timer, 99);
	printf("OK\n");
}

static void test_dynamic_timer(void) {
	struct timer_list dynamic_timer;
	printf("Test dynamic timer\n");
	init_timer(&dynamic_timer);
	dynamic_timer.data = 101;
	dynamic_timer.function = timer_callback;
	do_test_timer(&dynamic_timer, 101);
	printf("OK\n");
}

static void test_jiffies_running(void) {
	int i, n = 0;
	unsigned long j = 0;
	printf("Test jiffies running\n");
	for (i = 0; i < 10; ++i) {
		assert(jiffies >= j);
		if (jiffies > j) {
			j = jiffies;
		} else {
			n++;
		}
		msleep(100);
	} /* end for */
	//printf("  n=%i\n", n);
	assert(n < 3);
	printf("OK\n");
}

void test_timer(void) {
	init_timers();
	test_jiffies_running();
	test_static_timer();
	test_dynamic_timer();
}

