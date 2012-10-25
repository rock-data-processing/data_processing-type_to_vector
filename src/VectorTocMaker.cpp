// \file  VectorTocMaker.cpp

#include <sstream>

#include "VectorTocMaker.hpp"

using namespace general_processing;

VectorTocMaker::VectorTocMaker() {}

void VectorTocMaker::push_valueinfo(Typelib::Type const& type) {

    if (mDelta) mPosition += mDelta;
    else mPosition += mLastSize;

    VectorValueInfo info;

    info.placeDescription = utilmm::join(mPlaceStack,".");
    info.position = mPosition;
    info.castFun = getCastFunction(type);
    info.content = 0;
    mToc.push_back(info);

    mLastSize = type.getSize();
}

void VectorTocMaker::push_container(VectorToc* toc_ptr) {
    VectorValueInfo info;
    info.placeDescription = utilmm::join(mPlaceStack,".");
    info.position = mPosition;
    info.castFun = 0;
    info.content = toc_ptr;
    mToc.push_back(info);
}

bool VectorTocMaker::visit_ (Typelib::NullType const& type) {

    if (mDelta) mPosition += mDelta;
    else mPosition += mLastSize;

    mLastSize = type.getSize();

    return Typelib::TypeVisitor::visit_(type); 
}

bool VectorTocMaker::visit_ (Typelib::OpaqueType const& type) { 
    
    if (mDelta) mPosition += mDelta;
    else mPosition += mLastSize;

    mLastSize = type.getSize();

    return true; 
}

bool VectorTocMaker::visit_ (Typelib::Numeric const& type) {
    push_valueinfo(type);
    return true; 
}

bool VectorTocMaker::visit_ (Typelib::Enum const& type) {
    push_valueinfo(type);
    return true; 
}

bool VectorTocMaker::visit_ (Typelib::Pointer const& type) { 
    return Typelib::TypeVisitor::visit_(type); 
}

bool VectorTocMaker::visit_ (Typelib::Array const& type) {

    for (unsigned int i=0; i<type.getDimension(); i++ ) {
        std::stringstream ss;
        ss << i;
        mPlaceStack.push_back(ss.str());
        bool result = Typelib::TypeVisitor::visit_(type);
        mPlaceStack.pop_back();
        if (!result) return false;
    }
    return true;
}

bool VectorTocMaker::visit_ (Typelib::Container const& type) {
   
    mPlaceStack.push_back("*");

    VectorTocMaker vtm;
    VectorToc toc = vtm.apply(type.getIndirection());

    push_container(new VectorToc(toc)); 

    mPosition += vtm.getPosition();    
    
    mPlaceStack.pop_back();

    return true;
}

bool VectorTocMaker::visit_ (Typelib::Compound const& type) {

    // for when there is a compund within a compound
    if (mDelta) {
        mPosition += mDelta;
        mDelta = 0;
        mLastSize = 0;
    }

    mOffsetStack.push_back(0);

    bool result = Typelib::TypeVisitor::visit_(type);

    mOffsetStack.pop_back();

    return result;
}

bool VectorTocMaker::visit_ (Typelib::Compound const& type, 
    Typelib::Field const& field) { 
   
    mPlaceStack.push_back(field.getName());

    unsigned int offset = field.getOffset();

    if (offset) {
        mDelta = offset - mOffsetStack.back();
        mOffsetStack.back() = offset;
    } else
        mDelta = 0;

    bool result = Typelib::TypeVisitor::visit_(type, field);

    mPlaceStack.pop_back();
    mDelta = 0;

    return result;
}

VectorToc VectorTocMaker::apply (Typelib::Type const& type) {
    mToc.clear();
    mToc.mType = type.getName();
    mPlaceStack.clear();
    mPosition = 0;
    mOffsetStack.clear();
    mDelta = 0;
    mLastSize = 0;
    Typelib::TypeVisitor::visit_(type); 
    return mToc; 
}
