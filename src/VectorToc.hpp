/**
 * \file VectorToc.hpp
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

#ifndef GENERALPROCESSING_VECTORTOC_HPP
#define GENERALPROCESSING_VECTORTOC_HPP

#include <string>
#include <vector>

#include <typelib/typemodel.hh>
#include <typelib/value.hh>

#include <Eigen/Core>

#include "NumericConverter.hpp"

namespace general_processing {

struct VectorToc;

/** Information to which place in a type a vector value belongs. 
 *
 * Only types that should be stored is numerics and containers.
 * Containers will not increase the position. They have to be
 * determined during conversion time. */
struct VectorValueInfo {
    std::string placeDescription; //!< Something like position.3, rotation.im.1 or ...
    unsigned int position; //!< The position in bytes in the memory of this value.
    CastFunction castFun; //!< To cast the value, 0 for container or other type.
    VectorToc* content; //!< Subcontent (is needed mostly for containers).

protected:
    class DeleteVisitor; //!< To also delete recursivly all content.
 
public:
    VectorValueInfo();
    ~VectorValueInfo();

    bool operator== (const VectorValueInfo& other) const;
};


/** Table of contents for the vector made of a type.
 *
 * This class holds the information where to find what in the type and how to put
 * it into a vector.
 */
struct VectorToc : public std::vector<VectorValueInfo> {

    std::string mType; //!< The type the toc is made for.
    std::string mSlice; //!< Slice operation made and marks a concrete toc.
    int maxDepth; //!< Maximum recursion depth for the visitors.

    VectorToc();

    std::vector<double> valueToVector (const Typelib::Value& value);
    Eigen::VectorXd valueToEigen (const Typelib::Value& value);

    Typelib::Value vectorToValue (const std::vector<double>& vector);
    Typelib::Value eigenToValue (const Eigen::VectorXd& vector);
    
    /** Gives the concret toc for a certain value. 
     *
     * The toc is different if containers are part of the type. 
     * The size for containers is determined and the toc has only one level.
     * The slice is regarded as well.*/
    VectorToc concreteToc (const Typelib::Value& value);

    void clear();

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

} // namespace general_processing
#endif // GENERALPROCESSING_VECTORTOC_HPP

