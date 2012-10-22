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

#ifndef LOBBASE_VALUETOVECTOR_HPP
#define LOBBASE_VALUETOVECTOR_HPP

#include <string>
#include <vector>

#include <typelib/typevisitor.hh>
#include <typelib/typemodel.hh>

namespace lob_base {

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
class VectorToc : protected std::vector<VectorValueInfo> {

    friend class TypeTocMaker;

    Typelib::Type const& type; //!< The type the toc is made for
    std::string slice; //!< Slice operation made and marks a concrete toc

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
};

/** Finds out where in the vector to find which part of the \c Typelib::Type. */
class VectorTocMaker: public Typelib::TypeVisitor {a
    
    VectorToc mToc;
    unsigned int mPosition;
    unsigned int mContainerLoop; 
    std::vector<std::string> mPlaceStack;

    VectorTocMaker();
    
protected:    
    virtual bool visit_ (NullType const& type);
    virtual bool visit_ (OpaqueType const& type);
    virtual bool visit_ (Numeric const& type);
    virtual bool visit_ (Enum const& type);

    virtual bool visit_ (Pointer const& type);
    virtual bool visit_ (Array const& type);
    virtual bool visit_ (Container const& type);

    virtual bool visit_ (Compound const& type);
    virtual bool visit_ (Compound const& type, Field const& field);

public:
    static VectorToc apply (Typelib::Type const& type);
};

}

#endif // LOBBASE_VALUETOVECTOR_HPP

