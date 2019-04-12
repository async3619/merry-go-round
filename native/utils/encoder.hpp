#ifndef MERRY_GO_ROUND_ENCODER_HPP
#define MERRY_GO_ROUND_ENCODER_HPP

struct null_t {	typedef void elem_t; typedef void managed_t; };
struct wide_t {	typedef wchar_t elem_t; typedef std::wstring managed_t; };
struct multibyte_t { typedef char elem_t; typedef std::string managed_t; };
struct utf8_t {	typedef char elem_t; typedef std::string managed_t; };

template <typename src_t, typename dest_t, bool managed = true>
struct code_cvt {
	typedef typename dest_t::elem_t output_t;
	typedef typename src_t::elem_t input_t;
	typedef typename std::conditional<managed, typename dest_t::managed_t, output_t*>::type result_t;
	typedef code_cvt<src_t, dest_t, !managed> inverse_t;

	static result_t convert(const input_t* input);
};

typedef code_cvt<wide_t, multibyte_t, false> unmanaged_wide_to_multibyte_t;
typedef code_cvt<wide_t, utf8_t, false> unmanaged_wide_to_utf8_t;
typedef code_cvt<multibyte_t, wide_t, false> unmanaged_multibyte_to_wide_t;
typedef code_cvt<multibyte_t, utf8_t, false> unmanaged_multibyte_to_utf8_t;
typedef code_cvt<utf8_t, wide_t, false> unmanaged_utf8_to_wide_t;
typedef code_cvt<utf8_t, multibyte_t, false> unmanaged_utf8_to_multibyte_t;

typedef unmanaged_wide_to_multibyte_t::inverse_t wide_to_multibyte_t;
typedef unmanaged_wide_to_utf8_t::inverse_t wide_to_utf8_t;
typedef unmanaged_multibyte_to_wide_t::inverse_t multibyte_to_wide_t;
typedef unmanaged_multibyte_to_utf8_t::inverse_t multibyte_to_utf8_t;
typedef unmanaged_utf8_to_wide_t::inverse_t utf8_to_wide_t;
typedef unmanaged_utf8_to_multibyte_t::inverse_t utf8_to_multibyte_t;

template <typename T>
struct pure_type {
	typedef typename std::remove_const<typename std::remove_pointer<T>::type>::type type;
};

struct code_cvt_helper {
	template <class T>
	static std::enable_if_t<std::is_class<typename pure_type<T>::type>::value, utf8_t::elem_t*> toUtf8(T&& ref);

	template <class T>
	static std::enable_if_t<!std::is_class<typename pure_type<T>::type>::value, utf8_t::elem_t*> toUtf8(const T* ref);
};

#define _TO_UTF8(x, type) code_cvt_helper::toUtf8<type>(x)
#define TO_UTF8(x) code_cvt_helper::toUtf8(x)

// convert node.js string type (utf-8) to normal string (ASCII/ANSI)
#define CONV_ARGUMENT_ENCODING(x) x = utf8_to_multibyte_t::convert(x.c_str());

// convert TagLib string type (ASCII/ANSI/UTF-8/UTF-16/etc.) to UTF-8
#define TAGLIB_STRING_TO_UTF8(x) _TO_UTF8(x, TagLib::String)

#endif // MERRY_GO_ROUND_ENCODER_HPP
