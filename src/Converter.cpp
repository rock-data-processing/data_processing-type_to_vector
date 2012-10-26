// \file  Converter.cpp

#include <typelib/registry.hh>

#include "Converter.hpp"

using namespace general_processing;

void* ConvertToVector::getPosition(const VectorValueInfo& info) {

    void* ptr = mBaseStack.back() + info.position;

    if (!mContainersSizeStack.empty())
        ptr += mContainersSizeStack.back();

    return ptr;
}

void ConvertToVector::visit (const VectorValueInfo& info) {

    if (info.content.get()) {

        const Typelib::Container& t = 
            static_cast<const Typelib::Container&>(*mrRegistry.get(info.containerType));

        void* ptr = getPosition(info);

        unsigned int ecnt = t.getElementCount( ptr );
        unsigned int esize = t.getIndirection().getSize();

        std::vector<uint8_t>* vector_ptr = 
            reinterpret_cast<std::vector<uint8_t>*>( ptr );

        void* base = &(*vector_ptr)[0];

        mBaseStack.push_back(base);
        mContainersSizeStack.push_back(0);


        for ( int i=0; i<ecnt; i++) {
            VectorTocVisitor::visit(*(info.content));
            mContainersSizeStack.back() += esize;
        }

        mContainersSizeStack.pop_back();
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
    mContainersSizeStack.clear();

    VectorTocVisitor::visit(mToc);

    return mVector;

}

Eigen::VectorXd ConvertToVector::getEigenVector () {
    Eigen::VectorXd result;

    if (!mVector.empty()) 
        result = Eigen::Map<Eigen::VectorXd>(&(mVector[0]), mVector.size());

    return result;
}
