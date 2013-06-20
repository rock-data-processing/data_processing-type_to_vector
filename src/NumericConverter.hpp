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

#ifndef TYPETOVECTOR_NUMERIVCONVERTER_HPP
#define TYPETOVECTOR_NUMERIVCONVERTER_HPP

#include <typelib/typemodel.hh>

namespace type_to_vector {

/* The function signature for cast functions. */
typedef double (*CastFunction)(void*);
/* The function signature for back cast functions. */
typedef void (*BackCastFunction)(void*, double);

template <typename T>
static double cast(void* data) {
    return double(*reinterpret_cast<T*>(data));
}

static double castEnum(void* data) {
    return cast<Typelib::Enum::integral_type>(data);
}

static double castNull(void* data) { return 0.0; }

template <typename T>
static void backCast(void* data, double value) {
    *reinterpret_cast<T*>(data) = T(value);
}

static void backCastEnum(void* data, double value) {
    backCast<Typelib::Enum::integral_type>(data, value);
}

/** The right cast function for numeric types.  */
static CastFunction getNumericCastFunction(Typelib::Numeric const& type) {
    switch(type.getNumericCategory()) {

    case Typelib::Numeric::SInt:
        switch(type.getSize()) {
        case 1: return &cast<int8_t>;
        case 2: return &cast<int16_t>;
        case 4: return &cast<int32_t>;
        case 8: return &cast<int64_t>;
        default:
            return 0;
        }

    case Typelib::Numeric::UInt:
        switch(type.getSize()) {
        case 1: return &cast<uint8_t>;
        case 2: return &cast<uint16_t>;
        case 4: return &cast<uint32_t>;
        case 8: return &cast<uint64_t>;
        default:
            return 0;
        }

    case Typelib::Numeric::Float:
        switch(type.getSize()) {
        case sizeof(float): return &cast<float>;
        case sizeof(double): return &cast<double>;
        case sizeof(long double): return &cast<long double>;
        default:
            return 0;
        }
    default:
        return 0;
    }
}

/** Determines the right cast function for a typelib type. */
static CastFunction getCastFunction(Typelib::Type const& type) {

    switch(type.getCategory()) {

    case Typelib::Type::Numeric:
        return getNumericCastFunction(static_cast<Typelib::Numeric const&>(type));

    case Typelib::Type::Enum:
        return &castEnum;

    case Typelib::Type::NullType:
        return &castNull;

    default:
        return 0;
    }
}

/** The right back cast function for numeric types.  */
static BackCastFunction getNumericBackCastFunction(Typelib::Numeric const& type) {
    switch(type.getNumericCategory()) {

    case Typelib::Numeric::SInt:
        switch(type.getSize()) {
        case 1: return &backCast<int8_t>;
        case 2: return &backCast<int16_t>;
        case 4: return &backCast<int32_t>;
        case 8: return &backCast<int64_t>;
        default:
            return 0;
        }

    case Typelib::Numeric::UInt:
        switch(type.getSize()) {
        case 1: return &backCast<uint8_t>;
        case 2: return &backCast<uint16_t>;
        case 4: return &backCast<uint32_t>;
        case 8: return &backCast<uint64_t>;
        default:
            return 0;
        }

    case Typelib::Numeric::Float:
        switch(type.getSize()) {
        case sizeof(float): return &backCast<float>;
        case sizeof(double): return &backCast<double>;
        case sizeof(long double): return &backCast<long double>;
        default:
            return 0;
        }
    default:
        return 0;
    }
}

/** Determines the right back cast function for a typelib type. */
static BackCastFunction getBackCastFunction(Typelib::Type const& type) {

    switch(type.getCategory()) {

    case Typelib::Type::Numeric:
        return getNumericBackCastFunction(static_cast<Typelib::Numeric const&>(type));

    case Typelib::Type::Enum:
        return &backCastEnum;

    default:
        return 0;
    }
}

} // namespace type_to_vector

#endif // TYPETOVECTOR_NUMERIVCONVERTER_HPP

