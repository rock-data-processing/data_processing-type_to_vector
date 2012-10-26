// \file  VectorToc.cpp

#include <stdexcept>

#include "VectorToc.hpp"

using namespace general_processing;

VectorValueInfo::VectorValueInfo() : 
    placeDescription(""), position(0), castFun(0) {}

bool VectorValueInfo::operator==(const VectorValueInfo& other ) const {
    return placeDescription == other.placeDescription &&
        position == other.position &&
        castFun == other.castFun;
}


VectorToc::VectorToc() : mType(""), mSlice(""), maxDepth(100) {}

void VectorToc::clear() {
    mType = "";
    mSlice = "";
    std::vector<VectorValueInfo>::clear();
}

bool VectorToc::isFlat() {
    VectorToc::const_iterator it = begin();
    for ( ; it != end(); it++)
        if ( it->content.get() ) return false;
    return true;
}


void VectorTocVisitor::visit(VectorValueInfo const& info) {
    if (info.content.get()) {
        mDepth++;
        if (mMaxDepth >= 0 && mDepth > mMaxDepth ) 
            throw std::runtime_error("reached maximum recursion depth in VectorTocVistor");
        visit(*(info.content));
        mDepth--;
    }
}

void VectorTocVisitor::visit(VectorToc const& toc) {
    VectorToc::const_iterator it = toc.begin();
    for ( ; it<toc.end(); it++)
        visit(*it);
}
