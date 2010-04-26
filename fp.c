#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#define CLDCNT 3
#define BUFSIZE 128
int main ()
{ int i,len,arg3,arg2;
  char* buf = malloc(BUFSIZE);
  printf("type quit to end progran");
while(1){
  printf(">");
  fgets(buf,BUFSIZE-1,stdin);
  if(buf[0]=='q' && buf[1]=='u' && buf[2]=='i' && buf[3]=='t')exit(0);
  len=strlen(buf);
  for(arg3=len-1;arg3>=0 && buf[arg3]!=';';--arg3);
  buf[len-1]=0;
  if(arg3<=0){
    perror("invalid input\n");
    continue;
  }
  buf[arg3++]=0;
  for(arg2=arg3-1;arg2>=0 && buf[arg2]!=';';--arg2);
  if(arg2<=0){
    perror("invalid input\n");
    continue;
  }
  buf[arg2++]=0;
  int p1[2],p2[2];
  if(pipe(p1)){
    fprintf(stderr,"can't piie, %s\n",strerror(errno));
    continue;
  }
  if(pipe(p2)){
    fprintf(stderr,"can't pipe, %s\n",strerror(errno));
    close(p1[0]);
    close(p2[0]);
    continue;
  }
  pid_t cld_pids[CLDCNT];
  //создаем потомков
  for(i=0;i<CLDCNT;++i)
    if((cld_pids[i] = fork())>0){
    //parent
	printf("child %i creat\n",cld_pids[i]);
    }else if(cld_pids[i] == 0){
    //child
	if(i==0){
	  if(dup2(p1[1],1)==-1 || dup2(p2[1],2)==-1){
		fprintf(stderr,"can't dup, %s\n",strerror(errno));
		exit(1);
	  }
	  execlp(buf,(char *)0);
//	  fprintf(stderr,"exec error\n");
	  exit(1);
	}
	if(i==1){
	  if(dup2(p1[0],0)==-1){
		fprintf(stderr,"can't dup on exit pipe, %s\n",strerror(errno));
		exit(1);
		}
	  execlp(buf+arg2,(char *)0);
	  fprintf(stderr,"exec error on cld 2, %s\n",strerror(errno));
	  exit(1);
	}
	if(dup2(p2[0],0)==-1){
		fprintf(stderr,"can't dup on exit pipe 2, %s\n",strerror(errno));
		exit(1);
	}
	execlp(buf+arg3,(char *)0);
	fprintf(stderr,"exec error on cld 3, %s\n",strerror(errno));
	exit(1);
    }else {
	fprintf(stderr,"can't fork cld %i, %s\n",i,strerror(errno));
	if(i>0)kill(cld_pids[0],SIGINT);
	if(i>1)kill(cld_pids[1],SIGINT);
	break;
    }
  //  завершаем работу
  int status,pid;
  close(p1[0]);
  close(p1[1]);
  close(p2[0]);
  close(p2[1]);
   
  for(i=0;i<CLDCNT;++i){
    pid=wait(&status);
    printf("process %i done\n",pid);
  }
}
return 0;
}
