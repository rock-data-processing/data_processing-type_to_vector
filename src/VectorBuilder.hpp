/**
 * \file  VectorBuilder.hpp
 *
 * \brief Builds vectors from different data sources. 
 *
 *      German Research Center for Artificial Intelligence\n
 *      Project: VirGo4
 *
 * \date 13.11.2012
 *
 * \author felix.rehrmann@dfki.de
 */

#ifndef TYPETOVECTOR_VECTORBUILDER_HPP
#define TYPETOVECTOR_VECTORBUILDER_HPP

#include <vector>
#include <string>

#include "Definitions.hpp"
#include "Converter.hpp"

namespace type_to_vector {

typedef std::vector<AbstractConverter::Pointer> Converters;
typedef std::vector<VectorOfDoubles> DataVectors;

struct VectorPosition {
    int start;
    int end;

    VectorPosition () : start(0), end(-1) {}
    VectorPosition (int s, int e) : start(s), end(e) {}

    bool empty() { return end == start-1; }
};

/** Holder of all information and data of a conversion. */
class VectorConversion {
    std::string mIdentifier;

    Converters mConverters;
    DataVectors mData;

public:
    VectorConversion (std::string name) : mIdentifier(name) {}
    VectorConversion () : mIdentifier("") {}

    int addConverter(AbstractConverter::Pointer converter_ptr);

    void update (const Typelib::Value& value, bool create_places=false);

    void update ( void* data, bool create_places=false);
    
    void update ( int converter_idx, void* data, bool create_places=false);

    const VectorOfDoubles& getData(int idx) const;

    const VectorToc& getToc(int idx) const { return mConverters.at(idx)->getToc(); }

    StringVector getPlaces(int idx) const;

    int size() const { return mConverters.size(); }

    void setName(const std::string& name) { mIdentifier = name; }
    std::string name() const { return mIdentifier; }
    std::string getTypeName() const { return mConverters.back()->getTypeName(); }
};

/** Builds vector from several types. */
class DataVectorBuilder : public std::vector<VectorConversion> {

    VectorOfDoubles mStore;

public:
    /** Updates all vectors. */
    void update(int vector_idx, void* data, bool create_places=false);

    /** Only update vectors for a certain converter. */
    void update(int converter_idx, int vector_idx, void* data, bool create_places=false);

    const VectorOfDoubles& getVector(int converter_idx);
    Eigen::VectorXd getEigenVector(int converter_idx);
    
    template<typename Derived>
    bool getEigenVector(int converter_idx, Eigen::DenseBase<Derived>& vector){
    
        const VectorOfDoubles& vec = getVector(converter_idx);

        if (!vec.empty()) {
            vector = Eigen::Map<const Eigen::VectorXd>((&vec[0]), vec.size());
            return true;
        }
        else return false; 
    }

    StringVector getPlaces(int converter_idx) const;

    VectorPosition getVectorPosition(int converter_idx, int vector_idx) const;

    int getVectorIdx(std::string vector_id) const;

    int getVectorSize(int conveter_idx) const;
};

}

#endif // TYPETOVECTOR_VECTORBUILDER_HPP


