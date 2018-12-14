/*
 *  Project user_kernel_interface
 *  container_test.c
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


#include "uki/kernel.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct myStruct { int a, b; };

static void test1(void)
{
    struct myStruct var = {.a = 0, .b = 0};

    int *memberPointer = &var.b;
    printf("Struct addr=%p\n", &var);
    struct myStruct *newSp = container_of(memberPointer, struct myStruct, b);
    printf("Struct addr new=%p\n", newSp);
    assert(newSp == &var);
    printf("It's equal.\n");
}

void test_container(void) {
	test1();
}

