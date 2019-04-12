#include "../includes.hpp"

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
			goto returning;
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

returning:
		std::memset(global_buffer_t::buffer, 0, sizeof(data_t) * global_buffer_t::length);
		return global_buffer_t::buffer;
	}
};

template <> Watcher global_buffer_t<char>::watch([]() { 
	delete[] global_buffer_t<char>::buffer; 
});
template <> char* global_buffer_t<char>::buffer = nullptr;
template <> std::size_t global_buffer_t<char>::length = 0;

template <> Watcher global_buffer_t<wchar_t>::watch([]() {
	delete[] global_buffer_t<wchar_t>::buffer; 
});
template <> wchar_t* global_buffer_t<wchar_t>::buffer = nullptr;
template <> std::size_t global_buffer_t<wchar_t>::length = 0;

unmanaged_wide_to_multibyte_t::result_t unmanaged_wide_to_multibyte_t::convert(const input_t* input) {
	using scope_t = unmanaged_wide_to_multibyte_t;

	std::size_t inputLength = string_helper<scope_t::input_t>::strlen(input);
	int outputLength = WideCharToMultiByte(CP_ACP, 0, input, inputLength, NULL, 0, NULL, NULL);
	scope_t::result_t output = global_buffer_t<scope_t::output_t>::allocate(outputLength);

	WideCharToMultiByte(CP_ACP, 0, input, inputLength, output, outputLength, NULL, NULL);

	return output;
}
unmanaged_wide_to_utf8_t::result_t unmanaged_wide_to_utf8_t::convert(const input_t* input) {
	using scope_t = unmanaged_wide_to_utf8_t;

	std::size_t inputLength = string_helper<scope_t::input_t>::strlen(input);
	int outputLength = WideCharToMultiByte(CP_UTF8, 0, input, inputLength, NULL, 0, NULL, NULL);
	scope_t::result_t output = global_buffer_t<scope_t::output_t>::allocate(outputLength);

	WideCharToMultiByte(CP_UTF8, 0, input, inputLength, output, outputLength, NULL, NULL);

	return output;
}

unmanaged_multibyte_to_wide_t::result_t unmanaged_multibyte_to_wide_t::convert(const input_t* input) {
	using scope_t = unmanaged_multibyte_to_wide_t;

	std::size_t inputLength = string_helper<scope_t::input_t>::strlen(input);
	int outputLength = MultiByteToWideChar(CP_ACP, 0, input, inputLength, NULL, NULL);

	scope_t::result_t output = global_buffer_t<scope_t::output_t>::allocate(outputLength);
	MultiByteToWideChar(CP_ACP, 0, input, inputLength, output, outputLength);

	return output;
}
unmanaged_multibyte_to_utf8_t::result_t unmanaged_multibyte_to_utf8_t::convert(const input_t* input) {
	unmanaged_multibyte_to_wide_t::result_t data = unmanaged_multibyte_to_wide_t::convert(input);
	return unmanaged_wide_to_utf8_t::convert(data);
}

unmanaged_utf8_to_wide_t::result_t unmanaged_utf8_to_wide_t::convert(const input_t* input) {
	using scope_t = unmanaged_utf8_to_wide_t;

	std::size_t inputLength = string_helper<scope_t::input_t>::strlen(input);
	std::size_t outputLength = MultiByteToWideChar(CP_UTF8, 0, input, inputLength, NULL, NULL);
	scope_t::result_t output = global_buffer_t<scope_t::output_t>::allocate(outputLength);

	MultiByteToWideChar(CP_UTF8, 0, input, inputLength, output, outputLength);

	return output;
}
unmanaged_utf8_to_multibyte_t::result_t unmanaged_utf8_to_multibyte_t::convert(const input_t* input) {
	unmanaged_utf8_to_wide_t::result_t data = unmanaged_utf8_to_wide_t::convert(input);
	return unmanaged_wide_to_multibyte_t::convert(data);
}

#define IMPL_CODE_CVT_MANAGED(x) x::result_t x::convert(const input_t* input) { return x::inverse_t::convert(input); }

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
