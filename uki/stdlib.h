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

/**
 * @file
 * @brief Use #include <uki/stdlib.h> to use this library.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GFP_KERNEL 0
#define __GFP_ZERO 1

static inline void* kmalloc(size_t size, int flags)
{
	void *mem = malloc(size);
	if (!mem)
		return NULL;
	if (flags & __GFP_ZERO)
		memset(mem, 0x00, size);
	return mem;
}

static inline void kfree(void* ptr)
{
	free(ptr);
}

#define KERN_ALERT "Alert: "
#define KERN_ERR   "Error: "

#define printk(...) printf(__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* UKI_STDLIB_H_ */
