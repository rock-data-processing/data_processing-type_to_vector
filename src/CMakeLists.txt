set(LIBSOURCES  Utilities.cpp
                VectorToc.cpp
                VectorTocMaker.cpp
                Converter.cpp
                SliceMatcher.cpp
                VectorBuilder.cpp
)

set(LIBHEADERS  Utilities.hpp
                Definitions.hpp
                VectorToc.hpp 
                VectorTocMaker.hpp
                NumericConverter.hpp
                Converter.hpp
                SliceMatcher.hpp
                VectorBuilder.hpp
)

rock_library(general_processing
    SOURCES ${LIBSOURCES}
    HEADERS ${LIBHEADERS}
    DEPS_PKGCONFIG typelib eigen3)
