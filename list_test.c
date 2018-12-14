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

#include "uki/list.h"
#include "uki/stdlib.h"

struct test_data
{
        int no;
        struct list_head list;
};

struct test_data head = {
        .no = -1,
};

static void listtest_push_no(int no)
{
        struct test_data *new_data;
        new_data = kmalloc(sizeof(struct test_data), GFP_KERNEL);
        if(new_data) {
                new_data->no = no;
                list_add(&new_data->list, &head.list);
        }
}

static void listtest_show_list(void)
{
        struct list_head *listptr;
        struct test_data *entry;
        printk(KERN_ALERT "show_list\n");

        printk(KERN_ALERT "no = %d (list %p, prev = %p, next = %p)\n",
               head.no, &head.list, head.list.prev, head.list.next);

        list_for_each(listptr, &head.list) {
                entry = list_entry(listptr, struct test_data, list);
                printk(KERN_ALERT "no = %d (list %p, prev = %p, next = %p)\n",
                       entry->no, &entry->list, entry->list.prev, entry->list.next);
        }
}

static void listtest_pop(void)
{
        struct test_data *ptr;
        ptr = list_entry(head.list.next, struct test_data, list);
        list_del(head.list.next);
        kfree(ptr);
}

static int test1(void)
{
        struct test_data *ptr;
        struct test_data *new_data;

        INIT_LIST_HEAD(&head.list);
        listtest_show_list();

        new_data = kmalloc(sizeof(struct test_data), GFP_KERNEL);
        new_data->no = 1;
        list_add(&new_data->list, &head.list);
        listtest_show_list();

        ptr = list_entry(head.list.next, struct test_data, list);
        list_del(head.list.next);

        listtest_show_list();

        kfree(ptr);
        return 0;
}

static int test2(void)
{
        int i;
        printk(KERN_ALERT "Listtest init\n");
        INIT_LIST_HEAD(&head.list);

        for(i = 0; i < 3; ++i) {
                listtest_push_no(i);
                listtest_show_list();
        }

        for(i = 0; i < 3; ++i) {
                listtest_pop();
                listtest_show_list();
        }

        return 0;
}

void test_list(void) {
	test1();
	test2();
}

