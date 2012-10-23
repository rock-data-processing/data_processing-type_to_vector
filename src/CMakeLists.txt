set(LIBSOURCES  Utilities.cpp
                VectorToc.hpp
                VectorTocMaker.hpp )

set(LIBHEADERS  Utilities.hpp
                VectorToc.hpp 
                VectorTocMaker.hpp
                NumerivConverter.hpp )

rock_library(general_processing
    SOURCES ${LIBSOURCES}
    HEADERS ${LIBHEADERS}
    DEPS_PKGCONFIG typelib eigen3)
