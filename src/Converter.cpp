// \file  Converter.cpp

#include <typelib/registry.hh>

#include "Converter.hpp"

using namespace general_processing;

void* ConvertToVector::getPosition(const VectorValueInfo& info) {
    return mBaseStack.back() + info.position + mContainersSize;
}

void ConvertToVector::visit (const VectorValueInfo& info) {

    if (info.content) {

        const Typelib::Container& t = 
            static_cast<const Typelib::Container&>(*mrRegistry.get(info.containerType));

        unsigned int ecnt = t.getElementCount( getPosition(info) );
        unsigned int esize = t.getIndirection().getSize();
        std::vector<uint8_t>* vector_ptr = 
            reinterpret_cast<std::vector<uint8_t>*>(getPosition(info));
        void* base = &(*vector_ptr)[0];
        mBaseStack.push_back(base);

        for ( int i=0; i<ecnt; i++) {
            visit(*(info.content));
            mContainersSize += esize;
        }

        mContainersSize = 0;
        mBaseStack.pop_back();
    }
    else mVector.push_back( info.castFun(getPosition(info)) );
}

void ConvertToVector::visit (const VectorToc& toc) {
    VectorTocVisitor::visit(toc);
}

ConvertToVector::ConvertToVector (const VectorToc& toc, const Typelib::Registry& registry) : 
    mToc(toc), mrRegistry(registry) {}

std::vector<double> ConvertToVector::apply (const Typelib::Value& value, 
        const std::string& slice) {

    mVector.clear();
    mFlatToc.clear();
    mSlice = slice;
    mpValue = &value;
    mBaseStack.clear();
    mBaseStack.push_back(mpValue->getData());
    mContainersSize = 0;

    VectorTocVisitor::visit(mToc);

    return mVector;

}

Eigen::VectorXd ConvertToVector::getEigenVector () {
    Eigen::VectorXd result;

    if (!mVector.empty()) 
        result = Eigen::Map<Eigen::VectorXd>(&(mVector[0]), mVector.size());

    return result;
}
