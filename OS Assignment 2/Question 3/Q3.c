#include <linux/init.h>         // Include the initialization and cleanup headers for Linux kernel modules
#include <linux/module.h>       // Include the module-related headers for Linux kernel modules
#include <linux/kernel.h>       // Include the kernel-related headers for Linux
#include <linux/sched/signal.h> // Include the headers for working with processes in the Linux kernel

MODULE_LICENSE("GPL");          // Declare the license for the module as GPL (General Public License)
MODULE_AUTHOR("Your Name: Sachin");     // Declare the author of the module
MODULE_DESCRIPTION("A custom system call to count running processes and search for a process by PID"); // Describe the module's purpose

static int process_pid = 0;      // Declare a static integer variable to store the target process PID
module_param(process_pid, int, 0); // Declare a module parameter for the target process PID with integer type
MODULE_PARM_DESC(process_pid, "Target process PID"); // Describe the module parameter

void procs_info_print(void)
{
    struct task_struct *task;    // Declare a pointer to a task_struct structure, which represents a process in the kernel
    size_t process_counter = 0;  // Declare a variable to count processes

    printk(KERN_INFO "== Listing all processes ==\n"); // Log a message indicating that we are listing all processes

    for_each_process(task) // Iterate through all processes in the system
    {
        pr_info("== %s [%d]\n", task->comm, task->pid); // Log the name and PID of each process
        ++process_counter; // Increment the process counter
    }

    printk(KERN_INFO "== Number of processes: %zu\n", process_counter); // Log the total number of processes
}

static int __init process_info_init(void)
{
    struct task_struct *task; // Declare a pointer to a task_struct structure
    int process_found = 0;    // Declare a variable to indicate if the target process is found

    printk(KERN_INFO "[ INIT ]\n"); // Log an initialization message

    if (process_pid != 0) // Check if a target process PID was provided as a module parameter
    {
        printk(KERN_INFO "Searching for process with PID: %d\n", process_pid); // Log the PID we are searching for

        for_each_process(task) // Iterate through all processes
        {
            if (task->pid == process_pid) // Check if the current process has the desired PID
            {
                printk(KERN_INFO "Process Name: %s\n", task->comm); // Log the name of the matching process
                printk(KERN_INFO "Process ID: %d\n", task->pid);    // Log the PID of the matching process
                process_found = 1; // Set the flag to indicate that the process was found
                break; // Exit the loop since we found the target process
            }
        }

        if (!process_found) // Check if the target process was not found
        {
            printk(KERN_INFO "Process with PID %d not found\n", process_pid); // Log a message indicating that the process was not found
        }
    }

    procs_info_print(); // Call the function to list and count all processes

    return 0; // Return 0 to indicate successful initialization
}

static void __exit process_info_exit(void)
{
    printk(KERN_INFO "== CLEANUP ]\n"); // Log a message indicating module cleanup
}

module_init(process_info_init); // Specify the initialization function for the module
module_exit(process_info_exit); // Specify the cleanup function for the module
