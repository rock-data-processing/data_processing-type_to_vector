// \file  TestBuffer.cpp

#include <boost/test/auto_unit_test.hpp>

#include "MatrixBuffer.hpp"

using namespace type_to_vector;
using namespace Eigen;

BOOST_AUTO_TEST_CASE( test_matrix_buffer ) {


    MatrixBuffer bm(4,10);

    BOOST_CHECK ( bm.getPushCount() == 0 ); 
    BOOST_CHECK ( !bm.isFilled() );
    
    MatrixXd m1;

    bm.getMatrix(0,-1,m1);
    
    VectorXd v0 = VectorXd::Zero(4);
    VectorXd v1 = VectorXd::Ones(4);

    bm.push(v1);


    bm.getMatrix(0,0,m1);

    BOOST_CHECK ( m1.col(0) == v1 );

    bm.getMatrix(1,4,m1);

    BOOST_CHECK ( m1 == MatrixXd::Zero(4,4) );

    for ( int i=0; i<5; i++) {
        bm.push(v0);
        bm.getMatrix(1,5,m1);
        MatrixXd m2 = MatrixXd::Zero(4,5);
        m2.col(i) = v1;
        BOOST_CHECK(m1==m2);
    }

    bm.push(v0);
    bm.getMatrix(1,5,m1);
    BOOST_CHECK ( m1 == MatrixXd::Zero(4,5) );

    bm.getMatrix(6,-4,m1);
    BOOST_CHECK ( m1.col(0) == v1 );


    VectorXd v2 = VectorXd::Ones(5);
    BOOST_CHECK_THROW ( bm.push(v2), std::runtime_error );

    for ( int i=0; i<20; i++)
        bm.push(VectorXd::Random(4));

    bm.reset();

    BOOST_CHECK ( bm.getPushCount() == 0 );

    bm.getMatrix(0,-1,m1);

    BOOST_CHECK ( m1 == MatrixXd::Zero(bm.getVectorSize(),bm.getVectorCount()) );
    
    for ( int i=0; i<20; i++)
        bm.push(VectorXd::Random(4));

    BOOST_CHECK_THROW ( bm.getMatrix(6,3,m1), std::runtime_error);
    BOOST_CHECK_THROW ( bm.getMatrix(6,13,m1), std::runtime_error);
    
}
