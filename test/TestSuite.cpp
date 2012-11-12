// \file  TestSuit.cpp

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#ifndef BOOST_TEST_MODULE
    #define BOOST_TEST_MODULE "type2vector"
#endif

#define BOOST_AUTO_TEST_MAIN

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test.hpp>

#include <typelib/pluginmanager.hh>
#include <typelib/importer.hh>
#include <typelib/registry.hh>

#include "TestSuite.hpp"

using namespace Typelib;

void import_types(Registry& registry) {

    static const char* test_file = TEST_DATA_PATH("TestTypes.tlb");

    utilmm::config_set config;
    PluginManager::self manager;
    manager->load("tlb", test_file, config, registry);
}

