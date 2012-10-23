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

struct general_processing::VectorValueInfo;
struct general_processing::VectorToc;

std::ostream& operator<< (std::ostream& os, const general_processing::VectorValueInfo& info);
std::ostream& operator<< (std::ostream& os, const general_processing::VectorToc& toc);

#endif //  GENERALPROCESSING_UTILITIES_HPP
