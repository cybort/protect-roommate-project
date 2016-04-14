//strcamp.hpp
//Author:qdxllecrn
//E-mail:sweheartiii[at]hotmail.com
//Code:https://raw.githubusercontent.com/Too-Naive/Tools/master/strcamp.hpp

#pragma once
#include <string.h>
#include <tchar.h>
#include <stdlib.h>



#ifdef __GXX_EXPERIMENTAL_CXX0X__
#error This file DO NOT requires compiler and library support for the \
ISO C++ 2011 standard. This support is currently experimental, and must be \
disable with the -std=c++11 or -std=gnu++11 compiler options.\
If you must to compile this file, please delete this error message.

TCHAR * _tstrlwr(TCHAR * str) noexcept {
	for (TCHAR * _str=str;_str;_str++) if (*_str >= TEXT('A') && *_str<=TEXT('Z'))
	*_str=*_str-TEXT('A')+TEXT('a');
	return str;
}

#undef _tcslwr
#define _tcslwr _tstrlwr
#endif


#ifndef USEING_STRING_LIBRARY

//Function:_tstrcamp(const TCHAR*,const TCHAR*)
//return type:bool(base)
//Author:qdxllecrn
//E-mail:sweheartiii@hotmail.com
bool _tstrcamp(const TCHAR* ____str1,const TCHAR * ____str2){
	int sz1=_tcslen(____str1);
	int sz2=_tcslen(____str2);
	if (sz1!=sz2) return false;
	if (sz1 > 999 || sz2 > 999) abort();
	TCHAR ____tmp1[1000]=TEXT("");
	TCHAR ____tmp2[1000]=TEXT("");
	return (!_tcscmp(_tcslwr(_tcscpy(____tmp1,____str1)),_tcslwr(_tcscpy(____tmp2,____str2))))?true:false;
}

#else

#include <string>

//defined std::basic_string<TCHAR>==TCHARstr
//Function:_tstrcamp(const TCHARstr,const TCHARstr)
//return type:bool(base)
//Author:qdxllecrn
//E-mail:sweheartiii@hotmail.com

typedef std::basic_string<TCHAR> TCHARstr;
bool _tstrcamp(const TCHARstr ____str1,const TCHARstr ____str2){
	TCHARstr str1,str2;
	size_t len1,len2;
	if ((len1=____str1.length())!=(len2=____str1.length())) return false;
	str1.assign(____str1);
	str2.assign(____str2);
	for (size_t i=0;i<len1;i++){
		if (str1[i]>='A' && str1[i]<='Z')
			str1[i]=str1[i]-'A'+'a';
		if (str2[i]>='A' && str2[i]<='Z')
			str2[i]=str2[i]-'A'+'a';
	}
	return !str1.compare(str2)?true:false;
}
#endif
