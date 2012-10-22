set(LIBSOURCES  Utilities.cpp
                ValueToVector.cpp )

set(LIBHEADERS  Utilities.hpp
                ValueToVector.hpp )

rock_library(general_processing
    SOURCES ${LIBSOURCES}
    HEADERS ${LIBHEADERS}
    DEPS_PKGCONFIG typelib eigen3)
