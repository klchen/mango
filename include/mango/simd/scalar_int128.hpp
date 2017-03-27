/*
    MANGO Multimedia Development Platform
    Copyright (C) 2012-2017 Twilight Finland 3D Oy Ltd. All rights reserved.
*/
#pragma once

#include "simd.hpp"

namespace mango {
namespace simd {

    // -----------------------------------------------------------------
    // helpers
    // -----------------------------------------------------------------

    template <typename ScalarType, int Size>
    static inline scalar_type<ScalarType, Size>
    scalar_set(ScalarType value)
    {
        scalar_type<ScalarType, Size> v;
        for (int i = 0; i < Size; ++i) {
            v[i] = value;
        }
        return v;
    }

    // unary

    template <typename ScalarType, int Size>
    static inline scalar_type<ScalarType, Size>
    scalar_unroll(ScalarType (*func)(ScalarType), scalar_type<ScalarType, Size> a)
    {
        scalar_type<ScalarType, Size> v;
        for (int i = 0; i < Size; ++i) {
            v[i] = func(a[i]);
        }
        return v;
    }

    template <typename ScalarType>
    static inline ScalarType scalar_abs(ScalarType a)
    {
        return std::abs(a);
    }

    template <typename ScalarType>
    static inline ScalarType scalar_neg(ScalarType a)
    {
        return -a;
    }

    // binary

    template <typename ScalarType, int Size>
    static inline scalar_type<ScalarType, Size>
    scalar_unroll(ScalarType (*func)(ScalarType, ScalarType), scalar_type<ScalarType, Size> a, scalar_type<ScalarType, Size> b)
    {
        scalar_type<ScalarType, Size> v;
        for (int i = 0; i < Size; ++i) {
            v[i] = func(a[i], b[i]);
        }
        return v;
    }

    template <typename ScalarType>
    static inline ScalarType scalar_add(ScalarType a, ScalarType b)
    {
        return a + b;
    }

    template <typename ScalarType>
    static inline ScalarType scalar_sub(ScalarType a, ScalarType b)
    {
        return a - b;
    }

    template <typename ScalarType>
    static inline ScalarType scalar_mullo(ScalarType a, ScalarType b)
    {
        return ScalarType(a * b);
    }

    template <typename ScalarType>
    static inline ScalarType scalar_unsigned_adds(ScalarType a, ScalarType b)
    {
	    ScalarType v = a + b;
	    v |= -(v < a);
	    return v;
    }

    template <typename ScalarType>
    static inline ScalarType scalar_unsigned_subs(ScalarType a, ScalarType b)
    {
    	ScalarType v = a - b;
	    v &= -(v <= a);
	    return v;
    }

    template <typename ScalarType>
    static inline ScalarType scalar_signed_adds(ScalarType a, ScalarType b)
    {
      	typedef typename std::make_unsigned<ScalarType>::type UnsignedScalarType;
        UnsignedScalarType x = a;
		UnsignedScalarType y = b;
		UnsignedScalarType v = x + y;

  	    // overflow
	    x = (x >> (sizeof(ScalarType) * 8 - 1)) + std::numeric_limits<ScalarType>::max();
	    if (ScalarType((x ^ y) | ~(y ^ v)) >= 0)
	    {
		    v = x;
	    }

	    return v;
    }

    template <typename ScalarType>
    static inline ScalarType scalar_signed_subs(ScalarType a, ScalarType b)
    {
      	typedef typename std::make_unsigned<ScalarType>::type UnsignedScalarType;
        UnsignedScalarType x = a;
        UnsignedScalarType y = b;
        UnsignedScalarType v = x - y;

  	    // overflow
	    x = (x >> (sizeof(ScalarType) * 8 - 1)) + std::numeric_limits<ScalarType>::max();
	    if (ScalarType((x ^ y) & (x ^ v)) < 0)
	    {
		    v = x;
	    }

	    return v;
    }

    template <typename ScalarType>
    static inline ScalarType scalar_and(ScalarType a, ScalarType b)
    {
        return a & b;
    }

    template <typename ScalarType>
    static inline ScalarType scalar_nand(ScalarType a, ScalarType b)
    {
        return ~a & b;
    }

