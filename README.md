# UDL4stdint

```C++
uint8_t example(){
	using namespace integer_literals;

	auto u8 = 42_u8; // uint8_t
	u8++;
	return u8;
}
```


This header defines User-defined literal suffix operators for the (major) fixed width integer types from the header `<cstdint>`.
	
The following suffixes are defined resulting the appropriate types:

* `_u8`
* `_u16`
* `_u32`
* `_u64`
* `_i8`
* `_i16`
* `_i32`
* `_i64`

Providing a too large literal raises a compile error. 

You can use decimal, hexidecimal, octal, and binary notation and apply digit separators.

Implementation for C++20 might compile much faster, because it uses `consteval`, whereas previous versions require compile-time DIY number parsing for guaranteed compile-time checks of constant overflow.

Compile time tests:
```C++
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

```