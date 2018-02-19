/* 
 * Sample kernel loadable module code for list usage in linux kernel.
 *
 * (C) 2018 by Murat Sezgin <msezgin@iotdweb.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/list.h>

/*
 * my_object structure.
 */
struct my_object {
	int data;
	struct list_head list;
};

/*
 * list_init()
 */
static int list_init(void)
{
	struct my_object *tmp;
			/* Temporary object created for each list member */
	struct my_object obj_list;
			/* List constructed with the tmp objects */
	struct list_head *pos, *n;
			/* Position pointers in the list */
	int i;

	printk(KERN_INFO "list init\n");

	/*
	 * Initialize the list object. This object is used only as the
	 * head of the list. It doesn't have any data in it.
	 */
	INIT_LIST_HEAD(&obj_list.list);

	for (i = 0; i < 10; i++) {
		tmp = kmalloc(sizeof(struct my_object), GFP_KERNEL);
		if (!tmp) {
			goto list_cleanup;
		}
		tmp->data = i;
		INIT_LIST_HEAD(&tmp->list);

		/*
		 * Add object to the head of the list.
		 * See list_add_tail() function for adding the object
		 * to the end of the list.
		 */
		list_add(&tmp->list, &obj_list.list);
	}

	/*
	 * Traverse the list and print what we have in the list.
	 */
	list_for_each(pos, &obj_list.list) {
		tmp = list_entry(pos, struct my_object, list);
		printk(KERN_INFO "%d", tmp->data); 
	}
	
list_cleanup:
	/*
	 * We should use list_for_each_safe() function while we are
	 * deleting or moving an object from the list.
	 */
	list_for_each_safe(pos, n, &obj_list.list) {
		tmp = list_entry(pos, struct my_object, list);
		printk(KERN_INFO "Deleting %d", tmp->data);
		list_del(pos);
		kfree(tmp);
	}
	return 0;
}

/*
 * list_exit()
 */
static void list_exit(void)
{
	printk(KERN_INFO "list exit\n");
}
module_init(list_init);
module_exit(list_exit);
