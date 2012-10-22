// \file  ValueToVector.cpp

#include "ValueToVector.hpp"

using namespace lob_base;

VectorToc() : type(Typelib::NullType), slice("") {}

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

VectorToc VectorToc::concretToc (const TypelibValue& value) {
    return *this;
}

VectorToc VectorToc::slice (const std::string& slice) {
    return *this;
}


VectorTocMaker::VectorTocMaker() : mPosition(0), mContainerLoop(0) {}

bool VectorTocMaker::visit_ (NullType const& type);
bool VectorTocMaker::visit_ (OpaqueType const& type);
bool VectorTocMaker::visit_ (Numeric const& type);
bool VectorTocMaker::visit_ (Enum const& type);

bool VectorTocMaker::visit_ (Pointer const& type);
bool VectorTocMaker::visit_ (Array const& type);
bool VectorTocMaker::visit_ (Container const& type);

bool VectorTocMaker::visit_ (Compound const& type);
bool VectorTocMaker::visit_ (Compound const& type, Field const& field);

VectorToc VectorTocMaker::apply (Typelib::Type const& type) {

}
