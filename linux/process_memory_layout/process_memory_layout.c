#include <linux/kernel.h>
#include <linux/module.h>

static int __init init(void)
{
        printk("The size of Total Process Virtual memory space = \n");
        printk("The size of User space = %x, %x\n", TASK_SIZE, CONFIG_PAGE_OFFSET);
        printk("The size of Kerenl space = \n");
}

static void __exit exit(void)
{
        printk("process_memory_layout: removed!\n");
}

module_init(init)
module_exit(exit)
MODULE_LICENSE("GPL");
