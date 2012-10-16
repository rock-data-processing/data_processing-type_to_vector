rock_library(data_infrastructure
    SOURCES Dummy.cpp
    HEADERS Dummy.hpp)

rock_executable(data_infrastructure_bin Main.cpp
    DEPS data_infrastructure)

