#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define CLDCNT 3
#define BUFSIZE 128
int main ()
{
  //execlp("ls",(char *)0); 
  int i,len,arg3,arg2;
  char* buf = malloc(BUFSIZE);
printf("type quit to end progran");
while(1){
  setjmp();
  printf(">");
  fgets(buf,BUFSIZE-1,stdin);
  if(buf[0]=='q' && buf[1]=='u' && buf[2]=='i' && buf[3]=='t')exit(0);
  len=strlen(buf);
  for(arg3=len-1;arg3>=0 && buf[arg3]!=';';--arg3);
  buf[len-1]=0;
  if(arg3<=0)//perror("invalid input\n");
  buf[arg3++]=0;
  for(arg2=arg3-1;arg2>=0 && buf[arg2]!=';';--arg2);
  if(arg2<=0)perror("invalid input\n");
  buf[arg2++]=0;
  int p1[2],p2[2];
  if(pipe(p1))perror("can't pipe\n");
  if(pipe(p2))perror("can't pipe\n");  
  pid_t cld_pids[CLDCNT];
  //создаем потомков
  for(i=0;i<CLDCNT;++i)
    if((cld_pids[i] = fork())>0){
    //parent
	printf("child %i creat\n",cld_pids[i]);
    }else if(cld_pids[i] == 0){
    //child
	if(i==0){
printf("%s_\n",buf);
	  if(dup2(p1[1],1)==-1)perror("can't dup\n");
	  if(dup2(p2[1],2)==-1)perror("can't dup\n");
	  execlp(buf,(char *)0);
	printf("exec error\n");
	exit(0);
	}
	if(i==1){
	  if(dup2(p1[0],0)==-1)perror("can't dup on exit pipe\n");
printf("%s_\n",buf+arg2);
	  execlp(buf+arg2,(char *)0);
	printf("exec error\n");
	exit(0);
	}
	if(dup2(p2[0],0)==-1)perror("can't dup on exit pipe\n");
printf("%s_\n",buf+arg3);
	execlp(buf+arg3,(char *)0);
	printf("exec error\n");
exit(0);
    }else {
      perror("can't fork\n");
    }
  //  завершаем работу
  int status,pid;
  close(p1[0]);
  close(p1[1]);
  close(p2[0]);
  close(p2[1]);
   
  for(i=0;i<CLDCNT;++i){
  pid=wait(&status);
  printf("process %i done with code %i\n",pid,status);}
}
return 0;
}
