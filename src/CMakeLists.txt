set(LIBSOURCES  Utilities.cpp
                VectorToc.cpp
                VectorTocMaker.cpp
)

set(LIBHEADERS  Utilities.hpp
                VectorToc.hpp 
                VectorTocMaker.hpp
                NumericConverter.hpp
)

rock_library(general_processing
    SOURCES ${LIBSOURCES}
    HEADERS ${LIBHEADERS}
    DEPS_PKGCONFIG typelib eigen3)
