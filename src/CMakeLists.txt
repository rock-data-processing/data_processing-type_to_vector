set(LIBSOURCES  Utilities.cpp
                VectorToc.cpp
                VectorTocMaker.cpp
                Converter.cpp
                SliceMatcher.cpp
                VectorBuilder.cpp
                MatrixBuffer.cpp
                BackConverter.cpp
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
                BackConverter.hpp
)

rock_library(type_to_vector
    SOURCES ${LIBSOURCES}
    HEADERS ${LIBHEADERS}
    DEPS_PKGCONFIG typelib eigen3
    DEPS_CMAKE Boost)

target_link_libraries(type_to_vector boost_system)
