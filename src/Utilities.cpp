// \file  Utilities.cpp

#include "ValueToVector.hpp"
#include "Utilities.hpp"

using namespace general_processing;

std::ostream& operator<< (std::ostream& os, const VectorValueInfo& info) {
    os << info.placeDescription << ":" << info.position;
    if (info.content)
        os << ">" << info.content->mType;
    return os;
}

std::ostream& operator<< (std::ostream& os, const VectorToc& toc) {
    VectorToc::const_iterator it = toc.begin();
    os << toc.mType << ":" << toc.mSlice << "\n";
    for (; it != toc.end(); it++) {
        os << "  " << *it << "\n";
        if (it->content)
            os << *(it->content);
    }
    return os;
}
