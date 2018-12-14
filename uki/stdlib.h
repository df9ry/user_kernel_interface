/*
 *  Project user_kernel_interface
 *  stdlib.h
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

#ifndef UKI_STDLIB_H_
#define UKI_STDLIB_H_

/** @file */

#include <stdio.h>

#define GFP_KERNEL 0

static inline void* kmalloc(size_t size, int flags)
{
	return malloc(size);
}

static inline void kfree(void* ptr)
{
	free(ptr);
}

#define KERN_ALERT "Alert: "

#define printk(...) printf(__VA_ARGS__)

#endif /* UKI_STDLIB_H_ */
