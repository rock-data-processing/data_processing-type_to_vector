// \file  TestConversion.cpp

#include <boost/test/auto_unit_test.hpp>

#include <typelib/typemodel.hh>
#include <typelib/registry.hh>

#include "TestSuite.hpp"

#include "Converter.hpp"
#include "VectorTocMaker.hpp"


using namespace Typelib;
using namespace general_processing;

BOOST_AUTO_TEST_CASE( test_convert_scalar )
{
    Registry registry;
    import_types(registry);
    
    {
        double f = 1.4;
        Value v(&f, *registry.get("/double"));

        VectorToc toc = VectorTocMaker().apply(*registry.get("/double")); 

        ConvertToVector ctv(toc);

        std::vector<double> dbl_vec;
        dbl_vec.push_back(f);

        BOOST_CHECK( dbl_vec == ctv.apply(v) );
    }

    {
        int i = 3;
        Value v(&i, *registry.get("/int"));

        VectorToc toc = VectorTocMaker().apply(*registry.get("/int")); 

        ConvertToVector ctv(toc);

        std::vector<double> dbl_vec;
        dbl_vec.push_back(i);

        BOOST_CHECK( dbl_vec == ctv.apply(v) );
    }
    
    {
        char c = 3;
        Value v(&c, *registry.get("/char"));

        VectorToc toc = VectorTocMaker().apply(*registry.get("/char")); 

        ConvertToVector ctv(toc);

        std::vector<double> dbl_vec;
        dbl_vec.push_back(c);

        BOOST_CHECK( dbl_vec == ctv.apply(v) );
    }

}

BOOST_AUTO_TEST_CASE( test_convert_array )
{
    Registry registry;
    import_types(registry);


}

BOOST_AUTO_TEST_CASE( test_convert_struct )
{
    Registry registry;
    import_types(registry);


}

BOOST_AUTO_TEST_CASE( test_convert_container )
{
    Registry registry;
    import_types(registry);


}

BOOST_AUTO_TEST_CASE( test_convert_advanced )
{
    Registry registry;
    import_types(registry);

}

BOOST_AUTO_TEST_CASE( test_convert_concret_toc )
{
    Registry registry;
    import_types(registry);

}