    template <typename ScalarType>
    static inline ScalarType scalar_or(ScalarType a, ScalarType b)
    {
        return a | b;
    }

    template <typename ScalarType>
    static inline ScalarType scalar_xor(ScalarType a, ScalarType b)
    {
        return a ^ b;
    }

    template <typename ScalarType>
    static inline ScalarType scalar_compare_eq(ScalarType a, ScalarType b)
    {
        return a == b ? ~0 : 0;
    }

    template <typename ScalarType>
    static inline ScalarType scalar_compare_gt(ScalarType a, ScalarType b)
    {
        return a > b ? ~0 : 0;
    }

    template <typename ScalarType>
    static inline ScalarType scalar_min(ScalarType a, ScalarType b)
    {
        return std::min(a, b);
    }

    template <typename ScalarType>
    static inline ScalarType scalar_max(ScalarType a, ScalarType b)
    {
        return std::max(a, b);
    }

    // misc

    template <typename ScalarType, int Size>
    static inline scalar_type<ScalarType, Size>
    scalar_select(scalar_type<ScalarType, Size> mask, scalar_type<ScalarType, Size> a, scalar_type<ScalarType, Size> b)
    {
        scalar_type<ScalarType, Size> v;
        for (int i = 0; i < Size; ++i) {
            v[i] = (mask[i] & a[i]) | (~mask[i] & b[i]);
        }
        return v;
    }

    template <typename VectorType>
    static inline VectorType scalar_unpacklo(VectorType a, VectorType b)
    {
        VectorType v;
        for (int i = 0; i < VectorType::size / 2; ++i)
        {
            v[i * 2 + 0] = a[i];
            v[i * 2 + 1] = b[i];
        }
        return v;
    }

    template <typename VectorType>
    static inline VectorType scalar_unpackhi(VectorType a, VectorType b)
    {
        VectorType v;
        constexpr int half = VectorType::size / 2;
        for (int i = 0; i < half; ++i)
        {
            v[i * 2 + 0] = a[i + half];
            v[i * 2 + 1] = b[i + half];
        }
        return v;
    }

    // -----------------------------------------------------------------
    // uint8x16
    // -----------------------------------------------------------------

    static inline uint8x16 uint8x16_zero()
    {
        return scalar_set<uint8, 16>(0);
    }

    static inline uint8x16 uint8x16_set1(uint8 s)
    {
        return scalar_set<uint8, 16>(s);
    }

    static inline uint8x16 unpacklo(uint8x16 a, uint8x16 b)
    {
        return scalar_unpacklo(a, b);
    }

    static inline uint8x16 unpackhi(uint8x16 a, uint8x16 b)
    {
        return scalar_unpackhi(a, b);
    }

    static inline uint8x16 add(uint8x16 a, uint8x16 b)
    {
        return scalar_unroll(scalar_add, a, b);
    }

    static inline uint8x16 sub(uint8x16 a, uint8x16 b)
    {
        return scalar_unroll(scalar_sub, a, b);
    }

    static inline uint8x16 mullo(uint8x16 a, uint8x16 b)
    {
        return scalar_unroll(scalar_mullo, a, b);
    }

    // saturated

    static inline uint8x16 adds(uint8x16 a, uint8x16 b)
    {
        return scalar_unroll(scalar_unsigned_adds, a, b);
    }

    static inline uint8x16 subs(uint8x16 a, uint8x16 b)
    {
        return scalar_unroll(scalar_unsigned_subs, a, b);
    }

    // bitwise

    static inline uint8x16 uint8x16_and(uint8x16 a, uint8x16 b)
    {
        return scalar_unroll(scalar_and, a, b);
    }

    static inline uint8x16 uint8x16_nand(uint8x16 a, uint8x16 b)
    {
        return scalar_unroll(scalar_nand, a, b);
    }

    static inline uint8x16 uint8x16_or(uint8x16 a, uint8x16 b)
    {
        return scalar_unroll(scalar_or, a, b);
    }

    static inline uint8x16 uint8x16_xor(uint8x16 a, uint8x16 b)
    {
        return scalar_unroll(scalar_xor, a, b);
    }

    // compare

