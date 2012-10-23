/**
 * \file  Utilities.hpp
 *
 * \brief Utilites for the package.
 *
 *      German Research Center for Artificial Intelligence\n
 *      Project: VirGo4
 *
 * \date 19.10.2012
 *
 * \author felix.rehrmann@dfki.de
 */

#ifndef GENERALPROCESSING_UTILITIES_HPP
#define GENERALPROCESSING_UTILITIES_HPP

#include <iostream>
#include <vector>
#include <string>
#include <utilmm/stringtools.hh>
#include "VectorToc.hpp"

std::ostream& operator<< (std::ostream& os, const general_processing::VectorValueInfo& info);
std::ostream& operator<< (std::ostream& os, const general_processing::VectorToc& toc);


namespace general_processing {

/** Converts a toc into a vector of strings. */
class PlainTocVisitor : public VectorTocVisitor {

    utilmm::stringlist mPlaceStack;
    std::vector<std::string> mPlainToc;

    void push_place();

protected:
    void visit ( VectorValueInfo const& info );
    void visit ( VectorToc const& toc );
public:
    PlainTocVisitor(int max_depth=-1) : VectorTocVisitor(max_depth) {}
    std::vector<std::string> apply( VectorToc const& toc );
};

} // namespace general_processing

#endif //  GENERALPROCESSING_UTILITIES_HPP
