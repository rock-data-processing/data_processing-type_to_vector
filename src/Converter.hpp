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
 * This visitor is mainly used when there is a container in the type. If
 * the container size doesn't change in a row of conversion the
 * flattend container free toc can be used beginning with the second conversion.
 *
 * Use apply to run the visitor and then getDoubleVector or getEigenVector to
 * get the result. 
 *
 * The function getFlatToc gives the concrete toc for the last handled value.
 * This is interessting when a value has containers in it. If the containers doesn't
 * change their size this concreteToc could be resued later as a conversion recipe.*/
class ConvertToVector : public VectorTocVisitor {

    const Typelib::Registry& mrRegistry;

    std::vector<double> mVector;

    VectorToc mFlatToc;
    const VectorToc&  mToc;

    const Typelib::Value* mpValue;
    
    std::string mSlice;

protected:
    void visit (const VectorValueInfo& info);
    void visit (const VectorToc& toc);

public:
    /** Construction of the Converter.
     *
     * \param toc is the \c VectorToc that describes the data.
     * \param registry it is needed when containers are in the type to resolve the
     * element count.
     */
    ConvertToVector ( const VectorToc& toc, const Typelib::Registry& registry);
    
    /** Applies the visitor to a value. */
    std::vector<double> apply (const Typelib::Value& value, const std::string& slice ="");
    
    Eigen::VectorXd getEigenVector ();
    
    /** Returns the flat toc that describes hat the vector actually contains. 
     *
     * The toc is different form the type's toc if containers are part of the type. 
     * The size for containers is determined so the toc has only one level. */
    VectorToc getFlatToc () { return mFlatToc; }
};

} // namespace general_processing

#endif // GENERALPROCESSING_CONVERTER_HPP


