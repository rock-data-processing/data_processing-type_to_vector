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

namespace general_processing {

/** Converts a \c Typelib::Value to a vector according to a \c VectorToc. 
 *
 * Use apply to run the visitor and then getDoubleVector or getEigenVector to
 * get the result. 
 *
 * The function getConcreteToc gives the concrete toc for the last handled value.
 * This is interessting when a value has containers in it. If the containers doesn't
 * change their size this concreteToc could be resued later as a conversion recipe.*/
class ConvertToVector : public VectorTocVisitor {

    std::vector<double> mVector;
    VectorToc mConcreteToc;
    const VectorToc&  mToc;
    const Typelib::Value* mpValue;
    std::string mSlice;

protected:
    void visit (const VectorValueInfo& info);
    void visit (const VectorToc& toc);

public:
    ConvertToVector ( const VectorToc& toc );
    
    /** Applies the visitor to a value. */
    std::vector<double> apply (const Typelib::Value& value, const std::string& slice ="");
    
    Eigen::VectorXd getEigenVector ();
    
    /** Returns the concrete toc that describes what the vector actually contains. 
     *
     * The toc is different form the type's toc if containers are part of the type. 
     * The size for containers is determined so the toc has only one level. */
    VectorToc getConcreteToc () { return mConcreteToc; }
};

} // namespace general_processing

#endif // GENERALPROCESSING_CONVERTER_HPP


