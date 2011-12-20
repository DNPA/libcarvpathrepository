#ifndef _CARVPATH_IMAGE_IMPL_HPP
#define _CARVPATH_IMAGE_IMPL_HPP
#include <string>
#include <vector>
#include <iostream>
#include <openssl/evp.h>
#include <CarvPathImage.hpp>


  class CarvPathImageImpl: public CarvPathImage { 
       int mFd;
       off_t mStartOffset;
       off_t mSize;
       off_t mWriteOffset;
       std::string mCarvPath;
       off_t mSparse;
       EVP_MD_CTX mCtxMD5;
       EVP_MD_CTX mCtxSHA1;
       std::string mMD5;
       std::string mSHA1;

    public:

       CarvPathImageImpl(int fd,off_t startoffset,off_t fragsize,std::string cproot);

       void writeblock(unsigned char *b512);

       void setDone();

       std::string getMd5();

       std::string getSha1();

       std::string getCarvPath();
  };
#endif