    static inline uint8x16 compare_eq(uint8x16 a, uint8x16 b)
    {
        return scalar_unroll(scalar_compare_eq, a, b);
    }

    static inline uint8x16 compare_gt(uint8x16 a, uint8x16 b)
    {
        return scalar_unroll(scalar_compare_gt, a, b);
    }

    static inline uint8x16 select(uint8x16 mask, uint8x16 a, uint8x16 b)
    {
        return scalar_select(mask, a, b);
    }

    static inline uint8x16 min(uint8x16 a, uint8x16 b)
    {
        return scalar_unroll(scalar_min, a, b);
    }

    static inline uint8x16 max(uint8x16 a, uint8x16 b)
    {
        return scalar_unroll(scalar_max, a, b);
    }

    // -----------------------------------------------------------------
    // uint16x8
    // -----------------------------------------------------------------

    static inline uint16x8 uint16x8_zero()
    {
        return scalar_set<uint16, 8>(0);
    }

    static inline uint16x8 uint16x8_set1(uint16 s)
    {
        return scalar_set<uint16, 8>(s);
    }

    static inline uint16x8 unpacklo(uint16x8 a, uint16x8 b)
    {
        return scalar_unpacklo(a, b);
    }

    static inline uint16x8 unpackhi(uint16x8 a, uint16x8 b)
    {
        return scalar_unpackhi(a, b);
    }

    static inline uint16x8 add(uint16x8 a, uint16x8 b)
    {
        return scalar_unroll(scalar_add, a, b);
    }

    static inline uint16x8 sub(uint16x8 a, uint16x8 b)
    {
        return scalar_unroll(scalar_sub, a, b);
    }

    static inline uint16x8 mullo(uint16x8 a, uint16x8 b)
    {
        return scalar_unroll(scalar_mullo, a, b);
    }

    // saturated

    static inline uint16x8 adds(uint16x8 a, uint16x8 b)
    {
        return scalar_unroll(scalar_unsigned_adds, a, b);
    }

    static inline uint16x8 subs(uint16x8 a, uint16x8 b)
    {
        return scalar_unroll(scalar_unsigned_subs, a, b);
    }

    // bitwise

    static inline uint16x8 uint16x8_and(uint16x8 a, uint16x8 b)
    {
        return scalar_unroll(scalar_and, a, b);
    }

    static inline uint16x8 uint16x8_nand(uint16x8 a, uint16x8 b)
    {
        return scalar_unroll(scalar_nand, a, b);
    }

    static inline uint16x8 uint16x8_or(uint16x8 a, uint16x8 b)
    {
        return scalar_unroll(scalar_or, a, b);
    }

    static inline uint16x8 uint16x8_xor(uint16x8 a, uint16x8 b)
    {
        return scalar_unroll(scalar_xor, a, b);
    }

    // compare

    static inline uint16x8 compare_eq(uint16x8 a, uint16x8 b)
    {
        return scalar_unroll(scalar_compare_eq, a, b);
    }

    static inline uint16x8 compare_gt(uint16x8 a, uint16x8 b)
    {
        return scalar_unroll(scalar_compare_gt, a, b);
    }

    static inline uint16x8 select(uint16x8 mask, uint16x8 a, uint16x8 b)
    {
        return scalar_select(mask, a, b);
    }

    static inline uint16x8 min(uint16x8 a, uint16x8 b)
    {
        return scalar_unroll(scalar_min, a, b);
    }

    static inline uint16x8 max(uint16x8 a, uint16x8 b)
    {
        return scalar_unroll(scalar_max, a, b);
    }
    
    // -----------------------------------------------------------------
    // uint32x4
    // -----------------------------------------------------------------

    // shuffle

    template <uint32 x, uint32 y, uint32 z, uint32 w>
    static inline uint32x4 shuffle(uint32x4 v)
    {
        static_assert(x < 4 && y < 4 && z < 4 && w < 4, "Index out of range.");
        return {{ v[x], v[y], v[z], v[w] }};
    }

    template <>
    inline uint32x4 shuffle<0, 1, 2, 3>(uint32x4 v)
    {
        // .xyzw
        return v;
    }

    // indexed access

