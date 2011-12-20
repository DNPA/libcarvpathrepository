#include <string>
#include <vector>
#include <iostream>
#include <openssl/evp.h>
#include <boost/lexical_cast.hpp>
#include "CarvPathImageImpl.hpp"
       CarvPathImageImpl::CarvPathImageImpl(int fd,off_t startoffset,off_t fragsize,std::string cproot):mFd(fd),mStartOffset(startoffset),mSize(fragsize),mWriteOffset(0),mCarvPath(cproot),mSparse(0) ,mCtxMD5(),mCtxSHA1() {
         mCarvPath += "/";
         mCarvPath += boost::lexical_cast<std::string>(mStartOffset) + "+" + boost::lexical_cast<std::string>(mSize) + ".crv";
         EVP_DigestInit(&mCtxMD5, EVP_md5());
         EVP_DigestInit(&mCtxSHA1, EVP_sha1());
         mMD5="BOGUS";
         mSHA1="BOGUS";
       }

       void CarvPathImageImpl::writeblock(unsigned char *b512){
         if (mWriteOffset <= (mStartOffset+mSize-512)) {
           bool sparse=true;
           for (int index=0;(index<512) && sparse;index++) {
              if (b512[index]) {
                sparse=false;
              }
           }
           if (sparse == false) {
             lseek64(mFd,mStartOffset+mWriteOffset,SEEK_SET);
             if (write(mFd,b512,512) != 512) {
               throw std::string("Problem while writing 512 byte block to raw storage sparse file");
             }   
           } else {
             mSparse+=512;
           }
           EVP_DigestUpdate(&mCtxMD5, b512, 512);
           EVP_DigestUpdate(&mCtxSHA1,b512, 512);
           mWriteOffset+=512;
         } else {
           throw std::string("Attempting to write passed fragment end");
         }
       }

       void CarvPathImageImpl::setDone(){
         unsigned int md5l, sha1l;
         unsigned char md5Buffer[EVP_MAX_MD_SIZE];
         unsigned char sha1Buffer[EVP_MAX_MD_SIZE];
         EVP_DigestFinal(&mCtxMD5,  md5Buffer, &md5l);
         EVP_DigestFinal(&mCtxSHA1, sha1Buffer, &sha1l);
         char md5Bufferasc[EVP_MAX_MD_SIZE * 2 + 1];
         for (int i = 0; i < 16; i++) {
            sprintf((md5Bufferasc + (i * 2)), "%.2x", md5Buffer[i]);
         }
         mMD5=std::string(md5Bufferasc);
         char sha1Bufferasc[EVP_MAX_MD_SIZE * 2 + 1];
         for (int i = 0; i < 20; i++) {
           sprintf((sha1Bufferasc + (i * 2)), "%.2x", sha1Buffer[i]);
         }
         mSHA1=std::string(sha1Bufferasc);
       }

       std::string CarvPathImageImpl::getMd5(){return mMD5;}

       std::string CarvPathImageImpl::getSha1(){return mSHA1;}

       std::string CarvPathImageImpl::getCarvPath(){ 
            off_t prom=1000*(mSparse)/mSize;
            std::cout << "sparsecount = " << prom << "/1000" << std::endl;
            return mCarvPath;
       }
