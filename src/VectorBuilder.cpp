// \file  VectorBuilder.cpp

#include <stdexcept>

#include "VectorBuilder.hpp"

using namespace general_processing;

int VectorConversion::addConverter (AbstractConverter::Pointer converter_ptr) {

    if ( !mConverters.empty() && 
            converter_ptr->getTypeName() != mConverters.back()->getTypeName() )
        throw std::runtime_error("all converters need to be for the same type");

    mConverters.push_back(converter_ptr);
    mData.push_back(VectorOfDoubles());
    return size();
}

void VectorConversion::update (const Typelib::Value& value, bool create_places) {

    if ( value.getType().getName() != mConverters.back()->getTypeName() )
        std::runtime_error("value and converter type does not match");

    update(value.getData(), create_places);
}

void VectorConversion::update (void* data, bool create_places) {
    
    Converters::iterator cit = mConverters.begin();
    DataVectors::iterator dit = mData.begin();

    for ( ; cit != mConverters.end(); cit++, dit++)
        *dit = (*cit)->apply(data, create_places);
}

const VectorOfDoubles& VectorConversion::getData(int idx) const {

    return mData.at(idx);
}

StringVector VectorConversion::getPlaces(int idx) const {

    return mConverters.at(idx)->getPlaceVector();
}

void DataVectorBuilder::update (int vector_idx, void* data, bool create_places) {

    at(vector_idx).update(data,create_places);
}


VectorOfDoubles DataVectorBuilder::getVector (int converter_idx) const {

    const_iterator it = begin();
    int size = 0;

    for ( ; it != end(); it++)
        size += it->getData(converter_idx).size();

    VectorOfDoubles result;
    result.reserve(size);

    for ( it = begin(); it != end(); it++) {
        const VectorOfDoubles& vec = it->getData(converter_idx);
        result.insert(result.end(), vec.begin(), vec.end());
    }

    return result; 
}

Eigen::MatrixXd DataVectorBuilder::getEigenVector (int converter_idx) const {
    
    Eigen::VectorXd result;
    VectorOfDoubles vec;

    vec = getVector(converter_idx);

    if (!vec.empty()) 
        result = Eigen::Map<Eigen::VectorXd>(&(vec[0]), vec.size());

    return result;
}

StringVector DataVectorBuilder::getPlaces(int converter_idx) const {
    
    int size = 0;
    
    const_iterator it = begin();

    for ( ; it != end(); it++)
        size += it->getData(converter_idx).size();

    StringVector result;
    result.resize(size, std::string(20,' '));

    int i = 0;
    for ( it = begin(); it != end(); it++) {

        StringVector vec = it->getPlaces(converter_idx);
        StringVector::const_iterator sit = vec.begin();
        for (; sit != vec.end(); sit++)
            if ( *sit == "" )
                result[i++] = it->name();
            else
                result[i++] = it->name() + "." + *sit;
    }

    return result; 
}

VectorPosition DataVectorBuilder::getVectorPosition(int converter_idx, 
        int vector_idx) const {

    VectorPosition pos;

    pos.start = 0;
    
    int i;
    for (i=0; i<vector_idx; i++ )
        pos.start += at(i).getData(converter_idx).size();

    pos.end = pos.start + at(i).getData(converter_idx).size() - 1;

    return pos;
}

int DataVectorBuilder::getVectorIdx (std::string vector_id) const {

    const_iterator it = begin();

    for ( int i=0; it != end(); it++, i++)
        if ( it->name() == vector_id ) return i;

    return -1;
}
