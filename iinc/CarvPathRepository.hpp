#ifndef _CARVPATH_REPOSITORY_HPP_
#define _CARVPATH_REPOSITORY_HPP_
#include <sys/types.h>
#include <CarvPathImage.hpp>
class CarvPathRepository {
     public:
       virtual CarvPathImage *createImage(off_t fragmentsize)=0;
};
#endif
