#include "/home/xendox/apue.h"
#include <stdarg.h>
#include <fcntl.h> 
#include <sys/mman.h>
#include <iostream>

void error_mess(const char*mess, ...)
{
  va_list va;
  va_start(va,mess);
  for(const char*p=mess;*p;++p)
  {
    if(*p=='%' && *++p=='s')
      std::cout << va_arg(va,char*) << " ";
    else std::cout << *p;
  }
  std::cout << std::endl;
  va_end(va);
  exit(-1);
}
 
int main(int argc, char *argv[]) 
{ 
  int fdin, fdout; 
  void *src, *dst; 
  struct stat statbuf; 
  if (argc != 3) 
  	error_mess("Arguments mistake");
  if ( (fdin = open(argv[1], O_RDONLY)) < 0 ) 
    	error_mess("Невозможно открыть %s для чтения",argv[1]);
  if ( (fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0 )
  	error_mess("Невозможно создать %s для записи", argv[2]);
  if ( fstat(fdin, &statbuf) < 0 )
  	error_mess("fstat error");
  if ( lseek(fdout, statbuf.st_size - 1, SEEK_SET) == -1 )
	error_mess("Ошибка вызова функции lseek");
  if ( write(fdout, "", 1) != 1 )
  	error_mess("Ошибка вызова функции write");
  if ( (src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0)) == MAP_FAILED ) 
  	error_mess("Ошибка вызова функции mmap для входного файла");
  if ( (dst = mmap(0, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fdout, 0)) == MAP_FAILED ) 
  	error_mess("Ошибка вызова функции mmap для выходного файла");
  memcpy(dst, src, statbuf.st_size); 
  return 0; 
}