    template <int Index>
    static inline uint32x4 set_component(uint32x4 a, uint32 s)
    {
        static_assert(Index >= 0 && Index < 4, "Index out of range.");
        a[Index] = s;
        return a;
    }

    template <int Index>
    static inline uint32 get_component(uint32x4 a)
    {
        static_assert(Index >= 0 && Index < 4, "Index out of range.");
        return a[Index];
    }

    static inline uint32x4 uint32x4_zero()
    {
        return {{ 0, 0, 0, 0 }};
    }

    static inline uint32x4 uint32x4_set1(uint32 s)
    {
        return {{ s, s, s, s }};
    }

    static inline uint32x4 uint32x4_set4(uint32 x, uint32 y, uint32 z, uint32 w)
    {
        return {{ x, y, z, w }};
    }

    static inline uint32x4 uint32x4_uload(const uint32* source)
    {
        return uint32x4_set4(source[0], source[1], source[2], source[3]);
    }

    static inline void uint32x4_ustore(uint32* dest, uint32x4 a)
    {
        dest[0] = a[0];
        dest[1] = a[1];
        dest[2] = a[2];
        dest[3] = a[3];
    }

    static inline uint32x4 unpacklo(uint32x4 a, uint32x4 b)
    {
        return scalar_unpacklo(a, b);
    }

    static inline uint32x4 unpackhi(uint32x4 a, uint32x4 b)
    {
        return scalar_unpackhi(a, b);
    }

    static inline uint32x4 add(uint32x4 a, uint32x4 b)
    {
        return scalar_unroll(scalar_add, a, b);
    }

    static inline uint32x4 sub(uint32x4 a, uint32x4 b)
    {
        return scalar_unroll(scalar_sub, a, b);
    }

    static inline uint32x4 mullo(uint32x4 a, uint32x4 b)
    {
        return scalar_unroll(scalar_mullo, a, b);
    }

    // saturated

    static inline uint32x4 adds(uint32x4 a, uint32x4 b)
    {
        return scalar_unroll(scalar_unsigned_adds, a, b);
    }

    static inline uint32x4 subs(uint32x4 a, uint32x4 b)
    {
        return scalar_unroll(scalar_unsigned_subs, a, b);
    }

    // bitwise

    static inline uint32x4 uint32x4_and(uint32x4 a, uint32x4 b)
    {
        return scalar_unroll(scalar_and, a, b);
    }

    static inline uint32x4 uint32x4_nand(uint32x4 a, uint32x4 b)
    {
        return scalar_unroll(scalar_nand, a, b);
    }

    static inline uint32x4 uint32x4_or(uint32x4 a, uint32x4 b)
    {
        return scalar_unroll(scalar_or, a, b);
    }

    static inline uint32x4 uint32x4_xor(uint32x4 a, uint32x4 b)
    {
        return scalar_unroll(scalar_xor, a, b);
    }

    // shift

    template <int Count> 
    static inline uint32x4 sll(uint32x4 a)
    {
        uint32x4 v;
        v[0] = a[0] << Count;
        v[1] = a[1] << Count;
        v[2] = a[2] << Count;
        v[3] = a[3] << Count;
        return v;
    }

    template <int Count> 
    static inline uint32x4 srl(uint32x4 a)
    {
        uint32x4 v;
        v[0] = a[0] >> Count;
        v[1] = a[1] >> Count;
        v[2] = a[2] >> Count;
        v[3] = a[3] >> Count;
        return v;
    }

    template <int Count> 
    static inline uint32x4 sra(uint32x4 a)
    {
        uint32x4 v;
        v[0] = static_cast<int32>(a[0]) >> Count;
        v[1] = static_cast<int32>(a[1]) >> Count;
        v[2] = static_cast<int32>(a[2]) >> Count;
        v[3] = static_cast<int32>(a[3]) >> Count;
        return v;
    }

    // compare

    static inline uint32x4 compare_eq(uint32x4 a, uint32x4 b)
    {
        return scalar_unroll(scalar_compare_eq, a, b);
    }

    static inline uint32x4 compare_gt(uint32x4 a, uint32x4 b)
    {
        return scalar_unroll(scalar_compare_gt, a, b);
    }

