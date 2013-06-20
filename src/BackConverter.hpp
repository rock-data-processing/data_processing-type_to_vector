/** \file BackConverter.hpp
 *
 *  Converters that take values from a vector and put it into a type.
 */
#ifndef TYPETOVECTOR_BACKCONVERTER_HPP
#define TYPETOVECTOR_BACKCONVERTER_HPP

#include <Eigen/Core>
#include <typelib/value.hh>

#include "Definitions.hpp"
#include "VectorToc.hpp"

namespace type_to_vector {

class SliceMatcher;

/** Base class for the back converters. */
class AbstractBackConverter {

public:
    /** The constructor of any BackConverter takes the toc for the type it is
     *  meant for. 
     */
    AbstractBackConverter(const VectorToc& toc) : mToc(toc) {}

    /** Fills the memory at target, which holds a type given by toc, with the
     * data from vec.
     */
    virtual void apply(const VectorOfDoubles& vec, void* target) = 0;

    /** Puts data from an eigen vector/matrix into the target. */
    template <typename Derived>
    void fromEigen(const Eigen::DenseBase<Derived>& vec, void* target) {

        VectorOfDoubles vod;
        vod.resize(vec.size());
        for(int i = 0; i < vec.size(); i++) vod[i] = vec.data()[i];

        apply(vod, target);  
    }

protected:
    const VectorToc& mToc;
};

/** Only fills level one of the type with the vector data. 
 *
 * Containers are not touched.
 */
class FlatBackConverter : public AbstractBackConverter, public VectorTocVisitor {

public:
    FlatBackConverter(const VectorToc& toc) : 
        AbstractBackConverter(toc), mpMatcher(0), mpData(0) {}
    virtual ~FlatBackConverter();
    
    virtual void apply(const VectorOfDoubles& vec, void* data);
    void setSlice(const std::string& slice);

protected:
    virtual void visit(const VectorValueInfo& info);

    void* mpData;
    SliceMatcher* mpMatcher;
};

/** Give the size of a conatiner. */
class ContainerSizeDefs : public std::map<std::string, unsigned int> {

public:
    /** Sets a size for a position.
     *
     * \param position The position string in the type.
     * \param container_size The lenght of the container.
     */
    void setSize(const std::string& position, unsigned int container_size) {
        insert(std::pair<std::string,  unsigned int>(position, container_size));
    }
};

/** Fill the type with data from a vector. 
 *
 * Containers sizes must be given, either by presizing the container, 
 * or by giving the size of the containters in the type.
 *
 * Container sizing behaves as follows:
 * 1. If no sizes are given, the containers will keep their size and filled up 
 *    to that size.
 * 2. If sizes are given, the container will be resized to the size and filled 
 *    with up to the given size with values.
 *  
 * The elements of a type will be set to the vector values as long as values
 * remain. The filling stops when either the type ends or there are no values in 
 * the vector anymore.
 */
class BackConverter : public FlatBackConverter {

public:
    BackConverter(const VectorToc& toc, const Typelib::Registry& registry) : 
        FlatBackConverter(toc), mrRegistry(registry) {}

    /** Returning the map to define the container sizes. */
    ContainerSizeDefs& containerSizes() { return mContainerSizes; }


protected:
    virtual void visit(const VectorValueInfo& info);
    
    const Typelib::Registry& mrRegistry;    
    ContainerSizeDefs mContainerSizes;
};

} // namespace type_to_vector
#endif // TYPETOVECTOR_BACKCONVERTER_HPP
