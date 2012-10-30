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

#ifndef GENERALPROCESSING_CONVERTER_HPP
#define GENERALPROCESSING_CONVERTER_HPP

#include <Eigen/Core>
#include <typelib/value.hh>
#include <utilmm/stringtools.hh>

#include "Definitions.hpp"
#include "VectorToc.hpp"


class Typelib::Registry;

namespace general_processing {

class SliceMatcher;

/** Converts a \c Typelib::Value to a vector according to a \c VectorToc. 
 *
 * Use apply to run the visitor and then getDoubleVector or getEigenVector to
 * get the result. 
 *
 * The function getFlatToc gives the concrete toc for the last handled value. 
 *
 * \warning std containers are handled, but for other containers it might not work. */
class ConvertToVector : public VectorTocVisitor {

    const Typelib::Registry& mrRegistry;

    bool mCreatePlaceVector;
    StringVector mPlaceVector;
    std::vector<double> mVector;

    const VectorToc&  mToc;

    const Typelib::Value* mpValue;
    std::vector<void*> mBaseStack;
    std::vector<int> mContainersSizeStack;
    utilmm::stringlist mPlaceStack;

    SliceMatcher* mpMatcher;
    
    void* getPosition (const VectorValueInfo& info); 

    void push_element (const VectorValueInfo& info);

protected:
    void visit (const VectorValueInfo& info);

public:
    /** Construction of the converter.
     *
     * \param toc is the \c VectorToc that describes the data.
     * \param registry it is needed when containers are in the type to resolve the
     * element count.
     */
    ConvertToVector ( const VectorToc& toc, const Typelib::Registry& registry);

    ~ConvertToVector();
   
    /** Applies the converter to a \c Typelib::Value and returns a vector of doubles.
     *
     * To get an eigen vector use getEigenVector after calling apply.
     * \param create_place_vector \see getPlaceVector */ 
    std::vector<double> apply ( const Typelib::Value& value, 
                                bool create_place_vector = false );
    
    Eigen::VectorXd getEigenVector ();
    
    /** Returns a vector of string that describes what the conversion results actually contains. 
     *
     * Each entry gives the place in the type for the vector element at this index.*/
    StringVector getPlaceVector () { return mPlaceVector; }

    /** Sets a slice. "" is no slice. */
    void setSlice (const std::string& slice);
};

} // namespace general_processing

#endif // GENERALPROCESSING_CONVERTER_HPP


