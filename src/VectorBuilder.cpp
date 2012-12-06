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

void VectorConversion::update (int converter_idx, void* data, bool create_places) {
    
    mData.at(converter_idx) = mConverters.at(converter_idx)->apply(data, create_places);
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

void DataVectorBuilder::update (int converter_idx, int vector_idx, void* data, 
        bool create_places) {

    at(vector_idx).update(converter_idx, data,create_places);
}


const VectorOfDoubles& DataVectorBuilder::getVector (int converter_idx) {

    mStore.clear();
    mStore.reserve(getVectorSize(converter_idx));

    for ( const_iterator it = begin(); it != end(); it++) {
        const VectorOfDoubles& vec = it->getData(converter_idx);
        mStore.insert(mStore.end(), vec.begin(), vec.end());
    }

    return mStore; 
}

Eigen::VectorXd DataVectorBuilder::getEigenVector (int converter_idx) {
    
    Eigen::VectorXd result;

    getEigenVector(converter_idx,result);

    return result;
}

StringVector DataVectorBuilder::getPlaces(int converter_idx) const {
    
    int size = getVectorSize(converter_idx);
    
    const_iterator it = begin();

    StringVector result;
    result.resize(size, std::string(20,' '));

    int i = 0;
    for ( ; it != end(); it++) {

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

int DataVectorBuilder::getVectorSize(int converter_idx) const {
    const_iterator it = begin();
    int size = 0;

    for ( ; it != end(); it++)
        size += it->getData(converter_idx).size();

    return size;
}
