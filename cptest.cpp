#include <CarvPathRepositoryBuilder.hpp>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc,char **argv) {
  if (argc != 3) {
     std::cerr << "bad argument count" << std::endl;
     return 1;
  }
  std::string archive=argv[1];
  std::string input=argv[2];
  int inputfile=open(input.c_str(),O_RDONLY);
  if (inputfile == -1) {
     std::cerr << "problem opening input file." << std::endl;
     return 1;
  }  
  CarvPathRepository *repository=CarvPathRepositoryBuilder::buildRepository(archive);
  if (repository == 0) {
     std::cerr << "problem opening input file." << std::endl;
     return(1);
  } 
  off_t inputsize=lseek(inputfile, 0, SEEK_END);
  lseek(inputfile, 0, SEEK_SET);
  CarvPathImage *sparseimg=repository->createImage(inputsize);
  unsigned char buffer[512];
  while (read(inputfile,buffer,512)) {
         sparseimg->writeblock(buffer);
  } 
  sparseimg->setDone();
  std::cerr << "carvpath = " << sparseimg->getCarvPath() << " , md5=" << sparseimg->getMd5() << " sha=" << sparseimg->getSha1() << std::endl;
}
