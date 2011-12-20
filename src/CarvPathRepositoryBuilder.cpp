#include <CarvPathRepositoryBuilder.hpp>
#include <CarvPathRepositoryImpl.hpp>

CarvPathRepository *CarvPathRepositoryBuilder::buildRepository(std::string archiveroot) {
    return new CarvPathRepositoryImpl(archiveroot);
} 
