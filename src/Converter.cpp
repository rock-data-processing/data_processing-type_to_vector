// \file  Converter.cpp

#include <stdexcept>
#include <boost/lexical_cast.hpp>
#include <typelib/registry.hh>

#include "Converter.hpp"

using namespace general_processing;

void* ConvertToVector::getPosition (const VectorValueInfo& info) {

    void* ptr = mBaseStack.back() + info.position;

    if (!mContainersSizeStack.empty())
        ptr += mContainersSizeStack.back();

    return ptr;
}

void ConvertToVector::push_element (const VectorValueInfo& info) {

    mVector.push_back( info.castFun(getPosition(info)) );

    if (mCreatePlaceVector) {

        if (info.placeDescription != "" ) mPlaceStack.push_back(info.placeDescription);

        mPlaceVector.push_back(utilmm::join(mPlaceStack,"."));

        if (info.placeDescription != "" ) mPlaceStack.pop_back();
    }
}

void ConvertToVector::visit (const VectorValueInfo& info) {

    if (info.content.get()) {

        const Typelib::Container& t = 
            static_cast<const Typelib::Container&>(*mrRegistry.get(info.containerType));

        void* ptr = getPosition(info);

        unsigned int ecnt = t.getElementCount( ptr );
        if ( ecnt == 0 ) return;

        unsigned int esize = t.getIndirection().getSize();

        std::vector<uint8_t>* vector_ptr = 
            reinterpret_cast<std::vector<uint8_t>*>( ptr );

        void* base = &(*vector_ptr)[0];

        mBaseStack.push_back(base);
        mContainersSizeStack.push_back(0);

        int istar;
        
        if (mCreatePlaceVector) {
            mPlaceStack.push_back(info.placeDescription);
            istar = mPlaceStack.back().size()-1;
        }

        for ( int i=0; i<ecnt; i++) {

            if (mCreatePlaceVector) {

                mPlaceStack.back().replace(mPlaceStack.back().begin()+istar, 
                        mPlaceStack.back().end(), 
                        boost::lexical_cast<std::string,int>(i) );
            }

            VectorTocVisitor::visit(*(info.content));
            mContainersSizeStack.back() += esize;
        }

        mContainersSizeStack.pop_back();
        if (mCreatePlaceVector) mPlaceStack.pop_back();
        mBaseStack.pop_back();
    }
    else push_element(info);
}

ConvertToVector::ConvertToVector (const VectorToc& toc, const Typelib::Registry& registry) : 
    mToc(toc), mrRegistry(registry) {}

std::vector<double> ConvertToVector::apply (const Typelib::Value& value, 
        const std::string& slice, bool create_place_vector ) {

    mVector.clear();
    mPlaceVector.clear();

    mSlice = slice;

    if (!create_place_vector) mCreatePlaceVector = slice != "";
    else mCreatePlaceVector = true;

    mpValue = &value;

    mBaseStack.clear();
    mBaseStack.push_back(mpValue->getData());
    mContainersSizeStack.clear();
    mPlaceStack.clear();

    VectorTocVisitor::visit(mToc);

    return mVector;

}
    
std::vector<double> ConvertToVector::apply ( const Typelib::Value& value, 
    bool create_place_vector) {

    return apply(value,"",create_place_vector);
}

std::vector<double> ConvertToVector::apply ( const Typelib::Value& value ) {

    return apply(value,"",true);
}

Eigen::VectorXd ConvertToVector::getEigenVector () {
    Eigen::VectorXd result;

    if (!mVector.empty()) 
        result = Eigen::Map<Eigen::VectorXd>(&(mVector[0]), mVector.size());

    return result;
}