    static inline uint32x4 select(uint32x4 mask, uint32x4 a, uint32x4 b)
    {
        return scalar_select(mask, a, b);
    }

    static inline uint32x4 min(uint32x4 a, uint32x4 b)
    {
        return scalar_unroll(scalar_min, a, b);
    }

    static inline uint32x4 max(uint32x4 a, uint32x4 b)
    {
        return scalar_unroll(scalar_max, a, b);
    }

    // -----------------------------------------------------------------
    // int8x16
    // -----------------------------------------------------------------

    static inline int8x16 int8x16_zero()
    {
        return scalar_set<int8, 16>(0);
    }

    static inline int8x16 int8x16_set1(int8 s)
    {
        return scalar_set<int8, 16>(s);
    }

    static inline int8x16 unpacklo(int8x16 a, int8x16 b)
    {
        return scalar_unpacklo(a, b);
    }

    static inline int8x16 unpackhi(int8x16 a, int8x16 b)
    {
        return scalar_unpackhi(a, b);
    }

    static inline int8x16 add(int8x16 a, int8x16 b)
    {
        return scalar_unroll(scalar_add, a, b);
    }

    static inline int8x16 sub(int8x16 a, int8x16 b)
    {
        return scalar_unroll(scalar_sub, a, b);
    }

    static inline int8x16 mullo(int8x16 a, int8x16 b)
    {
        return scalar_unroll(scalar_mullo, a, b);
    }

    // saturated

    static inline int8x16 adds(int8x16 a, int8x16 b)
    {
        return scalar_unroll(scalar_signed_adds, a, b);
    }

    static inline int8x16 subs(int8x16 a, int8x16 b)
    {
        return scalar_unroll(scalar_signed_subs, a, b);
    }

    static inline int8x16 abs(int8x16 a)
    {
        return scalar_unroll(scalar_abs, a);
    }

    static inline int8x16 neg(int8x16 a)
    {
        return scalar_unroll(scalar_neg, a);
    }

    // bitwise

    static inline int8x16 int8x16_and(int8x16 a, int8x16 b)
    {
        return scalar_unroll(scalar_and, a, b);
    }

    static inline int8x16 int8x16_nand(int8x16 a, int8x16 b)
    {
        return scalar_unroll(scalar_nand, a, b);
    }

    static inline int8x16 int8x16_or(int8x16 a, int8x16 b)
    {
        return scalar_unroll(scalar_or, a, b);
    }

    static inline int8x16 int8x16_xor(int8x16 a, int8x16 b)
    {
        return scalar_unroll(scalar_xor, a, b);
    }

    // compare

    static inline int8x16 compare_eq(int8x16 a, int8x16 b)
    {
        return scalar_unroll(scalar_compare_eq, a, b);
    }

    static inline int8x16 compare_gt(int8x16 a, int8x16 b)
    {
        return scalar_unroll(scalar_compare_gt, a, b);
    }

    static inline int8x16 select(int8x16 mask, int8x16 a, int8x16 b)
    {
        return scalar_select(mask, a, b);
    }

    static inline int8x16 min(int8x16 a, int8x16 b)
    {
        return scalar_unroll(scalar_min, a, b);
    }

    static inline int8x16 max(int8x16 a, int8x16 b)
    {
        return scalar_unroll(scalar_max, a, b);
    }

    // -----------------------------------------------------------------
    // int16x8
    // -----------------------------------------------------------------

    static inline int16x8 int16x8_zero()
    {
        return scalar_set<int16, 8>(0);
    }

    static inline int16x8 int16x8_set1(int16 s)
    {
        return scalar_set<int16, 8>(s);
    }

    static inline int16x8 unpacklo(int16x8 a, int16x8 b)
    {
        return scalar_unpacklo(a, b);
    }

    static inline int16x8 unpackhi(int16x8 a, int16x8 b)
    {
        return scalar_unpackhi(a, b);
    }

    static inline int16x8 add(int16x8 a, int16x8 b)
    {
        return scalar_unroll(scalar_add, a, b);
    }

    static inline int16x8 sub(int16x8 a, int16x8 b)
    {
        return scalar_unroll(scalar_sub, a, b);
    }

