#ifndef UDL4STDINT_H_
#define UDL4STDINT_H_

#include <cstdint>
#include <limits>
#include <initializer_list>

namespace integer_literals {
#ifdef  __cpp_consteval
consteval uint8_t operator""_u8(unsigned long long val) {
	if (val <= std::numeric_limits<uint8_t>::max())
		return val;
	else
		throw "value is out of range of uint8_t"; // raise compile time error
}
consteval uint16_t operator""_u16(unsigned long long val) {
	if (val <= std::numeric_limits<uint16_t>::max())
		return val;
	else
		throw "value is out of range of uint16_t"; // raise compile time error
}
consteval uint32_t operator""_u32(unsigned long long val) {
	if (val <= std::numeric_limits<uint32_t>::max())
		return val;
	else
		throw "value is out of range of uint32_t"; // raise compile time error
}
consteval uint64_t operator""_u64(unsigned long long val) {
	if (val <= std::numeric_limits<uint64_t>::max()) // not really possible today
		return val;
	else
		throw "value is out of range of uint64_t"; // raise compile time error
}
consteval int8_t operator""_i8(unsigned long long val) {
	if (val <= std::numeric_limits<int8_t>::max())
		return val;
	else
		throw "value is out of range of int8_t"; // raise compile time error
}
consteval int16_t operator""_i16(unsigned long long val) {
	if (val <= std::numeric_limits<int16_t>::max())
		return val;
	else
		throw "value is out of range of int16_t"; // raise compile time error
}
consteval int32_t operator""_i32(unsigned long long val) {
	if (val <= std::numeric_limits<int32_t>::max())
		return val;
	else
		throw "value is out of range of int32_t"; // raise compile time error
}
consteval int64_t operator""_i64(unsigned long long val) {
	if (val <= std::numeric_limits<int64_t>::max())
		return val;
	else
		throw "value is out of range of int64_t"; // raise compile time error
}
#else
namespace details___{

template<unsigned long long value, unsigned base>
constexpr bool check = value < std::numeric_limits<unsigned long long>::max()/base;


template<unsigned base, char ... digits>
constexpr unsigned long long
to_val_base() { // naive, but working compile time conversion for bin, oct, dec, hex
	static_assert((base > 1 && base <= 10) || base == 16);
	unsigned long long result{0};
	for (char const c:{digits...}){
		if (c == '\'') continue; // munch digit separator
		auto const digit = [c]{
			if constexpr (base <=10) {
					/*if ( c>='0' && c <='9') */ // compiler already checks syntax
						return c-'0';
			} else { // assume base is 16
				if (c >= 'A' && c <= 'F') return 10 + c - 'A';
				if (c >= 'a' && c <= 'f') return 10 + c - 'a';
				return c-'0';
				/*else throw base;*/ // compiler already checks syntax
			}
		}();
        // The following detects overflow in constants that otherwise might go undetected,
		// but through 3 compile errors, cannot do with static_assert here, afaik
		if (result > std::numeric_limits<unsigned long long>::max()/base) throw "overflow"; // out of range
		result = result * base + digit;
	}

	return result;
}

template<char ... digits>
constexpr unsigned long long to_val_dispatch=  to_val_base<10,digits...>();;

template<char ... digits>
constexpr unsigned long long to_val_dispatch<'0','x',digits...> =  to_val_base<16, digits...>();

template<char ... digits>
constexpr unsigned long long to_val_dispatch<'0','X',digits...> =  to_val_base<16, digits...>();
template<char ... digits>
constexpr unsigned long long to_val_dispatch<'0','b',digits...> =  to_val_base<2, digits...>();

template<char ... digits>
constexpr unsigned long long to_val_dispatch<'0','B',digits...> =  to_val_base<2, digits...>();

template<char ... digits>
constexpr unsigned long long to_val_dispatch<'0',digits...> =  to_val_base<8,digits...>();
template<>
constexpr unsigned long long to_val_dispatch<'0'> = 0;
}


template<char...digits>
constexpr uint8_t operator""_u8() noexcept {
	constexpr auto val {integer_literals::details___::to_val_dispatch<digits...>};
	static_assert ( val <= std::numeric_limits<uint8_t>::max(), "literal value out of range");
	return val;
}
template<char...digits>
constexpr uint16_t operator""_u16() noexcept {
	constexpr auto val {integer_literals::details___::to_val_dispatch<digits...>};
	static_assert ( val <= std::numeric_limits<uint16_t>::max(), "literal value out of range");
	return val;
}
template<char...digits>
constexpr uint32_t operator""_u32() noexcept {
	constexpr auto val {integer_literals::details___::to_val_dispatch<digits...>};
	static_assert ( val <= std::numeric_limits<uint32_t>::max(), "literal value out of range");
	return val;
}
template<char...digits>
constexpr uint64_t operator""_u64() noexcept {
	constexpr auto val {integer_literals::details___::to_val_dispatch<digits...>};
	static_assert ( val <= std::numeric_limits<uint64_t>::max(), "literal value out of range");
	return val;
}
template<char...digits>
constexpr int8_t operator""_i8() noexcept {
	constexpr auto val {integer_literals::details___::to_val_dispatch<digits...>};
	static_assert ( val <= std::numeric_limits<int8_t>::max(), "literal value out of range");
	return val;
}
template<char...digits>
constexpr int16_t operator""_i16() noexcept {
	constexpr auto val {integer_literals::details___::to_val_dispatch<digits...>};
	static_assert ( val <= std::numeric_limits<int16_t>::max(), "literal value out of range");
	return val;
}
template<char...digits>
constexpr int32_t operator""_i32() noexcept {
	constexpr auto val {integer_literals::details___::to_val_dispatch<digits...>};
	static_assert ( val <= std::numeric_limits<int32_t>::max(), "literal value out of range");
	return val;
}
template<char...digits>
constexpr int64_t operator""_i64() noexcept {
	constexpr auto val {integer_literals::details___::to_val_dispatch<digits...>};
	static_assert ( val <= std::numeric_limits<int64_t>::max(), "literal value out of range");
	return val;
}

#endif
}
#ifdef UDL4stdint_TESTING
#include <type_traits>
namespace udl4stdint_testing____ {
using namespace integer_literals;

static_assert(1 == 1_u8);
static_assert(255 == 255_u8);
static_assert(2'55_u8 == 255);
static_assert(0'377_u8 == 255);
static_assert(0xff_u8 == 255);
static_assert(0XfF_u8 == 255);
static_assert(0b1010'1010_u8 == 170);
static_assert(0B1010'1010_u8 == 170);
static_assert(0XFFFF'FFFF'FFFF'FFFF_u64 == std::numeric_limits<uint64_t>::max());
static_assert(0XFFFF'FFFF'FFFF'FFFE_u64 +1ULL == std::numeric_limits<uint64_t>::max());
static_assert(0x7fff_i16 == std::numeric_limits<int16_t>::max());


template<typename T, typename U>
constexpr bool hasType(U value) noexcept {
	return std::is_same_v<T,U>;
}

static_assert(hasType<uint8_t>(1_u8));
static_assert(hasType<uint16_t>(1_u16));
static_assert(hasType<uint32_t>(1_u32));
static_assert(hasType<uint64_t>(0_u64));
static_assert(hasType<int8_t>(1_i8));
static_assert(hasType<int16_t>(1_i16));
static_assert(hasType<int32_t>(1_i32));
static_assert(hasType<int64_t>(0_i64));


// won't compile, because minus is applied after conversion.
#if 0
static_assert(-32768_i16 == std::numeric_limits<int16_t>::min());
#endif

// won't compile, as intended:
#if 0
static_assert(0401_u8);
static_assert(0x1'0001_u16);
static_assert(0x1'0000'0001_u32);
static_assert(0XFFFF'FFFF'FFFF'FFFF'0_u64);
#endif

}
#endif




#endif /* UDL4STDINT_H_ */
