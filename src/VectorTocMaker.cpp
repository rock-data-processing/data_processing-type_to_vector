// \file  VectorTocMaker.cpp

#include "VectorTocMaker.hpp"

using namespace general_processing;

VectorTocMaker::VectorTocMaker() : 
    mPosition(0), mContainerLoop(0) {}

bool VectorTocMaker::visit_ (Typelib::NullType const& type) {
    return Typelib::TypeVisitor::visit_(type); 
}
bool VectorTocMaker::visit_ (Typelib::OpaqueType const& type) { 
    return Typelib::TypeVisitor::visit_(type); 
}
bool VectorTocMaker::visit_ (Typelib::Numeric const& type) { 
    return Typelib::TypeVisitor::visit_(type); 
}
bool VectorTocMaker::visit_ (Typelib::Enum const& type) {
    return Typelib::TypeVisitor::visit_(type); 
}

bool VectorTocMaker::visit_ (Typelib::Pointer const& type) { 
    return Typelib::TypeVisitor::visit_(type); 
}
bool VectorTocMaker::visit_ (Typelib::Array const& type) { 
    return Typelib::TypeVisitor::visit_(type); 
}
bool VectorTocMaker::visit_ (Typelib::Container const& type) { 
    return Typelib::TypeVisitor::visit_(type); 
}

bool VectorTocMaker::visit_ (Typelib::Compound const& type) { 
    return Typelib::TypeVisitor::visit_(type); 
}
bool VectorTocMaker::visit_ (Typelib::Compound const& type, 
    Typelib::Field const& field) { 
    
    return Typelib::TypeVisitor::visit_(type); 
}

VectorToc VectorTocMaker::apply (Typelib::Type const& type) {
    return mToc; 
}
