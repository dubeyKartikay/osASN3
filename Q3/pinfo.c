#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/uidgid.h>
#include<linux/syscalls.h>
#include<linux/unistd.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kartikay Dubey");
MODULE_DESCRIPTION("Os assignment ");

static int pid = 0;
module_param(pid, int, 0);

static int __init pinfo_init(void) {

   struct task_struct *task;

    task = pid_task(find_vpid(pid), PIDTYPE_PID);
    if (!task) {
        printk(KERN_INFO "no tasks with pid : %d\n", pid);
    }

    printk(KERN_INFO "pid = %d\n", task->pid);
    printk(KERN_INFO "uid = %d\n",task->cred->uid.val);
    printk(KERN_INFO "pgid = %d\n", task->group_leader->pid);
    printk(KERN_INFO "command path = %s\n", task->comm);

    return 0;
  
}

static void __exit pinfo_exit(void) {
    printk(KERN_INFO "module unloaded\n");
}

module_init(pinfo_init);
module_exit(pinfo_exit);
