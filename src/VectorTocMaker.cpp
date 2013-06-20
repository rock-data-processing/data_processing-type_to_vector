// \file  VectorTocMaker.cpp

#include <sstream>

#include "VectorTocMaker.hpp"

using namespace type_to_vector;

VectorTocMaker::VectorTocMaker() {}

void VectorTocMaker::push_valueinfo(Typelib::Type const& type) {

    VectorValueInfo info;

    info.placeDescription = utilmm::join(mPlaceStack,".");
    info.position = mPositionStack.back(); //position();
    info.castFun = getCastFunction(type);
    info.backCastFun = getBackCastFunction(type);
    info.containerType = "";
        
    mToc.push_back(info);
}

void VectorTocMaker::push_container(Typelib::Type const& type ,VectorTocPointer toc_ptr ) {
    
    VectorValueInfo info;

    info.placeDescription = utilmm::join(mPlaceStack,".");
    info.position = mPositionStack.back(); //position();
    info.castFun = 0;
    info.backCastFun = 0;
    info.content = toc_ptr;
    info.containerType = type.getName();

    mToc.push_back(info);
}

bool VectorTocMaker::visit_ (Typelib::NullType const& type) {

    return Typelib::TypeVisitor::visit_(type); 
}

bool VectorTocMaker::visit_ (Typelib::OpaqueType const& type) { 
    
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

    mPositionStack.push_back(mPositionStack.back());
    
    for (unsigned int i=0; i<type.getDimension(); i++ ) {

        mPlaceStack.push_back(boost::lexical_cast<std::string>(i));

        bool result = Typelib::TypeVisitor::visit_(type);
        mPositionStack.back() += type.getIndirection().getSize();

        mPlaceStack.pop_back();
        if (!result) return false;
    }

    mPositionStack.pop_back();

    return true;
}

bool VectorTocMaker::visit_ (Typelib::Container const& type) {
   
    mPlaceStack.push_back("*");

    VectorTocMaker vtm;
    VectorToc toc = vtm.apply(type.getIndirection());
    VectorTocPointer p_toc(new VectorToc(toc));

    push_container(type, p_toc); 

    mPlaceStack.pop_back();

    return true;
}

bool VectorTocMaker::visit_ (Typelib::Compound const& type, 
    Typelib::Field const& field) { 
   
    mPlaceStack.push_back(field.getName());
    mPositionStack.push_back(mPositionStack.back() + field.getOffset());

    bool result = Typelib::TypeVisitor::visit_(type, field);

    mPlaceStack.pop_back();
    mPositionStack.pop_back();

    return result;
}

VectorToc VectorTocMaker::apply (Typelib::Type const& type) {
    mToc.clear();
    mToc.mType = type.getName();
    mPlaceStack.clear();
    mPositionStack.clear();
    mPositionStack.push_back(0);
    Typelib::TypeVisitor::visit_(type); 
    return mToc; 
}
