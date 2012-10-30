/**
 * \file  SliceMatcher.hpp
 *
 * \brief Matches a position in a type to a slice description.
 *
 *      German Research Center for Artificial Intelligence\n
 *      Project: VirGo4
 *
 * \date 30.10.2012
 *
 * \author felix.rehrmann@dfki.de
 */

#ifndef GENERALPROCESSING_SLICEMATCHER_HPP
#define GENERALPROCESSING_SLICEMATCHER_HPP

#include <set>

#include <utilmm/stringtools.hh>

#include "Definitions.hpp"

namespace general_processing {

typedef std::set<unsigned int> IntSet;

/** Takes a slice and store it in a way that a match can be find easily. 
 *
 * Slice define what part of a vector shall be put into the vector.
 * Slices describe the place in the type. Levels are separated by space.
 * Countable types like arrays or containers can be sliced with the valid indicies 
 * in brackets:
 * all indices: *
 * element at index 2: 2
 * range 1 - 10: [1-10]
 * range 2-30, every 2nd: [2-30:2] (every xth work only together with a range)
 * elements 0, 3 and 9: [0,3,9]
 * mix: [0,3-9,12,20-100:5]
 *
 * Assume:
 * struct A {
 *  double a[3];
 *  int b;
 *  char c;
 * };
 * struct B {
 *  int idx;
 *  A data[5];
 * };
 *
 * get everything of B: ""
 * get everything in B.data: "data"
 * get b of first and third B.data: "data.[0,2].b"
 * get idx and b of B.data: "idx data.b"
 * get field c of all B.data: "data.*.c"
 * */
class SliceStore {

    StringVector mPlaces; //!< Valid places;
    IntSet mStarTokens; //!< Tokens indices at which stars are found.

public:
    
    struct IndexSlice {
        int from;
        int to;
        int every;
        IndexSlice() : from(-1), to(-1), every(1) {}
    };


    SliceStore (const std::string& slice);
   

    bool empty () { return mPlaces.empty(); }

    /** A const reference to the stored places. */
    const StringVector& getPlaces () const { return mPlaces; }

    /** A set of indices at which a star is in the slice description. */
    const IntSet& getStarTokens () const { return mStarTokens; }
 

    /** Put an index slice string into an IndexSlice struct. */
    static IndexSlice getIndices (const std::string& slice);

    /** Resolves an indices slice to the list of valid indices.
     *
     * e.g. "[1-3]" -> "1", "2", "3" 
     * \returns empty vector if it was no slice. */
    static StringVector resolveIndices (const std::string& str);

    /** Creates all possible slices form index descriptons for the first index slice.
     *
     * e.g. "d.[1-2].a.[2,5] -> "d.1.a.[2,5]" "d.2.a.[2,5]" 
     * \param star_toks the token indices of stars will be added here. */
    static StringVector makeDirectIndexSlice (const std::string& str, IntSet& star_toks);

    /** Replaces each string with index slices with multiple strings for the i
     *  allowed indices. 
     */
    static utilmm::stringlist replaceIndicesSlices (const std::string& str,
            IntSet& star_toks);
};

/** The slice matcher checks whether a place description fits one of its slices. */
class SliceMatcher {

    SliceStore mSlices;

public:

    SliceMatcher(const std::string& slice) : mSlices(slice) {}
    
    /** Checks whether a place fits at least one slice or not. */
    bool fitsASlice(const std::string& place);
    
    const SliceStore& getSlices() const { return mSlices; }

    /** For numeric tokens a string with a * instead is add, plus the combinations. */
    static StringVector createGeneralPlaces (const std::string& place, size_t start=0);

    static bool startswith (const std::string& str, const std::string& start);

    static bool isInteger (const std::string& str);

};

} // namespace general_processing

#endif //  GENERALPROCESSING_SLICEMATCHER_HPP

