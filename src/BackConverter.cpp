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
}

void FlatBackConverter::visit(const VectorValueInfo& info) {
}

void BackConverter::visit(const VectorValueInfo& info) {
}
