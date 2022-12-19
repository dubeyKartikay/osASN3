#include <linux/kernel.h>
#include <linux/pid.h>
#include <linux/sched.h>
#include <linux/uaccess.h>

asmlinkage long sys_get_process_info(pid_t pid, struct process_info *dest){
    struct task_struct* ts = pid_task(find_vpid(pid),PIDTYPE_PID);
    char str[32];
    sprintf(str,"%d\n",ts->pid);
    printk(KERN_INFO str);
    // free(str);

}