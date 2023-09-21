Linux Kernel Module Process : This is a custom Linux kernel module that counts the running process and search for process ID

How to run : a). make
		 b). ls
		 c). sudo insmod file_name.ko
		 d). dmesg

1). Module Initialization and Cleanup:

- The module initializes by defining its license, author, and description.
- It declares a module parameter `process_pid` to specify the target process PID.
- The module_init specifies the initialization function , process_info_init.
- The module_exit specifies the cleanup function, process_info_exit.
			
2). procs_info_print function: 

- This function lists all processes in the system.
- It iterates through all processes using the for_each_process.
- For each process, it logs the process name and PID.
- It keeps track of the total number of processes.

3). process_info_init function:

- This is the module's initialization function.
- It logs an initialization message.
- If a target process PID is provided as a module parameter, it searches for the process.
- It iterates through all processes using for_each_process and compares their PIDs.
- If a match is found, it logs the process name and PID and sets process_found to 1.
- If the target process is not found, it logs a message.
- Finally, it calls `procs_info_print` to list and count all processes.

4). process_info_exit function:

- This is the module's cleanup function.
- It logs a cleanup message.