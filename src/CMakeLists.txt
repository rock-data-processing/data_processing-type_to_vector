set(LIBSOURCES  Utilities.cpp
                VectorToc.cpp
                VectorTocMaker.cpp
                Converter.cpp
                SliceMatcher.cpp
)

set(LIBHEADERS  Utilities.hpp
                Definitions.hpp
                VectorToc.hpp 
                VectorTocMaker.hpp
                NumericConverter.hpp
                Converter.cpp
                SliceMatcher.cpp
)

rock_library(general_processing
    SOURCES ${LIBSOURCES}
    HEADERS ${LIBHEADERS}
    DEPS_PKGCONFIG typelib eigen3)
