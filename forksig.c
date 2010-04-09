#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <signal.h>

void sigevent(int k)
{
  printf("PID %i hear %i\n",getpid(),k);
}

int main (int argс, char* argv[])
{ //создаем сою группу процессов
  struct sigaction new,old;
  new.sa_handler=&sigevent;
//  sigemptyset(new.sa_mask);
  new.sa_flags=0;
  sigaction(SIGUSR1,&new,&old);
  if(setpgid(0,0) != 0)
    perror ("can't set new group");
  printf("GID: %i\nPID: %i\n",getgpid(),getpid()); 
  int cldcnt,i;
  //узнаем число потомков
  if(argc < 2) cldcnt=5;
  else cldcnt=atoi(argv[1]);
  pid_t* cld_pids=calloc(cldcnt,sizeof(pid_t));
  //создаем потомков
  for(i=0;i<cldcnt;++i)
    if((cld_pids[i] = fork())>0){
	printf("child %i creat\n",cld_pids[i]);
    //parent
    }else if(cld_pids[i] == 0){
	while(1)pause();
    //child
    }else {
      perror("can't fork");
    }
  while(1)pause();
  //  завершаем работу
  exit(0);
}
