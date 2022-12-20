#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/kallsyms.h>

asmlinkage long sys_hello(void) {
    printk(KERN_INFO "Hello, kernel world!\n");
    return 0;
}

static unsigned long *sys_call_table;

static int __init hello_init(void) {
    // Get a pointer to the sys_call_table array
    sys_call_table = (unsigned long*)kallsyms_lookup_name("sys_call_table");

    // Replace the entry for the sys_hello syscall with our custom implementation
    disable_wp();
    sys_call_table[__NR_hello] = (unsigned long)sys_hello;
    enable_wp();

    printk(KERN_INFO "Hello kernel module loaded.\n");
    return 0;
}

static void __exit hello_exit(void) {
    // Restore the original sys_hello syscall
    disable_wp();
    sys_call_table[__NR_hello] = (unsigned long)sys_hello;
    enable_wp();

    printk(KERN_INFO "Hello kernel module unloaded.\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("John Doe");
MODULE_DESCRIPTION("A simple kernel module with a custom syscall");

module_init(hello_init);
module_exit(hello_exit);