    static inline int16x8 mullo(int16x8 a, int16x8 b)
    {
        return scalar_unroll(scalar_mullo, a, b);
    }

    // saturated

    static inline int16x8 adds(int16x8 a, int16x8 b)
    {
        return scalar_unroll(scalar_signed_adds, a, b);
    }

    static inline int16x8 subs(int16x8 a, int16x8 b)
    {
        return scalar_unroll(scalar_signed_subs, a, b);
    }

    static inline int16x8 abs(int16x8 a)
    {
        return scalar_unroll(scalar_abs, a);
    }

    static inline int16x8 neg(int16x8 a)
    {
        return scalar_unroll(scalar_neg, a);
    }

    // bitwise

    static inline int16x8 int16x8_and(int16x8 a, int16x8 b)
    {
        return scalar_unroll(scalar_and, a, b);
    }

    static inline int16x8 int16x8_nand(int16x8 a, int16x8 b)
    {
        return scalar_unroll(scalar_nand, a, b);
    }

    static inline int16x8 int16x8_or(int16x8 a, int16x8 b)
    {
        return scalar_unroll(scalar_or, a, b);
    }

    static inline int16x8 int16x8_xor(int16x8 a, int16x8 b)
    {
        return scalar_unroll(scalar_xor, a, b);
    }

    // compare

    static inline int16x8 compare_eq(int16x8 a, int16x8 b)
    {
        return scalar_unroll(scalar_compare_eq, a, b);
    }

    static inline int16x8 compare_gt(int16x8 a, int16x8 b)
    {
        return scalar_unroll(scalar_compare_gt, a, b);
    }

    static inline int16x8 select(int16x8 mask, int16x8 a, int16x8 b)
    {
        return scalar_select(mask, a, b);
    }

    static inline int16x8 min(int16x8 a, int16x8 b)
    {
        return scalar_unroll(scalar_min, a, b);
    }

    static inline int16x8 max(int16x8 a, int16x8 b)
    {
        return scalar_unroll(scalar_max, a, b);
    }

    // -----------------------------------------------------------------
    // int32x4
    // -----------------------------------------------------------------

    // shuffle

    template <uint32 x, uint32 y, uint32 z, uint32 w>
    static inline int32x4 shuffle(int32x4 v)
    {
        static_assert(x < 4 && y < 4 && z < 4 && w < 4, "Index out of range.");
        return {{ v[x], v[y], v[z], v[w] }};
    }

    template <>
    inline int32x4 shuffle<0, 1, 2, 3>(int32x4 v)
    {
        // .xyzw
        return v;
    }

    // indexed access

    template <int Index>
    static inline int32x4 set_component(int32x4 a, int32 s)
    {
        static_assert(Index >= 0 && Index < 4, "Index out of range.");
        a[Index] = s;
        return a;
    }

    template <int Index>
    static inline int32 get_component(int32x4 a)
    {
        static_assert(Index >= 0 && Index < 4, "Index out of range.");
        return a[Index];
    }

    static inline int32x4 int32x4_zero()
    {
        return {{ 0, 0, 0, 0 }};
    }

    static inline int32x4 int32x4_set1(int s)
    {
        return {{ s, s, s, s }};
    }

    static inline int32x4 int32x4_set4(int x, int y, int z, int w)
    {
        return {{ x, y, z, w }};
    }

    static inline int32x4 int32x4_uload(const int* source)
    {
        return int32x4_set4(source[0], source[1], source[2], source[3]);
    }

    static inline void int32x4_ustore(int* dest, int32x4 a)
    {
        dest[0] = a[0];
        dest[1] = a[1];
        dest[2] = a[2];
        dest[3] = a[3];
    }

    static inline int32x4 unpacklo(int32x4 a, int32x4 b)
    {
        return scalar_unpacklo(a, b);
    }

    static inline int32x4 unpackhi(int32x4 a, int32x4 b)
    {
        return scalar_unpackhi(a, b);
    }

    static inline int32x4 abs(int32x4 a)
    {
        return scalar_unroll(scalar_abs, a);
    }

    static inline int32x4 neg(int32x4 a)
    {
        return scalar_unroll(scalar_neg, a);
    }

