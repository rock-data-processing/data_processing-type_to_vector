/**
 * \file  TestTocs.hpp
 *
 * \brief The toc strings of the test types.
 *
 *      German Research Center for Artificial Intelligence\n
 *      Project: VirGo4;
 *
 * \date 23.10.2012
 *
 * \author felix.rehrmann@dfki.de
 */

#ifndef GENERALPROCESSING_TESTOCS_HPP
#define GENERALPROCESSING_TESTOCS_HPP

#include <string>

#include <utilmm/stringtools.hh>

namespace plain_tocs {

std::string Scalar = "";
std::string Array3 = "0 1 2";
std::string A = "a b c d";
std::string DoubleVector = "a dbl_vector.*";
std::string VectorArray = "dbl_vector_array.0.* dbl_vector_array.1.* "
    "dbl_vector_array.2.*";
std::string StructArray = "A_vector.*.a A_vector.*.b A_vector.*.c A_vector.*.d";
std::string ContainerContainer = "dbl_vv.*.a dbl_vv.*.dbl_vector.*";
std::string StdString = "*"

std::vector<std::string> getPlainToc(const std::string& s) {
    stringlist ls = utilmm.split(s);
    return std::vector<std::string>(ls.begin(),ls.end());
}

} // plain_toc
#endif //  GENERALPROCESSING_TESTOCS_HPP
