/*
 *  Project user_kernel_interface
 *  Copyright (C) 2015  tania@df9ry.de
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

#include <stdio.h>
#include <stdlib.h>

#include "uki/list.h"

extern void test_container(void);
extern void test_list(void);
extern void test_timer(void);

int main(int argc, char *argv[]) {
	puts("Test container");
	test_container();

	puts("Test list");
	test_list();

	puts("Test timer");
	test_timer();

	puts("Success");
	return EXIT_SUCCESS;
}

