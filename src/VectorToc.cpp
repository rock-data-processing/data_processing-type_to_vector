// \file  VectorToc.cpp

#include <stdexcept>

#include "VectorToc.hpp"

using namespace general_processing;


class VectorValueInfo::DeleteVisitor : public VectorTocVisitor {

    void visit(VectorValueInfo& info) {
        VectorTocVisitor::visit(info);
        if ( info.content ) {
            delete info.content;
            info.content = 0;
        }
    }

    void visit(VectorToc& toc) {
        VectorToc::iterator it = toc.begin();
        for ( ; it<toc.end(); it++)
            visit(*it);
    }

public:
    void apply(VectorToc& toc) { visit(toc); }
};  // class DeleteVisitor


VectorValueInfo::VectorValueInfo() : 
    placeDescription(""), position(0), castFun(0), content(0) {}

VectorValueInfo::~VectorValueInfo() {
    if (content) {
        DeleteVisitor dv;
        dv.apply(*content);
    }
}

bool VectorValueInfo::operator==(const VectorValueInfo& other ) const {
    return placeDescription == other.placeDescription &&
        position == other.position &&
        castFun == other.castFun;
}




VectorToc::VectorToc() : mType(""), mSlice(""), maxDepth(100) {}

std::vector<double> VectorToc::valueToVector (const Typelib::Value& value) {
    std::vector<double> v;
    return v;
}

Eigen::VectorXd VectorToc::valueToEigen (const Typelib::Value& value) {
    std::vector<double> v = valueToVector(value);
    Eigen::VectorXd eigen_vector = Eigen::Map<Eigen::VectorXd>(&(v[0]), v.size());
    return eigen_vector;
}

Typelib::Value VectorToc::vectorToValue (const std::vector<double>& vector) {
    return Typelib::Value();
}

Typelib::Value VectorToc::eigenToValue (const Eigen::VectorXd& vector) {
    std::vector<double> v;
    v.resize(vector.rows(), 0.0);
    Eigen::Map<Eigen::VectorXd> m(&(v[0]),vector.rows());
    m = vector;
    return vectorToValue(v);
}

VectorToc VectorToc::concreteToc (const Typelib::Value& value) {
    return *this;
}


void VectorTocVisitor::visit(VectorValueInfo const& info) {
    if (info.content) {
        mDepth++;
        if (mMaxDepth >= 0 && mDepth > mMaxDepth ) 
            throw std::runtime_error("reached maximum recursion depth in VectorTocVistor");
        visit(*(info.content));
        mDepth--;
    }
}

void VectorTocVisitor::visit(VectorToc const& toc) {
    VectorToc::const_iterator it = toc.begin();
    for ( ; it<toc.end(); it++)
        visit(*it);
}
