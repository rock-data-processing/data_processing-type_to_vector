// \file  Utilities.cpp

#include "Utilities.hpp"

using namespace type_to_vector;

std::ostream& operator<< (std::ostream& os, const VectorValueInfo& info) {
    os << info.placeDescription << ":" << info.position
        << " (" <<  info.content << ")";
    if (info.content.get())
        os << ">" << info.content->mType;
    return os;
}

std::ostream& operator<< (std::ostream& os, const VectorToc& toc) {
    VectorToc::const_iterator it = toc.begin();
    os << toc.mType << ":" << toc.mSlice << "\n";
    for (; it != toc.end(); it++) {
        os << "  " << *it << "\n";
        if (it->content.get())
            os << *(it->content);
    }
    return os;
}


void PlainTocVisitor::push_place () {
    if (!mPlaceStack.empty())
        mPlainToc.push_back(utilmm::join(mPlaceStack,"."));
    else
        mPlainToc.push_back("");
}

void PlainTocVisitor::visit ( VectorValueInfo const& info ) {

    if (!info.placeDescription.empty()) mPlaceStack.push_back(info.placeDescription); 

    if (!info.content.get()) push_place();
    else VectorTocVisitor::visit(info);

    if (!info.placeDescription.empty()) mPlaceStack.pop_back();
}

std::vector<std::string> PlainTocVisitor::apply( VectorToc const& toc ) {
    mPlainToc.clear();
    mPlaceStack.clear();
    VectorTocVisitor::visit(toc);
    return mPlainToc;
}
    

void EqualityVisitor::visit(VectorValueInfo const& info) {
    if ( !(*mOtherInfo == info) ) {
        mEqual = false;
        return;
    }

    if ( mOtherInfo->content.get() && info.content.get() ) {
        mOtherStack.push_back(mOtherInfo->content.get());
        VectorTocVisitor::visit(info);
        mOtherStack.pop_back();
    }
    else if ( (info.content.get() == 0) != (mOtherInfo->content.get() == 0) ) {
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


bool type_to_vector::operator== (const VectorToc& one, const VectorToc& two) {
    EqualityVisitor ev(one);
    return ev.apply(two);
}

