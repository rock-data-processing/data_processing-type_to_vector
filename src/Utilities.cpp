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
    

void EqualityVisitor::visit(VectorValueInfo const& info) {
    if ( !(*mOtherInfo == info) ) {
        mEqual = false;
        return;
    }

    if ( mOtherInfo->content && info.content ) {
        mOtherStack.push_back(mOtherInfo->content);
        VectorTocVisitor::visit(info);
        mOtherStack.pop_back();
    }
    else if ( (info.content == 0) != (mOtherInfo->content == 0) ) {
        mEqual = false;
        return;
    }
}

void EqualityVisitor::visit(VectorToc const& toc){
    if ( toc.mType != mOtherStack.back()->mType || 
         toc.mSlice != mOtherStack.back()->mSlice ||
         toc.size() != mOtherStack.back()->size() ) {
        
        mEqual = false;
        return;
    }

    VectorToc::const_iterator it = toc.begin(), oit = mOtherStack.back()->begin();
    for ( ; it != toc.end(); it++, oit++ ) {
        mOtherInfo = &(*oit);
        visit(*it);
        if (!mEqual) break;
    }
}

bool EqualityVisitor::apply(const VectorToc& other) {
    mEqual = true;
    mOtherStack.clear();
    mOtherStack.push_back(&other);
    visit(mThisToc);
    return mEqual;
}


bool general_processing::operator== (const VectorToc& one, const VectorToc& two) {
    EqualityVisitor ev(one);
    return ev.apply(two);
}

