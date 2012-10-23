// \file  ValueToVector.cpp

#include <stdexcept>

#include "ValueToVector.hpp"

#define MAXDEPTH 100 // maximum recursive depth

using namespace general_processing;

VectorValueInfo::VectorValueInfo() : 
    placeDescription(""), position(0), castFun(0), content(0) {}

bool VectorValueInfo::operator==(const VectorValueInfo& other ) const {
    return placeDescription == other.placeDescription &&
        position == other.position &&
        castFun == other.castFun;
}

VectorToc::VectorToc() : mType(""), mSlice("") {}


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

VectorToc VectorToc::slice (const std::string& slice) {
    return *this;
}

bool VectorToc::operator== (const VectorToc& other) {
    static int depth=0;

    if ( mType != other.mType || mSlice != other.mSlice ||
            size() != other.size() ) return false;
 
    VectorToc::const_iterator it = begin(), oit = other.begin();

    for ( ; it < end(); it++, oit++ ) {
        if ( ! (*it == *oit) ) return false;
        if ( it->content > 0 && oit->content > 0 ) {
            depth++;
            if (depth > MAXDEPTH) 
                throw std::runtime_error("reached maximum recursive depth during ==");
            bool result = *(oit->content) == *(it->content);
            depth--;
            if (!result) {
                depth = 0;
                return false;
            }
        }
        else if ( it->content == 0 && oit->content != it->content ) return false;
    }
    return true;
}



VectorTocMaker::VectorTocMaker(Typelib::Type const& type) : 
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
    VectorTocMaker toc_maker(type); 
    return toc_maker.mToc; 
}
