/**
 * \file  MatrixBuffer.hpp
 *
 * \brief Buffers Vectors 
 *
 *      German Research Center for Artificial Intelligence\n
 *      Project:
 *
 * \date 30.11.2012
 *
 * \author felix.rehrmann@dfki.de
 */

#ifndef GENERALPROCESSING_MATRIXBUFFER_HPP
#define GENERALPROCESSING_MATRIXBUFFER_HPP

#include <Eigen/Core>;

namespace general_processing {

/** A buffer to hold a time series of vectors and provides a block of them as matrix. */
class AbstractMatrixBuffer {

protected:
    int vectorSize, vectorCount;
    int pushCount;

    /** Push a vector to the buffer. 
     *
     * returns true if resize has been necessary.*/
    virtual bool pushVector (const Eigen::VectorXd& v) = 0;
    
    /** Reset the buffer. */
    virtual void resetBuffer() = 0;

public:

    AbstractMatrixBuffer (int vector_size, int vector_count) : vectorSize(vector_size),
        vectorCount(vector_count), pushCount(0) {}
    
    /** Push a vector to the buffer. 
     *
     * returns true if resize has been necessary.*/
    bool push (const Eigen::VectorXd& v);

    /** Get a matrix from the buffer. Must provide matrix object.
     *
     * Get the matrix that from \p from to \p to, where 0 is the most recent pushed 
     * vector and \c vectorCount-1 is the oldes vector. 
     * Negative values count from the end: \p to = -1 is \p to = \p vectorCount-1.
     */
    virtual void getMatrix (int from, int to, Eigen::MatrixXd& matrix) = 0;

    /** Reset the buffer. */
    void reset();

    /** Get number of vectors pushed to the buffer. */
    int getPushCount() { return pushCount;}

    /** Is true if the buffer is filled once. */
    bool isFilled() { return pushCount >= vectorCount; }

    int getVectorCount() { return vectorCount; }
    int getVectorSize() { return vectorSize; }
};

class MatrixBuffer : public AbstractMatrixBuffer {

protected:
    Eigen::MatrixXd mMatrix;
    int mInIdx;
    
    virtual bool pushVector(const Eigen::VectorXd& v);
    
    virtual void resetBuffer();  

public:
    MatrixBuffer (int vector_size, int vector_count) :  
        AbstractMatrixBuffer(vector_size, vector_count), mInIdx(vectorCount),
        mMatrix(Eigen::MatrixXd::Zero(vector_size, vector_count))  {}

    virtual void getMatrix (int from, int to, Eigen::MatrixXd& matrix);
};

}
#endif // GENERALPROCESSING_MATRIXBUFFER_HPP
