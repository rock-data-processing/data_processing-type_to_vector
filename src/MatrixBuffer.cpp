// \file  MatrixBuffer.cpp

#include <stdexcept>
#include <iostream>

#include "MatrixBuffer.hpp"


using namespace general_processing;

bool AbstractMatrixBuffer::push (const Eigen::VectorXd& v) {
    bool result = pushVector(v);
    pushCount++;
    return result;
}

void AbstractMatrixBuffer::reset() {

    pushCount = 0;
    resetBuffer();
}

bool MatrixBuffer::pushVector (const Eigen::VectorXd& v) {

    if ( v.rows() != mMatrix.rows() ) 
        throw std::runtime_error("Resize is not allowed for MatrixBuffer");
    
    if ( mInIdx > 0 ) mInIdx--;
    else mInIdx = vectorCount-1;

    mMatrix.col(mInIdx) = v; 

    return false;
}

const Eigen::MatrixXd& AbstractMatrixBuffer::getMatrix (int from, int to) {
    fillOutMatrix(from,to);
    return mOutMatrix;
}

void MatrixBuffer::resetBuffer() {

    mMatrix = Eigen::MatrixXd::Zero(vectorSize,vectorCount);
    mInIdx = vectorCount;
}
    
void MatrixBuffer::fillOutMatrix (int from, int to) {

    if ( from < 0 ) from += vectorCount;
    if ( to < 0 ) to += vectorCount;
    if ( from > to ) throw std::runtime_error("! from <= to !");

    int n = to - from + 1;

    if ( to >= vectorCount || n > vectorCount ) 
        throw std::runtime_error("Cannot give more vectors than in buffer");

    from += mInIdx;
    to += mInIdx;

    if (from > vectorCount-1) {

        mOutMatrix = mMatrix.block(0,from-vectorCount,vectorSize,n);

    } else if ( to > vectorCount-1 ) {

        mOutMatrix.resize(vectorSize,n);
        mOutMatrix << mMatrix.block(0,from,vectorSize,vectorCount-from),
            mMatrix.block(0,0,vectorSize,to-vectorCount+1);

    } else
        mOutMatrix = mMatrix.block(0,from,vectorSize,n); 
}
