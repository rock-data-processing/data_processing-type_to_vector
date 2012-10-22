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

#ifndef LOBBASE_UTILITIES_HPP
#define LOBBASE_UTILITIES_HPP

#include <iostream>
#include "VectorValueInfo.hpp"

std::ostream& operator<< (std::ostream& os, const lob_base::VectorValueInfo& info):
std::ostream& operator<< (std::ostream& os, const lob_base::VectorToc& toc);

#endif //  LOBBASE_UTILITIES_HPP
