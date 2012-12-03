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
                TestTocSlices.cpp
                TestFlatConversion.cpp
                TestVectorBuilder.cpp
                TestBuffer.cpp
)

rock_executable( general_processing_test
    SOURCES ${TESTFILES}
    DEPS general_processing
    DEPS_CMAKE Boost
    NOINSTALL)

target_link_libraries(general_processing_test boost_system)

add_custom_command( TARGET general_processing_test POST_BUILD
    COMMAND "ruby" ARGS "create_tlb.rb" ">" "TestTypes.tlb") 

rock_executable( general_processing_timing
    SOURCES TestSuite.cpp Timing.cpp  
    DEPS general_processing
    DEPS_CMAKE Boost
    NOINSTALL)

target_link_libraries(general_processing_timing boost_system)
