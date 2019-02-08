/*
 *  Project user_kernel_interface
 *  kernel.h
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

#ifndef UKI_KERNEL_H_
#define UKI_KERNEL_H_

/**
 * @file
 * @brief Use #include <uki/kernel.h> to use this library.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#define container_of(ptr, type, member) ({                      \
                const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
                (type *)( (char *)__mptr - offsetof(type,member) );})

#define min(x, y) ({ \
		typeof(x) _min1 = x; \
		typeof(y) _min2 = y; \
		(void) (&_min1 == &_min2); \
		_min1 <_min2 ? _min1 :  _min2; })

#define max(x, y) ({ \
		typeof(x) _max1 = x; \
		typeof(y) _max2 = y; \
		(void) (&_max1 == &_max2); \
		_max1 > _max2 ? _max1 : _max2; })

/*
 * 'kernel.h' contains some often-used function prototypes etc
 */
#define __ALIGN_KERNEL(x, a) __ALIGN_KERNEL_MASK(x, (typeof(x))(a) - 1)
#define __ALIGN_KERNEL_MASK(x, mask) (((x) + (mask)) & ~(mask))

#define __KERNEL_DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))

#define DIV_ROUND_UP __KERNEL_DIV_ROUND_UP

#ifdef __cplusplus
}
#endif

#endif /* UKI_KERNEL_H_ */
