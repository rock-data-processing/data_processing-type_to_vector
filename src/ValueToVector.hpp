/**
 * \file  ValueToVector.hpp
 *
 * \brief Converting typelib types to an vector (flatten it).
 *
 *      German Research Center for Artificial Intelligence\n
 *      Project: VirGo4
 *
 * \date 16.10.2012
 *
 * \author felix.rehrmann@dfki.de
 */

#ifndef GENERALPROCESSING_VALUETOVECTOR_HPP
#define GENERALPROCESSING_VALUETOVECTOR_HPP

#include <string>
#include <vector>

#include <typelib/typevisitor.hh>
#include <typelib/typemodel.hh>
#include <typelib/value.hh>

#include <Eigen/Core>

namespace general_processing {

/** Information to which place in a type a vector value belongs. 
 *
 * Only types that should be stored is numerics and containers.
 * Containers will not increase the position. They have to be
 * determined during conversion time. */
struct VectorValueInfo {
    std::string placeDescription; //!< Something like position[3], rotation.im[1] or ...
    unsigned int position; //!< The position in bytes in the memory of this value.
    Typelib::Type const& type; //!< Type of the value at the given position.
    unsigned int containerLoop; //!< For resolving containers with unknown size in advanced.
    std::string containerSlice; //!< For when one wants to take parts of a container.
};

/** Table of contents for the vector made of a type.
 *
 * This class holds the information where to find what in the type and how to put
 * it into a vector.
 */
class VectorToc : public std::vector<VectorValueInfo> {

    friend class TypeTocMaker;

    Typelib::Type const& mrType; //!< The type the toc is made for
    std::string mSlice; //!< Slice operation made and marks a concrete toc

public:
    VectorToc();

    std::vector<double> valueToVector (const Typelib::Value& value);
    Eigen::VectorXd valueToEigen (const Typelib::Value& value);

    Typelib::Value vectorToValue (const std::vector<double>& vector);
    Typelib::Value eigenToValue (const Eigen::VectorXd& vector);
    
    /** Gives the concret toc for a certain value. That is necessary when containers are 
     * part of the type. */
    VectorToc concreteToc (const Typelib::Value& value);

    /** Choose parts of a vector. */
    VectorToc slice (const std::string& slice);

    /** The name of the type the toc is for. */
    std::string typeName() const { return mrType.getName(); }

    /** The slice string used for this toc. */
    std::string sliceString() const { return mSlice; }
};

/** Finds out where in the vector to find which part of the \c Typelib::Type. */
class VectorTocMaker: public Typelib::TypeVisitor {
    
    VectorToc mToc;
    unsigned int mPosition;
    unsigned int mContainerLoop; 
    std::vector<std::string> mPlaceStack;

    VectorTocMaker();
    
protected:    
    virtual bool visit_ (Typelib::NullType const& type);
    virtual bool visit_ (Typelib::OpaqueType const& type);
    virtual bool visit_ (Typelib::Numeric const& type);
    virtual bool visit_ (Typelib::Enum const& type);

    virtual bool visit_ (Typelib::Pointer const& type);
    virtual bool visit_ (Typelib::Array const& type);
    virtual bool visit_ (Typelib::Container const& type);

    virtual bool visit_ (Typelib::Compound const& type);
    virtual bool visit_ (Typelib::Compound const& type,Typelib::Field const& field);

public:
    static VectorToc apply (Typelib::Type const& type);
};

}

#endif // GENERALPROCESSING_VALUETOVECTOR_HPP

