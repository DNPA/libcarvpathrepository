#ifndef _CARVPATH_IMAGE_HPP_
#define _CARVPATH_IMAGE_HPP_
#include <string>
class CarvPathImage {
    public:
       virtual void writeblock(unsigned char *b512)=0;
       virtual void setDone()=0;
       virtual std::string getMd5()=0;
       virtual std::string getSha1()=0;
       virtual std::string getCarvPath()=0;
};
#endif
