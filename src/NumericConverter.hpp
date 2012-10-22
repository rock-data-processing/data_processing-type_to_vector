/**
 * \file  NumericConverter.hpp
 *
 * \brief Converts scalar typelib values to a double; 
 *
 *      German Research Center for Artificial Intelligence\n
 *      Project: VirGo4
 *
 * \date 22.10.2012
 *
 * \author felix.rehrmann@dfki.de
 */

#ifndef GENERALPROCESSING_NUMERIVCONVERTER_HPP
#define GENERALPROCESSING_NUMERIVCONVERTER_HPP

#include <typelib/typemodel.hh>

namespace general_processing {

typedef double (*CastFunction)(void*);


template <typename T>
static double cast(void* data) {
    return double(*reinterpret_cast<T*>(data));
}


template <Typelib::Enum>
static double cast(void* data) {
    return cast<Typelib::Enum::integral_type>();
}


template <Typelib::NullType>
static double cast(void* data) { return 0.0; }


/** Determines the right cast function for a typelib type. */
static CastFunction getCastFunction(Typelib::Type const& type) {

    switch(type.getCategory) {

    case Typelib::Numeric:
	switch(type->getNumericCategory()) {
	case Typelib::Numeric::SInt:
		switch(type->getSize()) {
		case 1: return &cast<int8_t>;
		case 2: return &cast<int16_t>;
		case 4: return &cast<int32_t>;
		case 8: return &cast<int64_t>;
		default:
			return 0;
		}

	case Typelib::Numeric::UInt:
		switch(type->getSize()) {
		case 1: return &cast<uint8_t>;
		case 2: return &cast<uint16_t>;
		case 4: return &cast<uint32_t>;
		case 8: return &cast<uint64_t>;
		default:
			return 0;
		}

	case Typelib::Numeric::Float:
		switch(type->getSize()) {
		case sizeof(float): return &cast<float>;
		case sizeof(double): return &cast<double>;
		case sizeof(long double): return &cast<long double>;
		default:
			return 0;
		}
	default:
            return &cast<Typelib::NullType>;
	}

    case Typelib::Enum:
        return &cast<Typelib::Enum>;

    default:
        return &cast<Typelib::NullType>;
    }
}

} // namespace general_processing

#endif // GENERALPROCESSING_NUMERIVCONVERTER_HPP

