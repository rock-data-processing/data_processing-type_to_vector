#include "BackConverter.hpp"

#include "SliceMatcher.hpp"

using namespace type_to_vector;

void FlatBackConverter::setSlice(const std::string& slice) {

    if ( mpMatcher ) {
        delete mpMatcher;
        mpMatcher = 0;
    }

    if (slice != "") mpMatcher = new SliceMatcher(slice);
}

FlatBackConverter::~FlatBackConverter() {

    if (mpMatcher) delete mpMatcher;
}

void FlatBackConverter::apply(const VectorOfDoubles& vec, void* data) {

    mpData = data;
    mpVec = &vec;
    mElementCounter = 0;

    VectorTocVisitor::visit(mToc);

    mpVec = 0;
    mpData = 0;
}

void FlatBackConverter::setElement(const VectorValueInfo& info) {

    if (mElementCounter >= mpVec->size()) return;

    if (!mpMatcher || mpMatcher->fitsASlice(info.placeDescription)) {

        void* ptr = mpData + info.position;
        info.backCastFun(ptr, mpVec->at(mElementCounter));
        mElementCounter++;
    }
}

void FlatBackConverter::visit(const VectorValueInfo& info) {

    // Pointer to content equals zero means no container.
    if (!info.content.get()) setElement(info);
}

void BackConverter::visit(const VectorValueInfo& info) {
}
