// \file  Utilities.cpp

#include "Utilites.hpp"

using namespace lob_base;

std::ostream& operator<< (std::ostream& os, const VectorValueInfo& info) {
    os << info.placeDescripton << ":" << info.position << "[" << info.type.getName();
    os << "] " << info.containerLoop << ":" << info.containerSlice;
    return os;
}

std::ostream& operator<< (std::ostream& os, const VectorToc& toc) {
    TypeToc::const_iterator it = toc.begin();
    os << toc.type.getName() << ":" << toc.slice << "\n"
    for (; it != toc.end(); it++)
        os << "  " << *it << "\n";
    return os;
}
