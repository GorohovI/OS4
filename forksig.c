#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
 
int main (int argс, char* argv[])
{ //создаем сою группу процессов
  if(setpgid(0,0) != 0)
    perror ("can't set new group");  
  int cldcnt,i;
  //узнаем число потомков
  if(argc < 2) cldcnt=5;
  else cldcnt=atoi(argv[1]);
  pid_t* cld_pids=new pid_t[cldcnt];
  //создаем потомков
  for(i=0;i<cldcnt;++i)
    if((cld_pids[i] = fork())>0){
    //parent
    }else if(cld_pids[i] == 0){
    //child
    }else {
      perror("can't fork");
    }
  //  завершаем работу
  delete[]pid_t;
  return 0;
}
