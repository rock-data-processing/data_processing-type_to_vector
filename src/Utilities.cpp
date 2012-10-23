// \file  Utilities.cpp

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


void PlainTocVisitor::push_place () {
    mPlainToc.push_back(utilmm::join(mPlaceStack,"."));
}

void PlainTocVisitor::visit ( VectorValueInfo const& info ) {
    mPlaceStack.push_back(info.placeDescription); 
    if (!info.content) push_place();
    else VectorTocVisitor::visit(info);
    mPlaceStack.pop_back();
}

void PlainTocVisitor::visit ( VectorToc const& toc ) {
    mPlaceStack.push_back("*");
    VectorTocVisitor::visit(toc);
    mPlaceStack.pop_back();
}

std::vector<std::string> PlainTocVisitor::apply( VectorToc const& toc ) {
    mPlainToc.clear();
    mPlaceStack.clear();
    visit(toc);
    return mPlainToc;
}


