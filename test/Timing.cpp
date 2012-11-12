// \file  Timing.cpp

#include <ctime>
#include <cstdio>

#include <boost/test/auto_unit_test.hpp>

#include <typelib/typemodel.hh>
#include <typelib/registry.hh>

#define BOOST_TEST_MODULE "type2vector_timing"
#include "TestSuite.hpp"

#include "Converter.hpp"
#include "VectorTocMaker.hpp"
#include "Utilities.hpp"

#include "TestTypes.h"

using namespace Typelib;
using namespace general_processing;

BOOST_AUTO_TEST_CASE( test_timing )
{

    Registry registry;
    import_types(registry);

    int nConvert = 1000000;
    
    std::cout << "How long do conversions take?"  << std::endl;
    std::cout << std::endl;
    std::cout << "CLOCKS_PER_SEC = " << CLOCKS_PER_SEC << std::endl;
    std::cout << "no. of conversions: " << nConvert << std::endl;
    std::cout << std::endl;
    
    {
        double f = 1.4;
        Value v(&f, *registry.get("/double"));

        VectorToc toc = VectorTocMaker().apply(*registry.get("/double")); 


        ConvertToVector ctv(toc, registry);

        clock_t start =  clock();
        for ( int i = 0 ; i < nConvert; i++)
            std::vector<double> dbl_vec = ctv.apply(v);
        clock_t end =  clock();

        printf("converting a double takes %.3f us\n",
                double(end-start)/nConvert/CLOCKS_PER_SEC*1.0e6);
        
    }
    
    {
        int i = 3;
        Value v(&i, *registry.get("/int"));

        VectorToc toc = VectorTocMaker().apply(*registry.get("/int")); 


        ConvertToVector ctv(toc, registry);

        clock_t start =  clock();
        for ( int i = 0 ; i < nConvert; i++)
            std::vector<double> dbl_vec = ctv.apply(v);
        clock_t end =  clock();

        printf("converting an int takes %.3f us\n",
                double(end-start)/nConvert/CLOCKS_PER_SEC*1.0e6);
        
    }
    
    {
        char c = 3;
        Value v(&c, *registry.get("/char"));

        VectorToc toc = VectorTocMaker().apply(*registry.get("/char")); 


        ConvertToVector ctv(toc, registry);

        clock_t start =  clock();
        for ( int i = 0 ; i < nConvert; i++)
            std::vector<double> dbl_vec = ctv.apply(v);
        clock_t end =  clock();

        printf("converting a char takes %.3f us\n",
                double(end-start)/nConvert/CLOCKS_PER_SEC*1.0e6);
        
    }
    
    std::cout << std::endl;
    
    {
        Registry registry;
        import_types(registry);

        struct A a = { 100, -23, 'c', 12 };
        
        const Type& t = *registry.get("/A");

        Value v(&a, t);

        VectorToc toc = VectorTocMaker().apply(t);
    
        ConvertToVector ctv(toc, registry);
        
        clock_t start =  clock();
        for ( int i = 0 ; i < nConvert; i++)
            std::vector<double> dbl_vec = ctv.apply(v);
        clock_t end =  clock();
        
        printf("converting struct A takes %.3f us\n",
                double(end-start)/nConvert/CLOCKS_PER_SEC*1.0e6);
        
        start =  clock();
        for ( int i = 0 ; i < nConvert; i++)
            std::vector<double> dbl_vec = ctv.apply(v,true);
        end =  clock();
        
        printf("converting struct A with creating a place vector takes %.3f us\n",
                double(end-start)/nConvert/CLOCKS_PER_SEC*1.0e6);

        FlatConverter fc(toc);
        
        start =  clock();
        for ( int i = 0 ; i < nConvert; i++)
            std::vector<double> dbl_vec = fc.apply(v);
        end =  clock();
        
        printf("flat converting struct A takes %.3f us\n",
                double(end-start)/nConvert/CLOCKS_PER_SEC*1.0e6);

        start =  clock();
        for ( int i = 0 ; i < nConvert; i++)
            std::vector<double> dbl_vec = fc.apply(v,true);
        end =  clock();
        
        printf("flat converting struct A with creating a place vector takes %.3f us\n",
                double(end-start)/nConvert/CLOCKS_PER_SEC*1.0e6);

    } 
    
    std::cout << std::endl;
    
    {
        const Type& t = *registry.get("/StructArray");
        
        StructArray sa;
        struct A a = { 10, -23, 51, 112 };
        struct A b = { -12452134, 12, 33, -1 };
        sa.A_vector.push_back(a);
        sa.A_vector.push_back(b);
        sa.A_vector.push_back(b);

        Value v(&sa, t);
        
        VectorToc toc = VectorTocMaker().apply(t);

        ConvertToVector ctv(toc,registry);
    
        clock_t start =  clock();
        for ( int i = 0 ; i < nConvert; i++)
            std::vector<double> dbl_vec = ctv.apply(v);
        clock_t end =  clock();

        int s1 = sa.A_vector.size();
        double t1 = double(end-start)/nConvert/CLOCKS_PER_SEC*1.0e6; 
        
        printf("converting a vector of A of size %2i takes %.3f us\n", s1, t1);
        
        sa.A_vector.push_back(a);
        sa.A_vector.push_back(b);
        sa.A_vector.push_back(a);
        sa.A_vector.push_back(b);
        
        Value v2(&sa, t);
        
        start =  clock();
        for ( int i = 0 ; i < nConvert; i++)
            std::vector<double> dbl_vec = ctv.apply(v2);
        end =  clock();
        
        int s2 = sa.A_vector.size();
        double t2 = double(end-start)/nConvert/CLOCKS_PER_SEC*1.0e6; 
        
        printf("converting a vector of A of size %2i takes %.3f us\n", s2, t2);

        double slope = (t2-t1) / (s2-s1);
        double offset = t1 - slope * s1; 

        printf("const part %.3f us, slope %.3f us/sample\n",offset,slope); 
    }
    
    std::cout << std::endl;
    
    {
        const Type& t = *registry.get("/std/vector</int>");
    
        std::vector<int> int_vec;
        
        int_vec.resize(10,1);

        Value v(&int_vec, t);
        
        VectorToc toc = VectorTocMaker().apply(t);

        ConvertToVector ctv(toc,registry);
    
        clock_t start =  clock();
        for ( int i = 0 ; i < nConvert; i++)
            std::vector<double> dbl_vec = ctv.apply(v);
        clock_t end =  clock();

        int s1 = int_vec.size();
        double t1 = double(end-start)/nConvert/CLOCKS_PER_SEC*1.0e6; 
        
        printf("converting a vector of int of size %2i takes %.3f us\n", s1, t1);
        
        int_vec.resize(5,1); 
        
        //Value v2(&int_vec, t);
        
        start =  clock();
        for ( int i = 0 ; i < nConvert; i++)
            std::vector<double> dbl_vec = ctv.apply(v);
        end =  clock();
        
        int s2 = int_vec.size();
        double t2 = double(end-start)/nConvert/CLOCKS_PER_SEC*1.0e6; 
        
        printf("converting a vector of int of size %2i takes %.3f us\n", s2, t2);

        double slope = (t2-t1) / (s2-s1);
        double offset = t1 - slope * s1; 

        printf("const part %.3f us, slope %.3f us/sample\n",offset,slope); 
    }

}

