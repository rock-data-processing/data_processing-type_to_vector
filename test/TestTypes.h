/**
 * \file  TestTypes.h
 *
 * \brief Types to test the converter with.
 *
 *      German Research Center for Artificial Intelligence\n
 *      Project: VirGo4
 *
 * \date 22.10.2012
 *
 * \author felix.rehrmann@dfki.de
 */

#ifndef GENERALPROCESSING_TESTTYPES_H
#define GENERALPROCESSING_TESTTYPES_H

#include <vector>
#include <string>

enum Count { One, Two, Three };

typedef std::vector<int> IntVec;

struct A {
    long long a;
    int b;
    char c;
    short d;
};

struct B {
    char a;
    A b;
};

struct DoubleVector {
    int a;
    std::vector<double> dbl_vector;
};

struct VectorArray {
    std::vector<double> dbl_vector_array[3];
};

struct StructArray {
    std::vector<A> A_vector;
};

struct ContainerContainer {
    std::vector<DoubleVector> dbl_vv;
};

struct ForFlatSliceTest {
    double a;
    std::vector<double> vec;
    std::string str;
    int b;
};

struct TwoArrays {
    int a[3];
    int b[5];
};

struct DocA {
    double a[3];
    int b;
    char c;
};
struct DocB {
    int idx;
    DocA data[5];
};


#endif // GENERALPROCESSING_TESTTYPES_H

