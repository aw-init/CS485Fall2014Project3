#define P3SYSCALL_H
#ifndef P3SYSCALL_H

#include <linux/linkage.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
     
#define MAX_BUF_SIZE 1000
#define MAX_VAR_NUM 20

char varnames[MAX_VAR_NUM][MAX_BUF_SIZE];
char vardefs[MAX_VAR_NUM][MAX_BUF_SIZE];

/*This routine saves the specified variable name varname 
and its associated variable definition vardef in the linux kernel. 
It returns 0 upon success and -1 if an error occurs.*/
asmlinkage long sys_SaveVariable(char __user *varname, char __user *vardef);


/*This routine looks for the variable name varname in
 the linux kernel. Upon finding the variable name, it 
 fills in the buffer pointed to by vardef with the
  associated definition. The parameter deflen specifies 
  the size of the vardef buffer. A value of 0 is returned
   if the variable is found, while a value of -1 is
    returned if the variable is not found.*/
asmlinkage long sys_GetVariable(char __user *varname, char __user *vardef, int deflen);

/*This routine can be used to retrieve all the 
variables names and definitions stored in the kernel.
The parameter prevname should be set to the empty string 
(``'') the first time it is called in order to retrieve the 
first variable name and definition stored in the kernel. 
The name and definition will be stored in varname 
and vardef buffers respectively. The size of buffers
 are specified by namelen and deflen respectively. 
 Subsequent calls should set prevname to the last
  variable name retrieved, which will, in turn, 
  retrieve the next variable and its definition.
   If a new variable is found, the procedure will
    return 0. If no more variables are found, 
    a value of -1 will be returned. If an error occurs,
     a value of -2 will be returned.*/
asmlinkage long sys_NextVariable(char __user *prevname, char __user *varname, int namelen, char __user *vardef, int deflen);

#endif