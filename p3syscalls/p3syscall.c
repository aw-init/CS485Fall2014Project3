#include <linux/linkage.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>


#define P3_DEBUG 1
#define MAX_BUF_SIZE 1000
#define MAX_VAR_NUM 20

char varnames[MAX_VAR_NUM][MAX_BUF_SIZE];
char vardefs[MAX_VAR_NUM][MAX_BUF_SIZE];

char emptyStr[MAX_BUF_SIZE];
char k_varname[MAX_BUF_SIZE];
char k_vardef[MAX_BUF_SIZE];

asmlinkage long sys_SaveVariable(char __user *varname, char __user *vardef){
	int i = 0 ;

	int varnameLen = strlen(varname);
	int vardefLen = strlen(vardef);
	if ( (varnameLen < 1) || (varnameLen >  MAX_BUF_SIZE) ) {
        printk(KERN_EMERG "SaveVariable() varname failed: illegal len (%d)!", varnameLen);
        return(-1);
    }

    if ( (vardefLen < 1) || (vardefLen >  MAX_BUF_SIZE) ) {
        printk(KERN_EMERG "SaveVariable() vardef failed: illegal len (%d)!", vardefLen);
        return(-1);
    }


    if (copy_from_user(k_varname, varname, varnameLen)) {
        printk(KERN_EMERG "SaveVariable() failed: copy_from_user() error on varname");
        return(-1);
 	}

 	if (copy_from_user(k_vardef, vardef, vardefLen)) {
        printk(KERN_EMERG "SaveVariable() failed: copy_from_user() error on vardef");
        return(-1);
 	}

	for(; i < MAX_VAR_NUM; ++i){
		if(!strcmp(varnames[i],emptyStr) || !strcmp(varnames[i],k_varname)){
			strcpy(varnames[i],k_varname);
			varnames[i][varnameLen] = '\0';

			strcpy(vardefs[i],k_vardef);
			vardefs[i][vardefLen] = '\0';

			printk(KERN_EMERG "The variable name is %s, and the variable def is %s",varnames[i],vardefs[i]);
			printk(KERN_EMERG "SaveVariable() save success!");
			return(0);
		}

	}

	printk(KERN_EMERG "SaveVariable() failed, no empty slot available!");
	return(-1);
}

asmlinkage long sys_GetVariable(char __user *varname, char __user *vardef, int __user *deflen){
	int i;
	int len;
	int bResult;
	int iCopyToUserRes;

	printk(KERN_EMERG "Entered GetVar routine varname is %s", varname);

	if (P3_DEBUG){
		i = 0;
		for(; i < MAX_VAR_NUM; i++){
			printk(KERN_EMERG "at index %d the varname is %s and vardef is %s\n",i,varnames[i],vardefs[i]);

			bResult = strcmp(varname,varnames[i]);

			printk(KERN_EMERG "The result is %d",bResult);
		}
	}
	i = 0;
	for(; i < MAX_VAR_NUM ; i++){
		if(strcmp(varname,varnames[i]) == 0){
			printk(KERN_EMERG "Found matching var\n");
			
			
			strcpy(k_vardef,vardefs[i]);
			len = strlen(k_vardef);
			printk(KERN_EMERG "The vardef length is %d , the value is %s\n",len,k_vardef);
			iCopyToUserRes = copy_to_user(vardef, k_vardef, len);
			printk(KERN_EMERG "The copy return value is %d\n" , iCopyToUserRes);
			if(iCopyToUserRes) 
			{
		        printk(KERN_EMERG "GetVariable() failed: copy_to_user() error on vardef");
		        return(-1);
		 	}
		 	printk(KERN_EMERG "Copied var to user, the vardef is %s", vardef);
		 	
		 	vardef[len] = '\0';
		 	len = len+1;
		 	if(copy_to_user(deflen, &len, sizeof(int))) {
		 		printk(KERN_EMERG "Failed to copy to user for the variable length");
		 		return(-1);
		 	}
		 	printk(KERN_EMERG "GetVariable() get vairable success!");
		 	return(0);
		}
	}

	printk(KERN_EMERG "GetVariable() failed, no variable named (%s) found!",varname);

	return(-1);
}

asmlinkage long sys_NextVariable(char __user *prevname, char __user *varname, int __user *namelen, char __user *vardef, int __user *deflen){

	int i = 0;
	int len;
	for(; i < MAX_VAR_NUM ; i++){	
		if(!strcmp(prevname,varnames[i])){
			if(i +1 >= MAX_VAR_NUM){
				printk(KERN_EMERG "NextVariable() failed reached end of list");
				return(-1);
			}
			i +=1;
			
			len = strlen(vardefs[i]);
			if (copy_to_user(vardef, vardefs[i], len)) {
		        printk(KERN_EMERG "NextVariable() failed: copy_to_user() error on vardef");
		        return(-1);
		 	}
		 	
		 	vardef[len] = '\0';
		 	len += 1;

		 	if(copy_to_user(deflen, &len, sizeof(int))) {
		 		printk(KERN_EMERG "Failed to copy to user for the variable length");
		 		return(-1);
		 	}

		 	//printk(KERN_EMERG "NextVariable() get next variable success!");

		 	len = 0;
		 	len = strlen(varnames[i]);
			if (copy_to_user(varname, varnames[i], len)) {
		        printk(KERN_EMERG "NextVariable() failed: copy_to_user() error on varname");
		        return(-1);
		 	}
		 	
		 	varname[len] = '\0';
		 	len += 1;

		 	if(copy_to_user(namelen, &len, sizeof(int))) {
		 		printk(KERN_EMERG "Failed to copy to user for the variable length");
		 		return(-1);
		 	}
		 	printk(KERN_EMERG "NextVariable() get next variable success!");
		 	return(0);
		}
	}

	printk(KERN_EMERG "NextVariable() failed, reached end of list, no matching variable");

	return(-1);
}