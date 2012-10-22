// \file  Utilities.cpp

#include "Utilities.hpp"

using namespace general_processing;

std::ostream& operator<< (std::ostream& os, const VectorValueInfo& info) {
    os << info.placeDescription << ":" << info.position << "[" << info.type.getName();
    os << "] " << info.containerLoop << ":" << info.containerSlice;
    return os;
}

std::ostream& operator<< (std::ostream& os, const VectorToc& toc) {
    VectorToc::const_iterator it = toc.begin();
    os << toc.typeName() << ":" << toc.sliceString() << "\n";
    for (; it != toc.end(); it++)
        os << "  " << *it << "\n";
    return os;
}
