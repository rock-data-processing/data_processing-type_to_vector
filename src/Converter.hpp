/**
 * \file  Converter.hpp
 *
 * \brief Convertsion to vectors. 
 *
 *      German Research Center for Artificial Intelligence\n
 *      Project: VirGo4
 *
 * \date 25.10.2012
 *
 * \author felix.rehrmann@dfki.de
 */

#ifndef TYPETOVECTOR_CONVERTER_HPP
#define TYPETOVECTOR_CONVERTER_HPP

#include <Eigen/Core>
#include <typelib/value.hh>
#include <utilmm/stringtools.hh>

#include "Definitions.hpp"
#include "VectorToc.hpp"


class Typelib::Registry;

namespace type_to_vector {

class SliceMatcher;


/** Basic functionality of converters. */
class AbstractConverter {

protected:    
    const VectorToc mToc;
    
    VectorOfDoubles mVector;
    
    StringVector mPlaceVector;
      
public:
    typedef boost::shared_ptr<AbstractConverter> Pointer;
    
    AbstractConverter (const VectorToc& toc) : mToc(toc) {}

    std::string getTypeName() { return mToc.mType; }

    const VectorToc& getToc() { return mToc; }
    
    /** Applies the converter to a \c Typelib::Value and returns a vector of doubles.
     *
     * To get an eigen vector use getEigenVector after calling apply.
     * \param create_place_vector \see getPlaceVector */ 
    VectorOfDoubles applyToValue (const Typelib::Value& value, 
            bool create_place_vector=false);
    
    /** Applies the converter to a some data and returns a vector of doubles.
     *
     * To get an eigen vector use getEigenVector after calling apply.
     * \param create_place_vector \see getPlaceVector */ 
    virtual VectorOfDoubles apply (void* data, bool create_place_vector = false) = 0;
    
    /** Returns the result of the last conversion as an Eigen::VectorXd. */
    Eigen::VectorXd getEigenVector ();

    /** To get the result of the last conversion. 
     *
     * \returns false if there are no data in the vector.*/
    template <typename Derived>
    bool getEigenVector(Eigen::DenseBase<Derived>& vector) const {

       if ( !mVector.empty() ) {
           vector = Eigen::Map<const Eigen::VectorXd>(&(mVector[0]), mVector.size());
           return true;
        } else
            return false;
    }
    
    /** Returns a vector of string that describes what the conversion results actually 
     *  contains. 
     *
     * Each entry gives the place in the type for the vector element at this index.
     * Should only be created when create_place_vector was set to true. */
    StringVector getPlaceVector () { return mPlaceVector; }
};

/** Only converts a single value (the first one in the toc). */
class SingleConverter : public AbstractConverter {
public:
    SingleConverter (const VectorToc& toc) : AbstractConverter(toc) {}

    VectorOfDoubles apply (void* data, bool create_place_vector = false);

};

/** Can be used to apply a factor to all converted values. */
class MultiplyConverter: public AbstractConverter {

    AbstractConverter::Pointer mpConverter;
    double mFactor;

public:
    MultiplyConverter (AbstractConverter::Pointer converter, double factor);
    
    virtual VectorOfDoubles apply (void* data, bool create_place_vector = false);

    double getFactor() { return mFactor; }
    void setFactor (double factor) { mFactor = factor; }
};

/** Only converts the first level of an type. Will not go into containers. */
class FlatConverter : public AbstractConverter, public VectorTocVisitor {
      
protected:
    void* mpData;
    
    bool mCreatePlaceVector;

    SliceMatcher* mpMatcher;
    
    virtual void* getPosition (const VectorValueInfo& info); 
   
    virtual void push_element (const VectorValueInfo& info);
     
    virtual void visit (const VectorValueInfo& info);

public:
    
    /** Construction of the converter.
     *
     * \param toc is the \c VectorToc that describes the data.
     */
    FlatConverter (const VectorToc& toc);

    virtual ~FlatConverter ();
   
    virtual VectorOfDoubles apply (void* data, bool create_place_vector = false);
    
    /** Sets a slice. "" is no slice. */
    void setSlice (const std::string& slice);
};
    

/** Converts a \c Typelib::Value to a vector according to a \c VectorToc. 
 *
 * Use apply to run the visitor and then getDoubleVector or getEigenVector to
 * get the result. 
 *
 * The function getFlatToc gives the concrete toc for the last handled value. 
 *
 * \warning std containers are handled, but for other containers it might not work. */
class ConvertToVector : public FlatConverter {

    const Typelib::Registry& mrRegistry;

    std::vector<void*> mBaseStack;
    std::vector<int> mContainersSizeStack;
    utilmm::stringlist mPlaceStack;

protected:
    void* getPosition (const VectorValueInfo& info); 

    void push_element (const VectorValueInfo& info);

    void visit (const VectorValueInfo& info);

public:
    /** Construction of the converter.
     *
     * \param toc is the \c VectorToc that describes the data.
     * \param registry it is needed when containers are in the type to resolve the
     * element count.
     */
    ConvertToVector (const VectorToc& toc, const Typelib::Registry& registry);

    VectorOfDoubles apply (void* data, bool create_place_vector = false);
};

} // namespace type_to_vector

#endif // TYPETOVECTOR_CONVERTER_HPP


