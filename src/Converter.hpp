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

#include <typelib/value.hh>

#include <Eigen/Core>

#include "VectorToc.hpp"

class Typelib::Registry;

namespace general_processing {

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

    std::vector<std::string> mFlatToc;
    std::vector<double> mVector;

    const VectorToc&  mToc;

    const Typelib::Value* mpValue;
    std::vector<void*> mBaseStack;
    std::vector<int> mContainersSizeStack;
    
    std::string mSlice;

    void* getPosition(const VectorValueInfo& info); 

protected:
    void visit (const VectorValueInfo& info);
    void visit (const VectorToc& toc);

public:
    /** Construction of the converter.
     *
     * \param toc is the \c VectorToc that describes the data.
     * \param registry it is needed when containers are in the type to resolve the
     * element count.
     */
    ConvertToVector ( const VectorToc& toc, const Typelib::Registry& registry);
    
    /** Applies the visitor to a value. */
    std::vector<double> apply (const Typelib::Value& value, const std::string& slice ="");
    
    Eigen::VectorXd getEigenVector ();
    
    /** Returns the flat toc that describes what the vector actually contains. 
     *
     * Each entry gives the place in the type for the vector element at this index.*/
    std::vector<std::string> getFlatToc () { return mFlatToc; }
};

} // namespace general_processing

#endif // GENERALPROCESSING_CONVERTER_HPP


