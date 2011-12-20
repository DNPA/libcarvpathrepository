#include "CarvPathRepositoryImpl.hpp" 
#include <iostream>
#include <boost/tokenizer.hpp>
#include <strings.h>
#include <stdio.h>

       CarvPathRepositoryImpl::CarvPathRepositoryImpl(std::string archiveroot):mCarvFsRoot(""){
         std::string datadir=archiveroot + "/data";
         std::string rawdatapath=datadir + "/0.dd";
         std::string mountpoint=archiveroot + "/mnt";
         if (existOrCreateDir(datadir) == false) {
            throw std::string("Problem creating non existing archive dir:") + datadir ; 
         }
         if (existOrCreateDir(mountpoint) == false) {
            throw std::string("Problem creating non existing archive dir:") + mountpoint;
         }
         if (existOrCreateFile(rawdatapath) == false) {
            throw std::string("Problem creating non existing archive raw file:") + rawdatapath;
         }
         mCarvFsRoot = mountpoint + "/0/CarvFS";
         if (mountedOrMount(rawdatapath,mountpoint) == false ) {
            throw std::string("Problem mounting non mounted archive raw file:") + rawdatapath + " at " + mountpoint;
         }
         mFd=open(rawdatapath.c_str(),O_RDWR|O_LARGEFILE|O_NOATIME);
         if (mFd == -1) {
           throw std::string("Problem opening raw carvfs basefile for writting");
         }
       }
       bool CarvPathRepositoryImpl::pathExists(std::string thepath) {
          struct stat astat;
          if (lstat(thepath.c_str(),&astat)== -1) {
             return false;
          }
          return true;
       }
       std::string parentDir(std::string thedir) {
          boost::char_separator<char> sep("/");
          boost::tokenizer<boost::char_separator<char> > tokens(thedir,sep); 
          std::string prevtoken="";
          std::string parentdir="";
          for (boost::tokenizer<boost::char_separator<char> >::iterator tok_iter = tokens.begin();tok_iter != tokens.end(); ++tok_iter) {
              std::string curtoken=*tok_iter;
              parentdir = parentdir + "/" + prevtoken;
              prevtoken=curtoken;
          }
          if (parentdir == "/") { parentdir = "";}
          return parentdir;
       }
       bool CarvPathRepositoryImpl::existOrCreateDir(std::string thedir){
           if (thedir == "") {  return true;}
           std::string pdir=parentDir(thedir);
           if (existOrCreateDir(pdir) == false) { return false; }
           if (pathExists(thedir)) { return true;}
           if (mkdir(thedir.c_str(),0750) == -1) {
              return false;
           }            
           return true;
       }
       bool CarvPathRepositoryImpl::existOrCreateFile(std::string rawdatapath) {
          std::string pdir=parentDir(rawdatapath);
          if (existOrCreateDir(pdir) == false) { return false; }
          if (pathExists(rawdatapath)) { return true;}
          char emptyblock[4096];
          bzero(emptyblock,4096);
          int fd=open(rawdatapath.c_str(),O_RDWR|O_CREAT|O_EXCL);
          if (fd == -1) {
             return false;
          } 
          write(fd,(const void *)emptyblock, 4096);
          close(fd); 
          chmod(rawdatapath.c_str(), 0600);
          return true;
       }
       bool CarvPathRepositoryImpl::mountedOrMount(std::string rawdatapath,std::string mountpoint) {
          std::string bigpseudoimg=mountpoint + "/0/CarvFS.crv";
          if (pathExists(bigpseudoimg) == false) {
            std::string command=std::string("/usr/local/bin/carvfs ") + mountpoint + " raw 0 " + rawdatapath;
            int status = system(command.c_str());
            for (int x=0;x<4;x++) {
               if (pathExists(bigpseudoimg)) { return true;}
               if (x < 3) {
                  sleep(1);
               }
            }
            return false;
          }
          return true;
       }
       CarvPathImage *CarvPathRepositoryImpl::createImage(off_t fragmentsize) {
          if (flock(mFd,LOCK_EX)) {
            throw std::string("Problem locking raw carvfs basefile");
          } 
          off_t cursize=lseek64(mFd,0,SEEK_END);
          std::cout << "Raw size = "<< cursize << std::endl;
          off_t newsize=cursize+fragmentsize;
          lseek64(mFd,newsize-1,SEEK_SET);
          char lastbyte=0;
          write(mFd, &lastbyte,1); 
          flock(mFd,LOCK_UN);
          std::string fullimg=mCarvFsRoot + ".crv";
          pathExists(fullimg); 
          FILE *fil=fopen(fullimg.c_str(),"r");
          if (fil == 0) {
            throw std::string("Problem opening full image:") + fullimg;
          }
          fclose(fil);
          return new CarvPathImageImpl(mFd,cursize,fragmentsize,mCarvFsRoot);
       }
