/*
 *  Project user_kernel_interface
 *  memory.h
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

#ifndef UKI_MEMORY_H_
#define UKI_MEMORY_H_

/**
 * @file
 * @brief Use #include <uki/memory.h> to use this library.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#define WRITE_ONCE(x, val) x=(val)
#define READ_ONCE(y) y

#ifdef __cplusplus
}
#endif

#endif /* UKI_MEMORY_H_ */
