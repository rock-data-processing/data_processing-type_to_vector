// \file  SliceMatcher.cpp

#include "SliceMatcher.hpp"

using namespace general_processing;

SliceStore::SliceStore (const std::string& slice) {
    
    utilmm::stringlist places = utilmm::split(slice," ");

    utilmm::stringlist::const_iterator it = places.begin();
    
    for ( ; it != places.end(); it++ ) {

        utilmm::stringlist concrete_places = replaceIndicesSlices(*it, mStarTokens);

        utilmm::stringlist::const_iterator pit = concrete_places.begin();

        for (; pit != concrete_places.end(); pit++ )
            mPlaces.push_back(*pit);
    }
}


SliceStore::IndexSlice SliceStore::getIndices( const std::string& slice ) {
    
    size_t minus_pos = slice.find("-");
    IndexSlice idx_slice;

    if ( minus_pos != std::string::npos ) {

        idx_slice.from = 
            boost::lexical_cast<int>(slice.substr(0, minus_pos));
    
        size_t colon_pos = slice.find(":", minus_pos);

        if ( colon_pos != std::string::npos ) {

            idx_slice.to = boost::lexical_cast<int>(slice.substr(minus_pos+1,
                        colon_pos-minus_pos-1));

            idx_slice.every = boost::lexical_cast<int>(slice.substr(colon_pos+1,
                            slice.length()-colon_pos-1));

        } else {
            
            idx_slice.to = boost::lexical_cast<int>(slice.substr(minus_pos+1, 
                            slice.length()-minus_pos-1));
        }

    } else {

        // single index
        idx_slice.from = boost::lexical_cast<int>(slice);
        idx_slice.to = idx_slice.from;     
    }

    return idx_slice; 
}

StringVector SliceStore::resolveIndices (const std::string& str) {
    
    if ( str[0] != '[' ) return StringVector();

    int n = str.length();

    if ( str[n-1] != ']' ) throw std::runtime_error("index slice must be in []");

    std::string slice = str.substr(1,n-2);

    utilmm::stringlist slices_list = utilmm::split(slice, ",");

    utilmm::stringlist::const_iterator it = slices_list.begin();

    std::vector<IndexSlice> slices;

    for ( ; it != slices_list.end(); it++ )
       slices.push_back(getIndices(*it));

    std::vector<IndexSlice>::const_iterator sit = slices.begin();

    StringVector indices;

    for ( ; sit != slices.end(); sit++ ) {

        for ( int i=sit->from; i <= sit->to; i += sit->every )

            indices.push_back( boost::lexical_cast<std::string,int>(i) );

    }

    return indices;

}

StringVector SliceStore::makeDirectIndexSlice(const std::string& str, IntSet& star_toks ) {
            
    utilmm::stringlist tokens = utilmm::split(str, ".");

    utilmm::stringlist::iterator jt = tokens.begin();
    
    StringVector result;

    unsigned int tok_idx = 0;

    for ( ; jt != tokens.end(); jt++, tok_idx++ ) {

        if ( *jt == "*" ) {
            star_toks.insert(tok_idx);
            continue;
        }

        StringVector indices = resolveIndices (*jt);

        if ( !indices.empty() ) {

            StringVector::const_iterator iit = indices.begin();

            for ( ; iit<indices.end(); iit++) {

                utilmm::stringlist::iterator jt_new = tokens.insert(jt,*iit);

                tokens.erase(jt);

                jt = jt_new;

                result.push_back( utilmm::join(tokens, ".") ); 
            }

            return result;
        }
    }

    // no slicing needed
    result.push_back(str);
    return result;
}

utilmm::stringlist SliceStore::replaceIndicesSlices(const std::string& str, 
        IntSet& star_toks) {

    bool replaced = true;

    utilmm::stringlist str_list;
    str_list.push_back(str);
    
    while (replaced) {

        replaced = false;
    
        utilmm::stringlist::iterator it = str_list.begin();

        for ( ; it != str_list.end(); it++) {

            StringVector replace_vector = makeDirectIndexSlice(*it, star_toks);

            if ( replace_vector.size() > 1 && replace_vector.back() != *it ) {

                replaced = true;

                utilmm::stringlist::iterator it_next = it;
                it_next++;
                
                str_list.insert(it,replace_vector.begin(),replace_vector.end());

                str_list.erase(it);

                it = it_next; 
            } 
        }
    }

    return str_list;
}



bool SliceMatcher::fitsASlice (const std::string& place) {
    return false;
}

StringVector SliceMatcher::createGeneralPlaces (const std::string& place) {

    return StringVector();
}


bool SliceMatcher::startswith (const std::string& str, const std::string& start) {

    if ( str.length() < start.length() ) return false;

    for ( int i=0; i < start.length(); i++ )
        if ( str[i] != start[i] ) return false;

    return true;
}
