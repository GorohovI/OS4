#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#define CLDCNT 3
int main (int argс, char* argv[])
{ 
  int i;
  pid_t cld_pids[CLDCNT];
  //создаем потомков
  for(i=0;i<CLDCNT;++i)
    if((cld_pids[i] = fork())>0){
    //parent
    }else if(cld_pids[i] == 0){
    //child
    }else {
      perror("can't fork");
    }
  //  завершаем работу
  return 0;
}
