// \file  Converter.cpp

#include <typelib/registry.hh>

#include "Converter.hpp"

using namespace general_processing;

void ConvertToVector::visit (const VectorValueInfo& info) {

    if (info.content) visit(*(info.content));
    else mVector.push_back( info.castFun(mpValue->getData()+info.position));
}

void ConvertToVector::visit (const VectorToc& toc) {
    VectorTocVisitor::visit(toc);
}

ConvertToVector::ConvertToVector (const VectorToc& toc, const Typelib::Registry& registry) : 
    mToc(toc), mrRegistry(registry) {}

std::vector<double> ConvertToVector::apply (const Typelib::Value& value, const std::string& slice) {
    mVector.clear();
    mFlatToc.clear();
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
