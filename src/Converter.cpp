// \file  Converter.cpp

#include "Converter.hpp"

using namespace general_processing;

void ConvertToVector::visit (const VectorValueInfo& info) {
    if (info.content) visit(*(info.content));
    else mVector.push_back( info.castFun(mpValue->getData()+info.position));
}

void ConvertToVector::visit (const VectorToc& toc) {
    VectorTocVisitor::visit(toc);
}

ConvertToVector::ConvertToVector (const VectorToc& toc ) : mToc(toc) {}

std::vector<double> ConvertToVector::apply (const Typelib::Value& value, const std::string& slice) {
    mVector.clear();
    mConcreteToc.clear();
    mSlice = slice;
    mpValue = &value;
    VectorTocVisitor::visit(mToc);
    return mVector;
}

Eigen::VectorXd ConvertToVector::getEigenVector () {
    Eigen::VectorXd result;

    if (!mVector.empty()) 
        result = Eigen::Map<Eigen::VectorXd>(&(mVector[0]), mVector.size());

    return result;
}
