#ifndef MERRY_GO_ROUND_PREPROCESSORS_HPP
#	define MERRY_GO_ROUND_PREPROCESSORS_HPP
#	define TEMPLACE_SPECIALIZE template <>
#	if defined(_WIN32)
#		define IO_H <io.h> 
#		if defined(MERRY_GO_ROUND_USE_WIN32) || !defined(MERRY_GO_ROUND_USE_POSIX)
#			define _MERRY_GO_ROUND_USE_WIN32 
#		elif defined(/*MERRY_GO_ROUND_USE_POSIX*/)
#			define _MERRY_GO_ROUND_USE_POSIX
#		endif
#	else
#		define IO_H <sys/io.h> 
#		define _MERRY_GO_ROUND_USE_POSIX
#	endif

#	if defined(_WIN32)
#		if defined(_MERRY_GO_ROUND_USE_POSIX)
#			define _CRT_SECURE_NO_WARNINGS
#			define _CRT_SECURE_NO_DEPRECATE
#			define _CRT_NONSTDC_NO_WARNINGS
#			include <sys/stat.h>
#			include IO_H
#			include <fcntl.h>
#		endif
#		define WIN32_LEAN_AND_MEAN
#		include <windows.h>
#	else
#		include <sys/stat.h>
#		include IO_H
#		include <fcntl.h>
#	endif
#endif // MERRY_GO_ROUND_PREPROCESSORS_HPP
