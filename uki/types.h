/*
 *  Project user_kernel_interface
 *  types.h
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

#ifndef UKI_TYPES_H_
#define UKI_TYPES_H_

/**
 * @file
 * @brief Use #include <uki/types.h> to use this library.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

struct list_head {
	struct list_head *next;
	struct list_head *prev;
};

struct hlist_head {
  struct hlist_node *first;
};

struct hlist_node {
  struct hlist_node *next;
  struct hlist_node **pprev;
};

#ifdef __cplusplus
}
#endif

#endif /* UKI_TYPES_H_ */
