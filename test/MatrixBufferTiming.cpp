// \file  MatrixBufferTiming.cpp

#include <list>
#include <vector>
#include <iostream>
#include <string>

#include <Eigen/Core>

// Build: g++ MatrixBufferTiming.cpp -o matrix_buffer_timing -I/usr/include/eigen3

using namespace Eigen;
using std::list;

struct Buffer {
    int vectorCount;
    int vectorSize;

    Buffer(int vsize, int vcnt) : vectorCount(vcnt), vectorSize(vsize) {}
    
    virtual void push(const VectorXd& v) = 0;
    virtual MatrixXd getMatrixRawIndices(int from, int to) = 0;

    MatrixXd getMatrix(int from, int to=1) {
        if ( to < 0 ) to = vectorCount + to;
        assert(from <= to);
        from = vectorCount - from - 1;
        to = vectorCount - to - 1;
        return getMatrixRawIndices(to,from).reverse();
    }

    virtual std::string name() = 0;
    
    int getVectorCount() { return vectorCount; }
    int getVectorSize() { return vectorSize; }
};

struct MatrixBuffer : public Buffer {
    MatrixXd mat;
    int idx;

    MatrixBuffer(int n, int m) : mat(MatrixXd::Zero(n,m)), idx(-1), Buffer(n,m) {}

    void push(const VectorXd& v) {
        assert ( v.rows() == mat.rows() );
        int m = mat.cols();
        
        if (idx < m-1 ) idx++;
        else {
            mat.leftCols(m-1) = mat.rightCols(m-1);
        }
        mat.col(idx) = v;
    }

    MatrixXd getMatrixRawIndices( int from, int to) {
        
        return mat.block(0,from,mat.rows(),to-from+1);
    }

    std::string name() { return "MatrixBuffer"; }
};

struct QueueBuffer : public Buffer, protected list<VectorXd> {
    
    QueueBuffer(int n, int m) : Buffer(n,m) {}

    void push(const VectorXd& v) {
        push_back(v);
        if ( size() > vectorCount ) pop_front();
    }

    MatrixXd getMatrixRawIndices(int from, int to=-1) {
        MatrixXd mat = MatrixXd::Zero(vectorSize,to-from+1);
        const_iterator it = begin();
        for ( int i=0; i<from; i++,it++);
        for ( int i=0; it != end() && i < to-from+1; i++,it++) {
            mat.col(i) = *it;
        }
        return mat; 
    }

    virtual std::string name() { return "QueueBuffer"; }
};

struct QueueCacheBuffer : public QueueBuffer {

    MatrixBuffer mB;
    
    QueueCacheBuffer(int n, int m)  : mB(n,m), QueueBuffer(n,m) {}

    void push(const VectorXd& v) {
        QueueBuffer::push(v);
        const_iterator it = begin();
        for ( int i=0; it != end(); i++, it++)
            mB.mat.col(vectorCount-i-1) = *it;
    }

    MatrixXd getMatrixRawIndices(int from, int to) {
        return mB.getMatrix(from,to);
    }

    std::string name() { return "QueueCacheBuffer"; }
};

struct Matrix2mBuffer : public Buffer {
    MatrixXd mat;
    int mat_idx, idx;
    
    Matrix2mBuffer(int n, int m) : mat_idx(0), idx(-1), Buffer(n,m) {
        mat = MatrixXd::Zero(n,2*m);
    }
    
    void push(const VectorXd& v) {
        assert ( v.rows() == mat.rows() );
        
        if (idx < vectorCount-1) idx++;
        else {
            idx = 0;
        }
        mat.col(idx) = v;
        mat.col(idx+vectorCount) = v;
    }

    MatrixXd getMatrixRawIndices(int from, int to) {
        assert(idx >= 0 && idx < vectorCount);
        from += idx+1;
        to += idx+1;
        return mat.block(0,from,vectorSize,to-from+1);
        
    }

    std::string name() { return "Matrix2mBuffer"; }
};

struct CircleMatrixBuffer : public Buffer {
    MatrixXd mat;
    int idx;
    
    CircleMatrixBuffer(int n, int m) : idx(-1), Buffer(n,m) {
        mat = MatrixXd::Zero(n,m);
    }
    
    void push(const VectorXd& v) {
        assert ( v.rows() == mat.rows() );
        
        if (idx < vectorCount-1) idx++;
        else {
            idx = 0;
        }
        mat.col(idx) = v;
    }

    MatrixXd getMatrixRawIndices(int from, int to) {
        assert(idx >= 0 && idx < vectorCount);
        from += idx+1;
        to += idx+1;
        
        if (from > vectorCount-1) {
            return mat.block(0,from-vectorCount,vectorSize,to-from+1);
        } else if ( to > vectorCount-1 ) {
            return (MatrixXd(vectorSize,to-from+1) << mat.block(0,from,vectorSize,vectorCount-from),mat.block(0,0,vectorSize,to-vectorCount+1)).finished();

        } else
            return mat.block(0,from,vectorSize,to-from+1); 
    }

    std::string name() { return "CircleMatrixBuffer"; }
};

struct ColumnCircleMatrixBuffer : public Buffer {
    MatrixXd mat;
    int idx;
    
    ColumnCircleMatrixBuffer(int n, int m) : idx(-1), Buffer(n,m) {
        mat = MatrixXd::Zero(m,n);
    }
    
    void push(const VectorXd& v) {
        assert ( v.rows() == mat.cols() );
        
        if (idx < vectorCount-1) idx++;
        else {
            idx = 0;
        }
        mat.row(idx) = v;
    }

