/**
 * \file  VectorTocMaker.hpp
 *
 * \brief To generate the toc of a vector.
 *
 *      German Research Center for Artificial Intelligence\n
 *      Project: VirGo4
 *
 * \date 23.10.2012
 *
 * \author felix.rehrmann@dfki.de
 */

#ifndef GENERALPROCESSING_VECTORTOCMAKER_HPP
#define GENERALPROCESSING_VECTORTOCMAKER_HPP

#include <typelib/typevisitor.hh>
#include <typelib/typemodel.hh>
#include "VectorToc.hpp"

namespace general_processing {

/** Finds out where in the vector to find which part of the \c Typelib::Type. */
class VectorTocMaker: public Typelib::TypeVisitor {
    
    VectorToc mToc;
    unsigned int mPosition;
    unsigned int mContainerLoop; 
    std::vector<std::string> mPlaceStack;

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
    VectorTocMaker();
    VectorToc apply (Typelib::Type const& type);
};

} // namespace general_processing

#endif // GENERALPROCESSING_VECTORTOCMAKER_HPP

