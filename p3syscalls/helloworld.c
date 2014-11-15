#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>

/* Replace this with your name */
#define author "Jun Huang RENEW"
#define __NR_helloworld  314
#define __NR_SaveVar 315
#define __NR_GetVar 316
#define __NR_NextVar 317
main(){
   int retval; 
   int retvalMyCall;
    
   int i_SaveVar2;
   printf("About to call the new helloworld() system call\n");
   retval = syscall(__NR_helloworld, author, strlen(author));
   retvalMyCall = syscall(__NR_SaveVar, author, "I am really happy!");
   i_SaveVar2 = syscall(__NR_SaveVar, "Andrew 12", "Rodger");
   int strlen;
   int ret2;
   char vardef[1000];
   strcpy(vardef,"didn't get called");
   ret2 = syscall(__NR_GetVar, author, vardef, &strlen);
   printf("Done calling helloworld()\n");
   printf("Your helloworld() system call returned the value %d\n", retval);
   printf("Your mysyscall  system call returned the value %d\n", retvalMyCall);
   printf("2nd save var call returned value %d\n", i_SaveVar2);
   printf("Your getVarcall  system call returned the value %d\n", ret2);
   printf("The vardef got back from GetVar is %s\n",vardef);

   	
   int retNext;
   char varname[1000];
   char vardefnext[1000];
   int iNamelen, iDeflen;
   retNext = syscall(__NR_NextVar,author,varname,&iNamelen,vardefnext,&iDeflen);
   
   printf("next call returned status %d\n",retNext);
   //if (retNext == 0)
  // {
	printf("Varname is %s, varname len is %d, vardef is %s, vardef len is %d\n",varname,iNamelen,vardefnext,iDeflen);
  // } 

}
