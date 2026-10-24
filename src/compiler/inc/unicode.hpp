#pragma once

#include <cstddef>
#include <string>
#include <utility>

#include <unicode/normalizer2.h>
#include <unicode/uchar.h>
#include <unicode/unistr.h>
#include <unicode/utfiterator.h>

namespace cbycpp {
namespace scanner {
class UnicodeUtil {
  public:
	inline static icu::UnicodeString getU16String(const std::string string) {
		return icu::UnicodeString::fromUTF8(string);
	}

	inline static std::string getU8String(icu::UnicodeString string16) {
		std::string re{};
		string16.toUTF8String(re);
		return re;
	}

	inline static std::string getU8String(UChar32 uc) {
		return getU8String(icu::UnicodeString(uc));
	}

	inline static icu::UnicodeString getNFCU16(const icu::Normalizer2 *nfc,
	                                           std::string string) {
		if (nfc == nullptr) {
			return {};
		}
		auto u16 = getU16String(string);
		UErrorCode status = U_ZERO_ERROR;
		auto re = nfc->normalize(u16, status);
		if (U_FAILURE(status)) {
			return {};
		}
		return re;
	}

	using U32String =
	    decltype(icu_78::header::utfStringCodePoints<UChar32,
	                                                 UTF_BEHAVIOR_NEGATIVE>(
	        std::declval<icu::UnicodeString>()));

	inline static U32String getU32String(icu::UnicodeString string16) {
		return icu_78::header::utfStringCodePoints<UChar32,
		                                           UTF_BEHAVIOR_NEGATIVE>(
		    std::move(string16));
		// 对右值和左值的效果不一样，这里用右值
	}

	using U32It = decltype(U32String{}.begin());

	// inline static UChar32 getChar32At(const icu::UnicodeString &string,
	//                                   std::size_t index) {
	// 	auto Size = string.length();
	// 	if (index < Size) {
	// 		return string.char32At(index);
	// 	}
	// 	return {};
	// }

	inline static bool isXID_Start(UChar32 c) {
		if (c == U'_')
			return true;
		return u_hasBinaryProperty(c, UCHAR_XID_START);
	}

	inline static bool isXID_Continue(UChar32 c) {
		if (c == U'_')
			return true;
		return u_hasBinaryProperty(c, UCHAR_XID_CONTINUE);
	}
};

} // namespace scanner
} // namespace cbycpp
