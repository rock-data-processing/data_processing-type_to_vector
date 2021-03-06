/**
 * \file VectorToc.hpp
 *
 * \brief To describe the content with conversion specific informations of a type.
 *
 *
 * \todo use hashes instead of strings to describe the place in a type.
 */

#ifndef TYPETOVECTOR_VECTORTOC_HPP
#define TYPETOVECTOR_VECTORTOC_HPP

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include <utilmm/stringtools.hh>

#include "NumericConverter.hpp"

namespace type_to_vector {

struct VectorToc;
typedef boost::shared_ptr<VectorToc> VectorTocPointer;

/** Information to which place in a type a vector value belongs. 
 *
 * Only types that should be stored is numerics and containers.
 * Containers will not increase the position. They have to be
 * determined during conversion time. */
struct VectorValueInfo {
    std::string placeDescription; //!< Something like position.3, rotation.im.1 or ...
    unsigned int position; //!< The position in bytes in the memory of this value.
    CastFunction castFun; //!< To cast the value, 0 for container or other type.
    BackCastFunction backCastFun; //!< Cast it back to the original type.
    VectorTocPointer content; //!< Subcontent (is needed for containers).
    std::string containerType; //!< Type name of the content aka container.

public:
    VectorValueInfo();

    bool operator== (const VectorValueInfo& other) const;
};


/** Table of contents for the vector made of a type.
 *
 * This class holds the information where to find what in the type and how to put
 * it into a vector.
 */
struct VectorToc : public std::vector<VectorValueInfo> {

    std::string mType; //!< The type the toc is made for.
    std::string mSlice; //!< Slice operations made and marks a concrete toc.
    int maxDepth; //!< Maximum recursion depth for the visitors.

    VectorToc();

    void clear();

    /** Checks if the toc has any containers. */
    bool isFlat();

private:
    class EqualityVisitor;
};

/** To recursivly go into containers. */
class VectorTocVisitor {
    int mMaxDepth; //!< Max recursive depth (<0 means no depth limit).
    int mDepth;
protected:
    virtual void visit(VectorValueInfo const& info);
    virtual void visit(VectorToc const& toc);
public:
    VectorTocVisitor(int max_depth=-1) : mMaxDepth(max_depth), mDepth(0) {}
};

class SliceTree;

/** Generates a VectorToc by slicing another VectorToc.
 *
 * This can be used to convert a vector and should be faster
 * since it has not to go through the whole VectorToc of a type.*/
class VectorTocSlicer : public VectorTocVisitor {

    const VectorToc& mToc;
    std::vector<VectorToc> mResultStack;
    utilmm::stringlist mPlaceStack;

    SliceTree* mpMatcher;

    void push_element (const VectorValueInfo& info);

protected:
    void visit (const VectorValueInfo& info);

public:

    VectorTocSlicer (const VectorToc& toc);
    ~VectorTocSlicer ();

    VectorToc apply (const std::string& slice);

    static VectorToc slice (const VectorToc& toc, const std::string& slice);
};



} // namespace type_to_vector
#endif // TYPETOVECTOR_VECTORTOC_HPP

