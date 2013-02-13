set(LIBSOURCES  Utilities.cpp
                VectorToc.cpp
                VectorTocMaker.cpp
                Converter.cpp
                SliceMatcher.cpp
                VectorBuilder.cpp
                MatrixBuffer.cpp
)

set(LIBHEADERS  Utilities.hpp
                Definitions.hpp
                VectorToc.hpp 
                VectorTocMaker.hpp
                NumericConverter.hpp
                Converter.hpp
                SliceMatcher.hpp
                VectorBuilder.hpp
                MatrixBuffer.hpp
)

rock_library(type_to_vector
    SOURCES ${LIBSOURCES}
    HEADERS ${LIBHEADERS}
    DEPS_PKGCONFIG typelib eigen3)