    MatrixXd getMatrixRawIndices(int from, int to) {
        assert(idx >= 0 && idx < vectorCount);
        from += idx+1;
        to += idx+1;
        
        if (from > vectorCount-1) {
            return mat.block(from-vectorCount,0,to-from+1,vectorSize).transpose();
        } else if ( to > vectorCount-1 ) {
            return (MatrixXd(vectorSize,to-from+1) << 
                    mat.block(from,0,vectorCount-from,vectorSize).transpose(),
                    mat.block(0,0,to-vectorCount+1,vectorSize).transpose()).finished();

        } else
            return mat.block(from,0,to-from+1,vectorSize).transpose(); 
    }

    std::string name() { return "ColumnCircleMatrixBuffer"; }
};




int main (void) {

    std::vector<Buffer*> bufs;

    bufs.push_back(new MatrixBuffer(3,5));
    bufs.push_back(new QueueBuffer(3,5));
    bufs.push_back(new Matrix2mBuffer(3,5));
    bufs.push_back(new CircleMatrixBuffer(3,5));
    bufs.push_back(new ColumnCircleMatrixBuffer(3,5));

    for ( int ib = 0; ib < bufs.size(); ib++ ) {
        std::cout << "Buffer " << ib << ": " << bufs.at(ib)->name() << std::endl;
        for ( int i = 0; i< 8; i++) {
            VectorXd v = VectorXd::Constant(3,i);
            bufs.at(ib)->push(v);
            std::cout << bufs.at(ib)->getMatrix(0,-1) << std::endl;
        }
    }
    
    for ( int ib = 0; ib < bufs.size(); ib++ ) {
        std::cout << "Buffer " << ib << " --- " << std::endl;
        for ( int i = 0; i< 3; i++) {
            std::cout << bufs.at(ib)->getMatrix(i,-2) << std::endl;
        }
    }

    for (int ib=0; ib < bufs.size(); ib++ ) {
        delete bufs.at(ib);
        bufs.at(ib) = 0;
    }
    
    bufs.clear();

    bufs.push_back(new MatrixBuffer(5,7));
    bufs.push_back(new MatrixBuffer(7,20));
    bufs.push_back(new QueueBuffer(5,7));
    bufs.push_back(new QueueBuffer(7,20));
    bufs.push_back(new Matrix2mBuffer(5,7));
    bufs.push_back(new Matrix2mBuffer(7,20));
    bufs.push_back(new CircleMatrixBuffer(5,7));
    bufs.push_back(new CircleMatrixBuffer(7,20));
    bufs.push_back(new ColumnCircleMatrixBuffer(5,7));
    bufs.push_back(new ColumnCircleMatrixBuffer(7,20));

    std::vector<double> sums;
    sums.resize(bufs.size(),0);

    std::cout << "Times for push:" << std::endl;
    
    int N = CLOCKS_PER_SEC;
    int start,end;

    for ( int ib=0; ib < bufs.size(); ib++) {
        Buffer* buf = bufs.at(ib);
        int n = buf->vectorSize;
        int m = buf->vectorCount;
        start = clock();
        VectorXd v = VectorXd::Zero(n);
        for (int i=0; i<N; i++) {
            v = VectorXd::Constant(n,i);
            buf->push(v); 
        }
        end = clock();
        std::cout << buf->name() << "(" << n << "," << m << ") needed " << 
            double(end-start)/1e6 << " us per push" << std::endl;
        sums.at(ib) += double(end-start)/1e6;
    }
    
    std::cout << "Times for getMatrix:" << std::endl;
    
    for ( int ib=0; ib < bufs.size(); ib++) {
        Buffer* buf = bufs.at(ib);
        int n = buf->vectorSize;
        int m = buf->vectorCount;
        start = clock();
        for (int i=0; i<N; i++) {
            MatrixXd mat = buf->getMatrix(m/2,-2); 
        }
        end = clock();
        std::cout << buf->name() << "(" << n << "," << m << ") needed " << 
            double(end-start)/1e6 << " us per get" << std::endl;
        sums.at(ib) += double(end-start)/1e6;
    }
    
    std::cout << "Times for getMatrix after push:" << std::endl;
    
    for ( int ib=0; ib < bufs.size(); ib++) {
        Buffer* buf = bufs.at(ib);
        int n = buf->vectorSize;
        int m = buf->vectorCount;
        VectorXd v = VectorXd::Zero(n);
        start = clock();
        for (int i=0; i<N; i++) {
            v = VectorXd::Constant(n,i+N);
            buf->push(v); 
            MatrixXd mat = buf->getMatrix(m/2,-2); 
        }
        end = clock();
        std::cout << buf->name() << "(" << n << "," << m << ") needed " << 
            double(end-start)/1e6 << " us per push/get" << std::endl;
        sums.at(ib) += double(end-start)/1e6;
    }
    
    std::cout << "Times in total:" << std::endl;
    
    for ( int ib=0; ib < bufs.size(); ib++) {
        Buffer* buf = bufs.at(ib);
        int n = buf->vectorSize;
        int m = buf->vectorCount;
        std::cout << buf->name() << "(" << n << "," << m << ") needed " << 
            sums.at(ib) << " us in total" << std::endl;
    }
    
    for (int ib=0; ib < bufs.size(); ib++ ) {
        delete bufs.at(ib);
        bufs.at(ib) = 0;
    }

    bufs.clear();

    return 0;
}
