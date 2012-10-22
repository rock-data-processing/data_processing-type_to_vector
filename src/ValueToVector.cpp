// \file  ValueToVector.cpp

#include "ValueToVector.hpp"

using namespace general_processing;

VectorToc::VectorToc() : mrType(Typelib::NullType("nil")), mSlice("") {}

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


VectorTocMaker::VectorTocMaker() : mPosition(0), mContainerLoop(0) {}

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
    return VectorToc(); 
}
