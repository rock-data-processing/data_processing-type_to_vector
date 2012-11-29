// \file  SliceMatcher.cpp

#include <set>

#include <boost/lexical_cast.hpp>

#include "SliceMatcher.hpp"

using namespace general_processing;

SliceStore::SliceStore (const std::string& slice, bool general) {
  
    mInverseSlices = slice[0] == '!';

    utilmm::stringlist places;

    if ( mInverseSlices )
        places = utilmm::split(slice.substr(1), " ");
    else
        places = utilmm::split(slice," ");

    utilmm::stringlist::const_iterator it = places.begin();

    typedef std::set<std::string> SliceSet;
    SliceSet sset;
    
    for ( ; it != places.end(); it++ ) {

        utilmm::stringlist concrete_places = replaceIndicesSlices(*it, general);

        utilmm::stringlist::const_iterator pit = concrete_places.begin();

        for (; pit != concrete_places.end(); pit++ )
            sset.insert((*pit)+"."); // stop with a . to not 1 matching 10
    }

    mPlaces = StringVector(sset.begin(), sset.end());
}


SliceStore::IndexSlice SliceStore::getIndices( const std::string& slice ) {
    
    IndexSlice idx_slice;

    if (slice == "") return idx_slice;

    size_t minus_pos = slice.find("-");

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

        idx_slice.from = boost::lexical_cast<int>(slice);
        idx_slice.to = idx_slice.from;
    }

    return idx_slice; 
}

StringVector SliceStore::resolveIndices (const std::string& str, bool general) {
    
    if ( str[0] != '[' ) return StringVector();

    int n = str.length();

    if ( str[n-1] != ']' ) throw std::runtime_error("index slice must be in []");
    
    StringVector indices;

    if (general) {
        indices.push_back("*");
        return indices;
    }

    std::string slice = str.substr(1,n-2);

    utilmm::stringlist slices_list = utilmm::split(slice, ",");

    if (slices_list.empty()) throw std::runtime_error("empty slice");

    utilmm::stringlist::const_iterator it = slices_list.begin();

    std::vector<IndexSlice> slices;

    for ( ; it != slices_list.end(); it++ )
        slices.push_back(getIndices(*it));

    std::vector<IndexSlice>::const_iterator sit = slices.begin();

    for ( ; sit != slices.end(); sit++ ) {

        for ( int i=sit->from; i <= sit->to; i += sit->every )
            indices.push_back( boost::lexical_cast<std::string,int>(i) );
    }

    return indices;
}

StringVector SliceStore::makeDirectIndexSlice(const std::string& str, bool general) {
            
    utilmm::stringlist tokens = utilmm::split(str, ".");

    utilmm::stringlist::iterator jt = tokens.begin();
    
    StringVector result;

    unsigned int tok_idx = 0;

    for ( ; jt != tokens.end(); jt++, tok_idx++ ) {

        StringVector indices = resolveIndices (*jt, general);

        if ( !indices.empty() ) {

            StringVector::const_iterator iit = indices.begin();

            for ( ; iit != indices.end(); iit++) {

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

utilmm::stringlist SliceStore::replaceIndicesSlices(const std::string& str, bool general) {

    bool replaced = true;

    utilmm::stringlist str_list;
    str_list.push_back(str);
    
    while (replaced) {

        replaced = false;
    
        utilmm::stringlist::iterator it = str_list.begin();

        for ( ; it != str_list.end(); it++) {

            StringVector replace_vector = makeDirectIndexSlice(*it, general);

            if ( replace_vector.size() > 0 && replace_vector.back() != *it ) {

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

    std::string place_dot = place + ".";

    StringVector places = createGeneralPlaces(place_dot);
    StringVector slices = mSlices.getPlaces();

    if ( slices.empty() ) return !mSlices.isInverse();

    StringVector::const_iterator it_places = places.begin();

    for ( ; it_places != places.end(); it_places++ ) {
        
        StringVector::const_iterator it_slices = slices.begin();

        for ( ; it_slices != slices.end(); it_slices++ )

            if ( startswith(*it_places, *it_slices) ) return !mSlices.isInverse();
    }

    return mSlices.isInverse();
}

StringVector SliceMatcher::createGeneralPlaces (const std::string& place, size_t start) {

    StringVector result;
    result.push_back(place);

    size_t from = start;

    while (from < place.length()) {
        
        size_t to = place.find(".", from);

        if ( to == std::string::npos ) to = place.length();

        if ( isInteger(place.substr(from,to-from)) ) {

            std::string star_place(place);
            star_place.replace(from,to-from,"*"); 

            StringVector places = createGeneralPlaces(place, to+1);
            StringVector places_star = createGeneralPlaces(star_place, from+2);

            for ( StringVector::const_iterator it = places.begin()+1;
                    it != places.end(); it++)

                result.push_back(*it);
           

            for ( StringVector::const_iterator it = places_star.begin(); 
                    it != places_star.end(); it++ )

                result.push_back(*it);
            
            return result;
        } 

        from = to+1;
    }

    return result;
}


bool SliceMatcher::startswith (const std::string& str, const std::string& start) {

    if ( str.length() < start.length() ) return false;

    for ( int i=0; i < start.length(); i++ )
        if ( str[i] != start[i] ) return false;

    return true;
}
    
bool SliceMatcher::isInteger (const std::string& str) {

    try {

        boost::lexical_cast<int>(str);

        return true;

    } catch (boost::bad_lexical_cast&) {}

    return false;
}


