#ifndef _CARVPATH_REPOSITORY_BUILDER_HPP_
#define _CARVPATH_REPOSITORY_BUILDER_HPP_
#include <CarvPathRepository.hpp>
class CarvPathRepositoryBuilder {
   public:
     static CarvPathRepository *buildRepository(std::string archiveroot);
};
#endif
