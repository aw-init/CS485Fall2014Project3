In order to compile kernel one needs to modify the following:


Do the following on your vm:


##Add file to kernel###
cd ~/linux/linux-source-3.13.0/kernel

then copy the p3syscall.c file to this folder



###Change kernel make file###
vim ~/linux/linux-source-3.13.0/kernel/Makefile

(or change it with any other editor other than vim)


add p3syscall.o to the definition of obj-y, like:
obj-y     = fork.o exec_domain.o panic.o \
            cpu.o exit.o itimer.o time.o softirq.o resource.o \
            sysctl.o sysctl_binary.o capability.o ptrace.o timer.o user.o \
            signal.o sys.o kmod.o workqueue.o pid.o task_work.o \
            extable.o params.o posix-timers.o \
            kthread.o sys_ni.o posix-cpu-timers.o \
            hrtimer.o nsproxy.o \
            notifier.o ksysfs.o cred.o reboot.o \
            async.o range.o groups.o smpboot.o hello.o p3syscall.o


###Add new sys call to sys call table###

vim ~/linux/linux-source-3.13.0/arch/x86/syscalls/syscall_64.tbl

add these near the bottom:

315     common  p3sysSaveVar            sys_SaveVariable
316     common  p3sysGetVar             sys_GetVariable
317     common  p3sysNextVar            sys_NextVariable

####modify sys call header to be able to use new sys calls ###

vim ~/linux/linux-source-3.13.0/include/linux/syscalls.h

Add these lines near the bottom before #endif:

asmlinkage long sys_SaveVariable(char __user *varname, char __user *vardef);

asmlinkage long sys_GetVariable(char __user *varname, char __user *vardef, int __user *deflen);

asmlinkage long sys_NextVariable(char __user *prevname, char __user *varname, int __user *namelen, char __user *vardef, int __user *deflen);