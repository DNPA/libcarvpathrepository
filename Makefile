SHELL=/bin/sh
INSTALL=install
CPP=g++
LIBTOOL=libtool

OBJFILESLIB=build/CarvPathImageImpl.lo  build/CarvPathRepositoryBuilder.lo  build/CarvPathRepositoryImpl.lo

libcarvpathrepository: $(OBJFILESLIB)
	$(LIBTOOL) --tag=CPP --mode=link $(CPP) ${LIBS} -rpath /usr/local/lib/ -release 2.3.0 -version-info 2:0:0  -o build/libcarvpathrepository.la $(OBJFILESLIB)

build/%.lo: src/%.cpp
	$(LIBTOOL) --tag=CPP --mode=compile  $(CPP) -ggdb -c -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_REENTRANT -Iiinc -Iinc -o $@ $<

install: build/libcarvpathrepository.la
	$(LIBTOOL) --mode=install cp build/libcarvpathrepository.la /usr/local/lib/
	$(LIBTOOL) --finish /usr/local/lib/
	cp iinc/*.hpp /usr/local/include/

clean:
	rm -rf build
	mkdir build
