#include <linux/linkage.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include "p3syscall.h"

asmlinkage long sys_SaveVariable(char __user *varname, char __user *vardef){
	return(0);
}

asmlinkage long sys_GetVariable(char __user *varname, char __user *vardef, int deflen){
	return(0);
}

asmlinkage long sys_NextVariable(char __user *prevname, char __user *varname, int namelen, char __user *vardef, int deflen){
	return(0);
}