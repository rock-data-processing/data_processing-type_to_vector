#if (NOT TYPELIB_PLUGIN_PATH)
#    set_source_files_properties(pluginmanager.cc PROPERTIES
#        COMPILE_FLAGS -DTYPELIB_PLUGIN_PATH="\\"${CMAKE_INSTALL_PREFIX}/lib/typelib\\"")
#else (NOT TYPELIB_PLUGIN_PATH)
#    set_source_files_properties(pluginmanager.cc PROPERTIES
#        COMPILE_FLAGS -DTYPELIB_PLUGIN_PATH="\\"${TYPELIB_PLUGIN_PATH}\\"")
#endif (NOT TYPELIB_PLUGIN_PATH)

include_directories(${CMAKE_SOURCE_DIR}/src)

rock_executable( general_processing_test
    SOURCES TestSuite.cpp TestToc.cpp
    DEPS general_processing
    DEPS_CMAKE Boost
    NOINSTALL)
    
# DEPS_PKGCONFIG typelib eigen3
