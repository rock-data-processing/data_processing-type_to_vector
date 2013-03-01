// \file  SliceTiming.cpp

#include <cstdio>
#include <ctime>

#include <vector>

#include <type_to_vector/SliceMatcher.hpp>

using namespace type_to_vector;

struct Test {
    std::string name;
    StringVector slices;
    StringVector places;
    int n; // amount of runs
    int m; // amount of slicing run
    double result[2];

    template <typename S>
    double test() {
        clock_t start, end;
        bool r;

        StringVector::const_iterator sit, pit;

        start = clock();
        for ( int i = 0; i < n; i++ ) {
            sit = slices.begin();
            for ( ; sit != slices.end(); sit++ ) {
                S slicer(*sit);
                for (int j=0; j<m; j++ ) {
                    pit = places.begin();
                    for ( ; pit != places.end(); pit++ )
                        r = slicer.fitsASlice(*pit);
                }
            }
        }
        end = clock();

        printf("Last result is %d (output only to use the var).\n", r);
        
        return double(end-start)/CLOCKS_PER_SEC;
    }
    
};

typedef std::vector<Test> Tests;

int main() {

    Tests tests;
    
    {
        Test T;
        T.name = "one construct";
        T.n = 100000;
        T.m = 1;
        T.slices.push_back("first_place");
        tests.push_back(T);
    }
    {
        Test T;
        T.name = "one m'n'c";
        T.n = 100000;
        T.m = 1;
        T.slices.push_back("first_place");
        T.places.push_back("first_place");
        T.places.push_back("second_place");
        tests.push_back(T);
    }
    {
    {
        Test T;
        T.name = "one match";
        T.n = 1;
        T.m = 100000;
        T.slices.push_back("first_place");
        T.places.push_back("first_place");
        T.places.push_back("second_place");
        tests.push_back(T);
    }
        Test T;
        T.name = "start constr.";
        T.n = 10000;
        T.m = 1;
        T.slices.push_back("start.idx start.[1,4-6].a start.[1,4-6].b start.*.c zwei");
        tests.push_back(T);
    }
    {
        Test T;
        T.name = "start mnc";
        T.n = 10000;
        T.m = 1;;
        T.slices.push_back("start.idx start.[1,4-6].a start.[1,4-6].b start.*.c zwei");
        T.places.push_back("start.idx");
        T.places.push_back("start.5.a");
        T.places.push_back("start.8.c");
        T.places.push_back("start.idx.val");
        T.places.push_back("zwei.idx");
        T.places.push_back("drei");
        T.places.push_back("start.2.a");
        T.places.push_back("start.1.d");
        T.places.push_back("start.1");
        tests.push_back(T);
    }
    {
        Test T;
        T.name = "start match";
        T.n = 1;
        T.m = 10000;
        T.slices.push_back("start.idx start.[1,4-6].a start.[1,4-6].b start.*.c zwei");
        T.places.push_back("start.idx");
        T.places.push_back("start.5.a");
        T.places.push_back("start.8.c");
        T.places.push_back("start.idx.val");
        T.places.push_back("zwei.idx");
        T.places.push_back("drei");
        T.places.push_back("start.2.a");
        T.places.push_back("start.1.d");
        T.places.push_back("start.1");
        tests.push_back(T);
    }
    {
        Test T;
        T.name = "long constr";
        T.n = 10000;
        T.m = 1;
        T.slices.push_back("a.*.b.[1-7:3].*.c"); 
        tests.push_back(T);
    }
    {
        Test T;
        T.name = "a longer";
        T.n = 10000;
        T.m = 1;
        T.slices.push_back("a.*.b.[1-7:3].*.c"); 
        T.places.push_back("a.12.b.1.3.c");
        T.places.push_back("a.12.b.1.10.c");
        T.places.push_back("a.30.b.1.10.d");
        T.places.push_back("a.12.b.2.10.d");
        T.places.push_back("a.12.b.2.10.d");
        T.places.push_back("d.12.b.2.10.d");
        tests.push_back(T);
    }
    {
        Test T;
        T.name = "a longer";
        T.n = 1;
        T.m = 10000;
        T.slices.push_back("a.*.b.[1-7:3].*.c"); 
        T.places.push_back("a.12.b.1.3.c");
        T.places.push_back("a.12.b.1.10.c");
        T.places.push_back("a.30.b.1.10.d");
        T.places.push_back("a.12.b.2.10.d");
        T.places.push_back("a.12.b.2.10.d");
        T.places.push_back("d.12.b.2.10.d");
        tests.push_back(T);
    }
    
    Tests::iterator tit = tests.begin();

    for (int i=0; tit != tests.end(); tit++, i++ ) {
        printf("%d/%d\n",i+1,tests.size());
        tit->result[0] = tit->test<SliceMatcher>();
        tit->result[1] = tit->test<SliceTree>();
    }
  
    printf("Timing results in seconds.\n"); 
    printf("%15s | %8s | %8s | %10s\n","Name","Matcher","Tree","Runs"); 
    printf("-----------------+----------+----------+-----------\n");
    for (tit = tests.begin(); tit != tests.end(); tit++ ) {
        printf ("%15s | %8.3f | %8.3f | %7d/%7d\n", tit->name.c_str(), tit->result[0], tit->result[1], 
                tit->n, tit->m);
    }
}
