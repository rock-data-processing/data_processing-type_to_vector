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

    bool equals(const A& other) {
        return a == other.a &&
               b == other.b &&
               c == other.c &&
               d == other.d;
    }
};

struct B {
    char a;
    A b;

    bool equals(const B& other) {
        return a == other.a && b.equals(other.b);
    }
};

struct DoubleVector {
    int a;
    std::vector<double> dbl_vector;
};

struct VectorArray {
    std::vector<double> dbl_vector_array[3];

    bool equals(const VectorArray& other) {

        for ( int i = 0; i<3; i++) {
            if ( dbl_vector_array[i].size() != other.dbl_vector_array[i].size() ) 
                return false;
            for ( int j = 0; j<dbl_vector_array[i].size(); j++)
                if (dbl_vector_array[i][j] != other.dbl_vector_array[i][j]) 
                    return false;
        }
        return true;
    }
};

struct StructArray {
    std::vector<A> A_vector;
};

struct ContainerContainer {
    std::vector<DoubleVector> dbl_vv;

    bool equals(const ContainerContainer& other) {
        if (dbl_vv.size() != other.dbl_vv.size()) return false;
        for (int i = 0; i < dbl_vv.size(); i++) {
            if (dbl_vv[i].a != dbl_vv[i].a) return false;
            if (dbl_vv[i].dbl_vector.size() != other.dbl_vv[i].dbl_vector.size()) return false;
            for (int j = 0; j < dbl_vv[i].dbl_vector.size(); j++)
                if (dbl_vv[i].dbl_vector[j] != other.dbl_vv[i].dbl_vector[j]) return false;
        }
        return true;
    }
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

    bool equals(const TwoArrays& other) {
    
        for (int i = 0; i < 3; i++) if (a[i] != other.a[i]) return false;
        for (int i = 0; i < 5; i++) if (b[i] != other.b[i]) return false;
        return true;
    }
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

