// \file  Converter.cpp

#include <stdexcept>
#include <boost/lexical_cast.hpp>
#include <typelib/registry.hh>

#include "SliceMatcher.hpp"

#include "Converter.hpp"

using namespace general_processing;

void* FlatConverter::getPosition (const VectorValueInfo& info) {

    void* ptr = mpValue->getData() + info.position;

    return ptr;
}

void FlatConverter::push_element (const VectorValueInfo& info) {

    bool push_this = true;
    
    if ( mpMatcher && !mpMatcher->fitsASlice(info.placeDescription) ) 
        push_this = false;

    if (mCreatePlaceVector && push_this) 
        mPlaceVector.push_back(info.placeDescription);

    if (push_this) mVector.push_back( info.castFun(getPosition(info)) );

}

void FlatConverter::visit (const VectorValueInfo& info) {

    if (!info.content.get()) push_element(info);
}


void* ConvertToVector::getPosition (const VectorValueInfo& info) {

    void* ptr = mBaseStack.back() + info.position;

    if (!mContainersSizeStack.empty())
        ptr += mContainersSizeStack.back();

    return ptr;
}

FlatConverter::FlatConverter (const VectorToc& toc) : 
    mToc(toc), mpMatcher(0) {}

FlatConverter::~FlatConverter () {
    delete mpMatcher;
}

Eigen::VectorXd FlatConverter::getEigenVector () {

    Eigen::VectorXd result;

    if (!mVector.empty()) 
        result = Eigen::Map<Eigen::VectorXd>(&(mVector[0]), mVector.size());

    return result;
}

void FlatConverter::setSlice (const std::string& slice) {

    if ( mpMatcher ) {
        delete mpMatcher;
        mpMatcher = 0;
    }

    if (slice != "") mpMatcher = new SliceMatcher(slice);
}

std::vector<double> FlatConverter::apply (const Typelib::Value& value, bool create_place_vector ) {

    mVector.clear();
    mPlaceVector.clear();

    mCreatePlaceVector = create_place_vector;

    mpValue = &value;

    VectorTocVisitor::visit(mToc);

    return mVector;
}


void ConvertToVector::push_element (const VectorValueInfo& info) {

    bool push_this = true;
    
    if (mCreatePlaceVector || mpMatcher ) {

        if (info.placeDescription != "" ) mPlaceStack.push_back(info.placeDescription);

        std::string this_place = utilmm::join(mPlaceStack,".");

        if ( mpMatcher && !mpMatcher->fitsASlice(this_place) )
            push_this = false;

        if (mCreatePlaceVector && push_this) mPlaceVector.push_back(this_place);

        if (info.placeDescription != "" ) mPlaceStack.pop_back();
    }

    if (push_this) mVector.push_back( info.castFun(getPosition(info)) );

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
        
        if (mCreatePlaceVector || mpMatcher ) {
            mPlaceStack.push_back(info.placeDescription);
            istar = mPlaceStack.back().size()-1;
        }

        for ( int i=0; i<ecnt; i++) {

            if (mCreatePlaceVector || mpMatcher ) {

                mPlaceStack.back().replace(mPlaceStack.back().begin()+istar, 
                        mPlaceStack.back().end(), 
                        boost::lexical_cast<std::string,int>(i) );
            }

            VectorTocVisitor::visit(*(info.content));
            mContainersSizeStack.back() += esize;
        }

        mContainersSizeStack.pop_back();
        if (mCreatePlaceVector || mpMatcher ) mPlaceStack.pop_back();
        mBaseStack.pop_back();
    }
    else push_element(info);
}

ConvertToVector::ConvertToVector (const VectorToc& toc, const Typelib::Registry& registry) : 
    mToc(toc), mrRegistry(registry), mpMatcher(0) {}

ConvertToVector::~ConvertToVector () {
    if (mpMatcher) delete mpMatcher;
}

std::vector<double> ConvertToVector::apply (const Typelib::Value& value, bool create_place_vector ) {

    mVector.clear();
    mPlaceVector.clear();

    mCreatePlaceVector = create_place_vector;

    mpValue = &value;

    mBaseStack.clear();
    mBaseStack.push_back(mpValue->getData());
    mContainersSizeStack.clear();
    mPlaceStack.clear();

    VectorTocVisitor::visit(mToc);

    return mVector;

}
    
Eigen::VectorXd ConvertToVector::getEigenVector () {
    Eigen::VectorXd result;

    if (!mVector.empty()) 
        result = Eigen::Map<Eigen::VectorXd>(&(mVector[0]), mVector.size());

    return result;
}

void ConvertToVector::setSlice (const std::string& slice) {

    if ( mpMatcher ) {
        delete mpMatcher;
        mpMatcher = 0;
    }

    if (slice != "") mpMatcher = new SliceMatcher(slice);
}
