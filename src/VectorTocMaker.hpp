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

#include <vector>

#include <typelib/typevisitor.hh>
#include <typelib/typemodel.hh>
#include <utilmm/stringtools.hh>

#include "VectorToc.hpp"

namespace general_processing {

/** Finds out where in the vector to find which part of the \c Typelib::Type. */
class VectorTocMaker: public Typelib::TypeVisitor {
    
    VectorToc mToc;
    utilmm::stringlist mPlaceStack;
    unsigned int mPosition; //!< Position in the data.
    unsigned int mLastSize;
    std::vector<unsigned int> mOffsetStack; //!< Last offset in case of a field
    unsigned int mDelta; //!< Delta between to field offsets

    void push_valueinfo(Typelib::Type const& type);
    void push_container(VectorToc* toc_ptr);

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
    unsigned int getPosition() { return mPosition; }
};

} // namespace general_processing

#endif // GENERALPROCESSING_VECTORTOCMAKER_HPP