    static inline int32x4 add(int32x4 a, int32x4 b)
    {
        return scalar_unroll(scalar_add, a, b);
    }

    static inline int32x4 sub(int32x4 a, int32x4 b)
    {
        return scalar_unroll(scalar_sub, a, b);
    }

    static inline int32x4 mullo(int32x4 a, int32x4 b)
    {
        return scalar_unroll(scalar_mullo, a, b);
    }

    // saturated

    static inline int32x4 adds(int32x4 a, int32x4 b)
    {
        return scalar_unroll(scalar_signed_adds, a, b);
    }

    static inline int32x4 subs(int32x4 a, int32x4 b)
    {
        return scalar_unroll(scalar_signed_subs, a, b);
    }

    // bitwise

    static inline int32x4 int32x4_and(int32x4 a, int32x4 b)
    {
        return scalar_unroll(scalar_and, a, b);
    }

    static inline int32x4 int32x4_nand(int32x4 a, int32x4 b)
    {
        return scalar_unroll(scalar_nand, a, b);
    }

    static inline int32x4 int32x4_or(int32x4 a, int32x4 b)
    {
        return scalar_unroll(scalar_or, a, b);
    }

    static inline int32x4 int32x4_xor(int32x4 a, int32x4 b)
    {
        return scalar_unroll(scalar_xor, a, b);
    }

    // shift

    template <int Count> 
    static inline int32x4 sll(int32x4 a)
    {
        int32x4 v;
        v[0] = static_cast<uint32>(a[0]) << Count;
        v[1] = static_cast<uint32>(a[1]) << Count;
        v[2] = static_cast<uint32>(a[2]) << Count;
        v[3] = static_cast<uint32>(a[3]) << Count;
        return v;
    }

    template <int Count> 
    static inline int32x4 srl(int32x4 a)
    {
        int32x4 v;
        v[0] = static_cast<uint32>(a[0]) >> Count;
        v[1] = static_cast<uint32>(a[1]) >> Count;
        v[2] = static_cast<uint32>(a[2]) >> Count;
        v[3] = static_cast<uint32>(a[3]) >> Count;
        return v;
    }

    template <int Count> 
    static inline int32x4 sra(int32x4 a)
    {
        int32x4 v;
        v[0] = a[0] >> Count;
        v[1] = a[1] >> Count;
        v[2] = a[2] >> Count;
        v[3] = a[3] >> Count;
        return v;
    }

    // compare

    static inline int32x4 compare_eq(int32x4 a, int32x4 b)
    {
        return scalar_unroll(scalar_compare_eq, a, b);
    }

    static inline int32x4 compare_gt(int32x4 a, int32x4 b)
    {
        return scalar_unroll(scalar_compare_gt, a, b);
    }

    static inline int32x4 select(int32x4 mask, int32x4 a, int32x4 b)
    {
        return scalar_select(mask, a, b);
    }

    static inline int32x4 min(int32x4 a, int32x4 b)
    {
        return scalar_unroll(scalar_min, a, b);
    }

    static inline int32x4 max(int32x4 a, int32x4 b)
    {
        return scalar_unroll(scalar_max, a, b);
    }

    static inline uint32 get_mask(int32x4 a)
    {
        const uint32 x = a[0] & 0x80000000;
        const uint32 y = a[1] & 0x80000000;
        const uint32 z = a[2] & 0x80000000;
        const uint32 w = a[3] & 0x80000000;
        const uint32 mask = (x >> 31) | (y >> 30) | (z >> 29) | (w >> 28);
        return mask;
    }

    static inline uint32 pack(int32x4 s)
    {
        const uint32 x = byteclamp(s[0]);
        const uint32 y = byteclamp(s[1]);
        const uint32 z = byteclamp(s[2]);
        const uint32 w = byteclamp(s[3]);
        return x | (y << 8) | (z << 16) | (w << 24);
    }

    static inline int32x4 unpack(uint32 s)
    {
        int32x4 v;
        v[0] = (s >> 0) & 0xff;
        v[1] = (s >> 8) & 0xff;
        v[2] = (s >> 16) & 0xff;
        v[3] = (s >> 24);
        return v;
    }

} // namespace simd
} // namespace mango