#ifndef _CARVPATH_REPOSITORY_IMPL_HPP
#define _CARVPATH_REPOSITORY_IMPL_HPP
#include <string>
#include <vector>
#include <sys/file.h>
#include <sys/types.h>
#include <unistd.h>
#include <CarvPathRepository.hpp>
#include <CarvPathImageImpl.hpp>
  class CarvPathRepositoryImpl: public CarvPathRepository {
       int mFd;
       std::string mCarvFsRoot;
       bool pathExists(std::string thedir);
       bool existOrCreateDir(std::string thedir);
       bool existOrCreateFile(std::string rawdatapath);
       bool mountedOrMount(std::string rawdatapath,std::string mountpoint);
     public:
       CarvPathRepositoryImpl(std::string archiveroot);
       CarvPathImage *createImage(off_t fragmentsize);
  };
#endif
