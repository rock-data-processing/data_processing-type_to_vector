// \file  VectorToc.cpp

#include <stdexcept>

#include "SliceMatcher.hpp"

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


void VectorTocSlicer::push_element(const VectorValueInfo& info) {

    if (mpMatcher) {

        if (info.placeDescription != "" ) mPlaceStack.push_back(info.placeDescription);


        std::string this_place = utilmm::join(mPlaceStack,".");

        if (mpMatcher->fitsASlice(this_place)) mResultStack.back().push_back(info);

        if (info.placeDescription != "" ) mPlaceStack.pop_back();

    } else
        mResultStack.back().push_back(info);
}

void VectorTocSlicer::visit(const VectorValueInfo& info) {
    
    if (info.content.get()) {
        
        int istar;
        
        if (mpMatcher ) {
            
            mPlaceStack.push_back(info.placeDescription);

            mResultStack.push_back(VectorToc());

            VectorTocVisitor::visit(*(info.content));

            if (!mResultStack.back().empty()) {

                VectorValueInfo vvi(info);
                vvi.content.reset(new VectorToc(mResultStack.back()));
                mResultStack.pop_back();
                mResultStack.back().push_back(vvi);

            } else
                mResultStack.pop_back();

            mPlaceStack.pop_back();
        }

    } else
        push_element(info);

}

VectorTocSlicer::VectorTocSlicer(const VectorToc& toc) : mToc(toc), mpMatcher(0) {
}

VectorTocSlicer::~VectorTocSlicer() {
    if (mpMatcher) delete mpMatcher;
}

VectorToc VectorTocSlicer::apply(const std::string& slice) {
    
    if ( mpMatcher ) {
        delete mpMatcher;
        mpMatcher = 0;
    }
    if (slice != "") mpMatcher = new SliceMatcher(slice, true); 

    mResultStack.clear();
    mResultStack.push_back(VectorToc());
    mResultStack.back().mType = mToc.mType;
    mResultStack.back().mSlice = mToc.mSlice + "|" + slice;

    VectorTocVisitor::visit(mToc);
    
    if ( mpMatcher ) {
        delete mpMatcher;
        mpMatcher = 0;
    }
    
    return mResultStack.back();
}

VectorToc VectorTocSlicer::slice(const VectorToc& toc, const std::string& slice) {

    VectorTocSlicer vts(toc);
    return vts.apply(slice);
}
