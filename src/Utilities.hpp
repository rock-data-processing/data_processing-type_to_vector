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

#ifndef TYPETOVECTOR_UTILITIES_HPP
#define TYPETOVECTOR_UTILITIES_HPP

#include <iostream>
#include <vector>
#include <string>

#include <utilmm/stringtools.hh>
#include "VectorToc.hpp"

std::ostream& operator<< (std::ostream& os, const type_to_vector::VectorValueInfo& info);
std::ostream& operator<< (std::ostream& os, const type_to_vector::VectorToc& toc);

namespace type_to_vector {

/** Converts a toc into a vector of strings. */
class PlainTocVisitor : public VectorTocVisitor {

    utilmm::stringlist mPlaceStack;
    std::vector<std::string> mPlainToc;

    void push_place();

protected:
    void visit ( VectorValueInfo const& info );
    //void visit ( VectorToc const& toc );
public:
    PlainTocVisitor(int max_depth=-1) : VectorTocVisitor(max_depth) {}
    std::vector<std::string> apply( VectorToc const& toc );
};

/** Determines whether two tocs are equal or not. */
class EqualityVisitor : public VectorTocVisitor {
    bool mEqual;
    const VectorToc& mThisToc;
    std::vector<const VectorToc*> mOtherStack;
    const VectorValueInfo* mOtherInfo;

protected:
    void visit(VectorValueInfo const& info);
    void visit(VectorToc const& toc);

public:
    EqualityVisitor(const VectorToc& this_toc, int max_depth=-1) : 
        VectorTocVisitor(max_depth), mThisToc(this_toc), mOtherInfo(0) {}

    bool apply(const VectorToc& other);
};


bool operator== (const VectorToc& one, 
        const VectorToc& two);

inline bool operator!= (const VectorToc& one, 
        const VectorToc& two) { return !operator==(one,two); }

} // namespace type_to_vector



#endif //  TYPETOVECTOR_UTILITIES_HPP
