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
    typedef boost::shared_ptr<AbstractBackConverter> Pointer;

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

    const VectorToc mToc;
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
    virtual void* getPosition(const VectorValueInfo& info);
    virtual void setElement(const VectorValueInfo& info);
    virtual void visit(const VectorValueInfo& info);

    void* mpData;
    SliceMatcher* mpMatcher;
    const VectorOfDoubles* mpVec;
    unsigned int mElementCounter;
};

/** Fill the type with data from a vector. 
 *
 * Containers are take as they are. That means they will not be resized and
 * resizing of containers in a type has to be done prior to back conversion.
 *  
 * The elements of a type will be set to the vector values as long as values
 * remain. The filling stops when either the type ends or there are no values in 
 * the vector anymore.
 */
class BackConverter : public FlatBackConverter {

public:
    BackConverter(const VectorToc& toc, const Typelib::Registry& registry) : 
        FlatBackConverter(toc), mrRegistry(registry) {}

    virtual void apply(const VectorOfDoubles& vec, void* data);

protected:
    virtual void* getPosition(const VectorValueInfo& info);
    virtual void setElement(const VectorValueInfo&);
    virtual void visit(const VectorValueInfo& info);
    
    const Typelib::Registry& mrRegistry;    
    std::vector<void*> mBaseStack;
    std::vector<int> mContainersSizeStack;
    utilmm::stringlist mPlaceStack;
};

} // namespace type_to_vector
#endif // TYPETOVECTOR_BACKCONVERTER_HPP
