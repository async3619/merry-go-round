#include "../includes.hpp"

#if !defined(_MERRY_GO_ROUND_USE_WIN32) && defined(_MERRY_GO_ROUND_USE_POSIX)
#   include <codecvt>
#	include <locale>

template <typename char_t>
struct string_helper {};

template <>
struct string_helper<char> {
    static size_t strlen(const char* string) {
        return std::strlen(string);
    }
};

template <>
struct string_helper<wchar_t> {
    static size_t strlen(const wchar_t* string) {
        return std::wcslen(string);
    }
};

template <typename data_t>
struct global_buffer_t {
private:
    static Watcher watch;
    static data_t* buffer;
    static std::size_t length;

public:
    static data_t* allocate(std::size_t length) {
        if (length < global_buffer_t::length) {
            std::memset(global_buffer_t::buffer, 0, sizeof(data_t) * global_buffer_t::length);
            return global_buffer_t::buffer;
        }

        std::size_t nearestLength = 1;
        while (length >= nearestLength) {
            nearestLength *= 2;
        }

        void* data = global_buffer_t::buffer;
        if (global_buffer_t::buffer) {
            delete[] global_buffer_t::buffer;
        }

        // I used unmanaged one (malloc) here because it'll be DESTRUCTED when node.js instance is about to close.
        // because watcher is defined as *static* variable. and static variable destructing is called lately than
        // callback registered by `napi_add_env_cleanup_hook`.
        global_buffer_t::buffer = static_cast<data_t*>(malloc(nearestLength * sizeof(size_t)));
        global_buffer_t::length = nearestLength;

        std::memset(global_buffer_t::buffer, 0, sizeof(data_t) * global_buffer_t::length);
        return global_buffer_t::buffer;
    }
};

template <> char* global_buffer_t<char>::buffer = nullptr;
template <> std::size_t global_buffer_t<char>::length = 0;
template <> Watcher global_buffer_t<char>::watch([]() {
    delete[] global_buffer_t<char>::buffer;
});

template <> wchar_t* global_buffer_t<wchar_t>::buffer = nullptr;
template <> std::size_t global_buffer_t<wchar_t>::length = 0;
template <> Watcher global_buffer_t<wchar_t>::watch([]() {
    delete[] global_buffer_t<wchar_t>::buffer;
});

//
// string types (char*) of this project will be two possibilities. UTF-8 or Latin1 (multibyte-based string).
// In majority of compilers nowadays will treat normal string literal as UTF-8 but we still don't know whether if 
// given char* is UTF-8 or Lain1. so we're going to consider multibyte string is just multibyte string. even if it's not.
//
// but if we wanted to wide string or UTF-8 as multibyte, we would convert it to UTF-8 instead of multibyte.
// because they're same actually.
//
// and we'll treat wide character (wchar_t) as UTF-16 because the only usage of wide character encoding is for TagLib
// library which uses wchar_t to store UTF-16 string.
//

// utility wrapper to adapt locale-bound facets for wstring/wbuffer convert
template<class Facet>
struct deletable_facet : Facet {
	template<class ...Args>
	deletable_facet(Args&& ...args) : Facet(std::forward<Args>(args)...) {}
	~deletable_facet() {}
};

TEMPLACE_SPECIALIZE unmanaged_multibyte_to_utf8_t::result_t unmanaged_multibyte_to_utf8_t::convert(const input_t* input) {
    //
    // I found that TagLib handles all string data as UTF-8 even if they're encoded as Latin1.
    // so I'll do basic copy operation for this function.
    //

    // UTF-8 -> UTF-8
    std::string latin1String = input;

    char* result = global_buffer_t<char>::allocate(latin1String.size() + 1);
    memcpy(result, latin1String.c_str(), sizeof(char) * latin1String.size());

	return result;
}
TEMPLACE_SPECIALIZE unmanaged_multibyte_to_wide_t::result_t unmanaged_multibyte_to_wide_t::convert(const input_t* input) {
    // Latin1 -> UTF-8
    std::string u8string = unmanaged_multibyte_to_utf8_t::convert(input);

    std::cout << "Latin1 -> UTF-8: " << input << " -> " << u8string << std::endl;

    // UTF-8 -> UTF-16
    std::wstring_convert<deletable_facet<std::codecvt<char16_t, char, std::mbstate_t>>, char16_t> conv16;
    std::u16string u16 = conv16.from_bytes(u8string);

    std::cout << "UTF-8 -> UTF-16: " << u8string << std::endl;

    // we shouldn't memcpy on here because value of `sizeof(wchar_t)` is compiler-dependent.
    auto* buffer = global_buffer_t<unmanaged_wide_to_utf8_t::input_t>::allocate(u16.size() + 1);
    for (std::size_t i = 0, j = u16.size(); i < j; ++i) {
        buffer[i] = u16[i];
    }

    return buffer;
}

