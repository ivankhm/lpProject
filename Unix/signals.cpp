#include "/home/xendox/apue.h"
#include <iostream>
#include <cstdlib>
#include <csignal>

static void signalHandler( int signum )
{
    // terminate
   if (signum == SIGUSR1)
     std::cout << "Принят сигнал SIGUSR1" << std::endl;
   else
     std::cout << "Принят сигнал " << signum << std::endl;
}

int main ()
{
    // register signal SIGINT and signal handler  
    if (signal(SIGUSR1, signalHandler) == SIG_ERR)
      std::cout << "Невозможно перехватить сигнал SIGUSR1" << std::endl, exit(-1);  

    while(1){
       pause();
    }

    return 0;
}
