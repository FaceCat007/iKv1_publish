/*基于捂脸猫FaceCat框架 v1.0
上海卷卷猫信息技术有限公司
 */

#ifndef __FCSTR_H__
#define __FCSTR_H__
#pragma once
#include "stdafx.h"
#include <tchar.h>

namespace FaceCat{
	class CodeConvert_Win{
	public:
		CodeConvert_Win(const char* input, unsigned int fromCodePage, unsigned int toCodePage);
		~CodeConvert_Win(){
			delete [] wcharBuf;        
			delete [] charBuf;    
			};    
			const char * toString(){    
				return charBuf;    
			};
	private:    
		  wchar_t * wcharBuf;    
		  char * charBuf;
	};

	class FCStr{
	public:
		static double round(double x); 
	public:
		static void ANSCToUnicode(string& out, const char* inSrc);
		static void contact(wchar_t *str, const wchar_t *str1, const wchar_t *str2, const wchar_t *str3);
		static String convertDoubleToStr(double value);
		static String convertFloatToStr(float value);
		static String convertIntToStr(int value);
		static String convertLongToStr(int64 value);
		static double convertStrToDouble(const String& str);
		static double convertStrToDouble(const wchar_t *str);
		static float convertStrToFloat(const String& str);
		static int convertStrToInt(const String& str);
		static int convertStrToInt(const wchar_t *str);
		static int64 convertStrToLong(const String& str);
		static int64 convertStrToLong(const wchar_t *str);
		static string getAppPath();
		static double getDateNum(int tm_year, int tm_mon, int tm_mday, int tm_hour, int tm_min, int tm_sec, int tm_msec);
        static void getDateByNum(double num, int *tm_year, int *tm_mon, int *tm_mday, int *tm_hour, int *tm_min, int *tm_sec, int *tm_msec);
		static void getFormatDate(double date, wchar_t *str);
		static String getFormatDate(const String& format, int year, int month, int day, int hour, int minute, int second);
		static void getValueByDigit(double value, int digit, wchar_t *str);
		static int hexToDec(const char *str);
		static string replace(const string& str, const string& src, const string& dest);
		static String replace(const String& str, const String& src, const String& dest);
		static float safeDoubleToFloat(double value, int digit);
		static vector<String> split(const String& str, const String& pattern);
		static String stringTowstring(const string& strSrc);
		static String toLower(const String& str);
		static String toUpper(const String& str);
		static void unicodeToANSC(string& out, const char* inSrc);
		static string urlEncode(const std::string& szToEncode);
		static string urlDecode(const std::string& szToDecode);
		static string wstringTostring(const String& strSrc);
	};
}
#endif