TEMPLACE_SPECIALIZE unmanaged_wide_to_utf8_t::result_t unmanaged_wide_to_utf8_t::convert(const input_t* input) {
    // UTF-16 -> UTF-8
    std::wstring wideString = input;
    std::string utf8String = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>{}.to_bytes(wideString);

    char* result = global_buffer_t<char>::allocate(utf8String.size() + 1);
    memcpy(result, utf8String.c_str(), sizeof(char) * utf8String.size());

    return result;
}
TEMPLACE_SPECIALIZE unmanaged_wide_to_multibyte_t::result_t unmanaged_wide_to_multibyte_t::convert(const input_t* input) {
    // UTF-16 -> UTF-8
    return unmanaged_wide_to_utf8_t::convert(input);
}

TEMPLACE_SPECIALIZE unmanaged_utf8_to_wide_t::result_t unmanaged_utf8_to_wide_t::convert(const input_t* input) {
	// UTF-8 -> UTF-16
	std::wstring wideString = std::wstring_convert<std::codecvt_utf8<wchar_t>>{}.from_bytes(input);

	wchar_t* result = global_buffer_t<wchar_t>::allocate(wideString.size() + 1);
	memcpy(result, wideString.c_str(), sizeof(wchar_t) * wideString.size());

    return result;
}
TEMPLACE_SPECIALIZE unmanaged_utf8_to_multibyte_t::result_t unmanaged_utf8_to_multibyte_t::convert(const input_t* input) {
	// UTF-8 -> UTF-8
	std::string utf8String = input;

	char* result = global_buffer_t<char>::allocate(utf8String.size() + 1);
	memcpy(result, utf8String.c_str(), sizeof(char) * utf8String.size());

    return result;
}

#define IMPL_CODE_CVT_MANAGED(x) TEMPLACE_SPECIALIZE x::result_t x::convert(const input_t* input) { return x::inverse_t::convert(input); }

IMPL_CODE_CVT_MANAGED(multibyte_to_wide_t);
IMPL_CODE_CVT_MANAGED(multibyte_to_utf8_t);
IMPL_CODE_CVT_MANAGED(wide_to_multibyte_t);
IMPL_CODE_CVT_MANAGED(wide_to_utf8_t);
IMPL_CODE_CVT_MANAGED(utf8_to_wide_t);
IMPL_CODE_CVT_MANAGED(utf8_to_multibyte_t);

template <>
utf8_t::elem_t* code_cvt_helper::toUtf8<TagLib::String>(const TagLib::String& ref) {
    // check if given string object is unicode.
    // if it's already a unicode string, that means it has own utf-8/16 string.
    // (which is at least wchar_t on windows)
    bool isUnicode = !(ref.isAscii() || ref.isLatin1());
    char* result = nullptr;

    // call most suitable code converter.
    if (isUnicode) {
        result = unmanaged_wide_to_utf8_t::convert(ref.toCWString());
    } else {
        result = unmanaged_multibyte_to_utf8_t::convert(ref.toCString());
    }

    return result;
}

template <>
utf8_t::elem_t* code_cvt_helper::toUtf8<TagLib::String>(TagLib::String&& ref) {
    return code_cvt_helper::toUtf8<TagLib::String>(ref);
}

template <>
utf8_t::elem_t* code_cvt_helper::toUtf8<std::string>(std::string&& ref) {
    return unmanaged_multibyte_to_utf8_t::convert(ref.c_str());
}

#endif
