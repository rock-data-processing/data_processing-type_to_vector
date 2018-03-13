#if (NOT TYPELIB_PLUGIN_PATH)
#    set_source_files_properties(pluginmanager.cc PROPERTIES
#        COMPILE_FLAGS -DTYPELIB_PLUGIN_PATH="\\"${CMAKE_INSTALL_PREFIX}/lib/typelib\\"")
#else (NOT TYPELIB_PLUGIN_PATH)
#    set_source_files_properties(pluginmanager.cc PROPERTIES
#        COMPILE_FLAGS -DTYPELIB_PLUGIN_PATH="\\"${TYPELIB_PLUGIN_PATH}\\"")
#endif (NOT TYPELIB_PLUGIN_PATH)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_BINARY_DIR}/test)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

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
                TestBackConversion.cpp
)

rock_executable( type_to_vector_test
    SOURCES ${TESTFILES}
    DEPS type_to_vector
    DEPS_CMAKE Boost
    NOINSTALL)
target_link_libraries(type_to_vector_test ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_custom_command( TARGET type_to_vector_test POST_BUILD
    COMMAND "ruby" ARGS "create_tlb.rb" ">" "TestTypes.tlb") 

rock_executable( type_to_vector_timing
    SOURCES TestSuite.cpp Timing.cpp
    DEPS type_to_vector
    NOINSTALL)
target_link_libraries(type_to_vector_timing ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

rock_executable( slice_timing_cmp
    SOURCES SliceTiming.cpp
    DEPS type_to_vector
    NOINSTALL)
target_link_libraries(slice_timing_cmp ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

