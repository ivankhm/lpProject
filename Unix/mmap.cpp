#include "apue.h"
#include <fcntl.h> 
#include <sys/mman.h>
#include <iostream>
 
int main(int argc, char *argv[]) 
{ 
  int fdin, fdout; 
  void *src, *dst; 
  struct stat statbuf; 
  if (argc != 3) 
  {
  	std::cout << "Arguments mistake" << std::endl;
	exit(-1);
  }
  if ( (fdin = open(argv[1], O_RDONLY)) < 0 ) 
  {
    	std::cout << "невозможно открыть " << argv[1] << " для чтения" << std::endl;
	exit(-1);
  } 
  if ( (fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0 )
  { 
    	std::cout << "невозможно создать" << argv[2] << " для записи" << std::endl;
	exit(-1);
  } 
  if ( fstat(fdin, &statbuf) < 0 )
  { 
    	std::cout << "fstat error" << std::endl;
	exit(-1);
  } 
  if ( lseek(fdout, statbuf.st_size - 1, SEEK_SET) == -1 ) 
  {
	std::cout << "ошибка вызова функции lseek" << std::endl;
	exit(-1);
  } 
  if ( write(fdout, "", 1) != 1 )
  { 
  	std::cout << "ошибка вызова функции write" << std::endl;
	exit(-1);
  } 
  if ( (src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0)) == MAP_FAILED ) 
  {
  	std::cout << "ошибка вызова функции mmap для входного файла" << std::endl;
	exit(-1);
  } 
  if ( (dst = mmap(0, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fdout, 0)) == MAP_FAILED ) 
  {
  	std::cout << "ошибка вызова функции mmap для выходного файла" << std::endl;
	exit(-1);
  } 
  memcpy(dst, src, statbuf.st_size); 
  return 0; 
}
