#if (NOT TYPELIB_PLUGIN_PATH)
#    set_source_files_properties(pluginmanager.cc PROPERTIES
#        COMPILE_FLAGS -DTYPELIB_PLUGIN_PATH="\\"${CMAKE_INSTALL_PREFIX}/lib/typelib\\"")
#else (NOT TYPELIB_PLUGIN_PATH)
#    set_source_files_properties(pluginmanager.cc PROPERTIES
#        COMPILE_FLAGS -DTYPELIB_PLUGIN_PATH="\\"${TYPELIB_PLUGIN_PATH}\\"")
#endif (NOT TYPELIB_PLUGIN_PATH)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_BINARY_DIR}/test)

configure_file(create_tlb.rb.in ${CMAKE_BINARY_DIR}/test/create_tlb.rb)
configure_file(TestSuite.hpp.in ${CMAKE_BINARY_DIR}/test/TestSuite.hpp)

set(TESTFILES   TestSuite.cpp 
                TestToc.cpp 
                TestTocMaker.cpp 
                TestConversion.cpp 
                TestSlice.cpp
)

rock_executable( general_processing_test
    SOURCES ${TESTFILES}
    DEPS general_processing
    DEPS_CMAKE Boost
    NOINSTALL)

add_custom_command( TARGET general_processing_test POST_BUILD
    COMMAND "ruby" ARGS "create_tlb.rb" ">" "TestTypes.tlb")
    
# DEPS_PKGCONFIG typelib eigen3
