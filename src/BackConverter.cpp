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

void* FlatBackConverter::getPosition (const VectorValueInfo& info) {

    return mpData + info.position;
}

void FlatBackConverter::setElement(const VectorValueInfo& info) {

    if (mElementCounter >= mpVec->size()) return;

    if (!mpMatcher || mpMatcher->fitsASlice(info.placeDescription)) {

        info.backCastFun(getPosition(info), mpVec->at(mElementCounter));
        mElementCounter++;
    }
}

void FlatBackConverter::visit(const VectorValueInfo& info) {

    // Pointer to content equals zero means no container.
    if (!info.content.get()) setElement(info);
}

void BackConverter::apply(const VectorOfDoubles& vec, void* data) {

    mpData = data;
    mpVec = &vec;
    mElementCounter = 0;
    mBaseStack.clear();
    mBaseStack.push_back(data);
    mContainersSizeStack.clear();
    mPlaceStack.clear();

    VectorTocVisitor::visit(mToc);

    mpVec = 0;
    mpData = 0;
}

void* BackConverter::getPosition(const VectorValueInfo& info) {

    void* ptr = mBaseStack.back() + info.position;

    if (!mContainersSizeStack.empty())
        ptr += mContainersSizeStack.back();

    return ptr;
}

void BackConverter::setElement(const VectorValueInfo& info) {

    if (mElementCounter >= mpVec->size()) return;

    bool push_this = true;
    
    if (mpMatcher) {

        if (info.placeDescription != "" ) mPlaceStack.push_back(info.placeDescription);
        std::string this_place = utilmm::join(mPlaceStack,".");
        push_this = mpMatcher->fitsASlice(this_place);
        if (info.placeDescription != "" ) mPlaceStack.pop_back();
    }

    if (push_this) {
        info.backCastFun(getPosition(info), mpVec->at(mElementCounter));
        mElementCounter++;
    }
}

void BackConverter::visit(const VectorValueInfo& info) {
   
    // If there is a content pointer, it means this is a container. 
    if (info.content.get()) {
        
        const Typelib::Container& t = 
            static_cast<const Typelib::Container&>(*mrRegistry.get(info.containerType));
        void* ptr = getPosition(info);
        unsigned int ecnt = t.getElementCount( ptr );
        if ( ecnt == 0 ) return;
        
        unsigned int esize = t.getIndirection().getSize();
        std::vector<uint8_t>* vector_ptr = 
            reinterpret_cast<std::vector<uint8_t>*>( ptr );
        mBaseStack.push_back(&(*vector_ptr)[0]);
        mContainersSizeStack.push_back(0);
       
        int istar;
        if (mpMatcher) {
            mPlaceStack.push_back(info.placeDescription);
            istar = mPlaceStack.back().size()-1;
        }
        
        for ( int i=0; i<ecnt; i++) {
            if (mpMatcher) {
                mPlaceStack.back().replace(mPlaceStack.back().begin()+istar, 
                        mPlaceStack.back().end(), 
                        boost::lexical_cast<std::string,int>(i) );
            }
            VectorTocVisitor::visit(*(info.content));
            mContainersSizeStack.back() += esize;
        }
        
        if (mpMatcher) mPlaceStack.pop_back();
        mContainersSizeStack.pop_back();
        mBaseStack.pop_back(); 
    }
    else setElement(info); 
}
