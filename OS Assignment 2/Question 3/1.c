#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched/signal.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Custom system call to count running processes");
MODULE_VERSION("0.1");

static int my_syscall(void) {
    struct task_struct *task;
    int count = 0;

    printk(KERN_INFO "Counting currently running processes:\n");

    // Traverse the task list to count running processes
    for_each_process(task) {
        if (task->state == TASK_RUNNING)
            count++;
    }

    printk(KERN_INFO "Number of currently running processes: %d\n", count);

    return count;
}

static int __init my_init(void) {
    printk(KERN_INFO "Loading my syscall module\n");
    my_syscall();
    return 0;
}

static void __exit my_exit(void) {
    printk(KERN_INFO "Unloading my syscall module\n");
}

module_init(my_init);
module_exit(my_exit);