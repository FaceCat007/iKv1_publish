#include "stdafx.h"
#include "FCStr.h"

namespace FaceCat{
	CodeConvert_Win::CodeConvert_Win( const char* input, unsigned int fromCodePage, unsigned int toCodePage ){
		int len = MultiByteToWideChar(fromCodePage, 0, input, -1, 0, 0);
		wcharBuf = new wchar_t[len + 1];
		memset(wcharBuf,0,sizeof(wchar_t)*(len + 1));
		MultiByteToWideChar(fromCodePage, 0, input, -1, wcharBuf, len);
		len = WideCharToMultiByte(toCodePage, 0, wcharBuf, -1, 0, 0, 0, 0);
		charBuf = new char[len + 1];
		memset(charBuf,0,sizeof(char)*(len + 1));
		WideCharToMultiByte(toCodePage, 0, wcharBuf, -1, charBuf, len, 0, 0);
	}

	double FCStr::round(double x){     
		int sa = 0, si = 0;  
		if(x == 0.0){
			return 0.0; 
		}
		else{
			if(x > 0.0){
				sa = (int)x;   
				si = (int)(x + 0.5);        
				if(sa == floor((double)si)){
					return sa;    
				}
				else{
					return sa + 1; 
				}
			}       
			else{
				sa = (int)x;   
				si = (int)(x - 0.5);      
				if(sa == ceil((double)si)){
					return sa;       
				}
				else{
					return sa - 1;      
				}
			}
		}
	}

	void FCStr::ANSCToUnicode(string& out, const char* inSrc){
		if (!inSrc){
			return ;
		}
		out = CodeConvert_Win(inSrc, CP_UTF8, CP_ACP).toString();
	}

	void FCStr::contact(wchar_t *str, const wchar_t *str1, const wchar_t *str2, const wchar_t *str3){
		str[0] = _T('\0');
		lstrcat(str, str1);
		if(lstrlen(str2) > 0){
			lstrcat(str, str2);
		}
		if(lstrlen(str3) > 0){
			lstrcat(str, str3);
		}
	}

	String FCStr::convertDoubleToStr(double value){
		wchar_t str[100] ={0};
		_stprintf_s(str, 99, L"%f", value);
		return str;
	}

	String FCStr::convertFloatToStr(float value){
		wchar_t str[100] ={0};
		_stprintf_s(str, 99, L"%f", value);
		return str;
	}

	String FCStr::convertIntToStr(int value){
		wchar_t str[100] ={0};
		_stprintf_s(str, 99, L"%d", value);
		return str;
	}

	String FCStr::convertLongToStr(int64 value){
		wchar_t str[100] ={0};
		_stprintf_s(str, 99, L"%ld", value);
		return str;
	}

	double FCStr::convertStrToDouble(const String& str){
		return _wtof(str.c_str());
	}

	double FCStr::convertStrToDouble(const wchar_t *str){
		return _wtof(str);
	}

	float FCStr::convertStrToFloat(const String& str){
		return (float)_wtof(str.c_str());
	}

	int FCStr::convertStrToInt(const String& str){
		return _wtoi(str.c_str());
	}

	int FCStr::convertStrToInt(const wchar_t *str){
		return _wtoi(str);
	}

	int64 FCStr::convertStrToLong(const String& str){
		return _wtol(str.c_str());
	}

	int64 FCStr::convertStrToLong(const wchar_t *str){
		return _wtol(str);
	}

	string FCStr::getAppPath(){
		char exeFullPath[MAX_PATH]; 
		string strPath = "";
		GetModuleFileNameA(0, exeFullPath, MAX_PATH);
		strPath = (string)exeFullPath; 
		int pos = (int)strPath.rfind('\\', strPath.length());
		return strPath.substr(0, pos);
	}

	    inline int isLeapYead(int year){
        return( (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
    }
    
    const int evenyMonths[2][12] ={{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
    };
    
    double FCStr::getDateNum(int tm_year, int tm_mon, int tm_mday, int tm_hour, int tm_min, int tm_sec, int tm_msec){
        double tn = 0;
        if ((tm_mon -= 2) <= 0){
            tm_mon += 12;
            tm_year -= 1;
        }
        double year = tm_year / 4 - tm_year / 100 + tm_year / 400 + 367 * tm_mon / 12 + tm_mday;
        double month = tm_year * 365 - 719499;
        tn = ((year + month) * 24 * 60 * 60) + tm_hour * 3600 + tm_min * 60 + tm_sec;
        return (long long) tn + tm_msec / 1000;
    }
    
    void FCStr::getDateByNum(double num, int *tm_year, int *tm_mon, int *tm_mday, int *tm_hour, int *tm_min, int *tm_sec, int *tm_msec){
        long tn = (long)num;
        int dayclock = (int)(tn % 86400);
        int dayno = (int)(tn / 86400);
        int year = 1970;
        *tm_sec = dayclock % 60;
        *tm_min = (dayclock % 3600) / 60;
        *tm_hour = dayclock / 3600;
        int yearSize = 0;
        while (dayno >= (yearSize = (isLeapYead(year) == 1 ? 366 : 365))){
            dayno -= yearSize;
            year++;
        }
        *tm_year = year;
        int month = 0;
        while (dayno >= evenyMonths[isLeapYead(year)][month]){
            dayno -= evenyMonths[isLeapYead(year)][month];
            month++;
        }
        *tm_mon = month + 1;
        *tm_mday = dayno + 1;
        *tm_msec = (long)((num * 1000 - floor(num) * 1000));
    }

	void FCStr::getFormatDate(double date, wchar_t *str){
		int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0, msecond = 0;
		getDateByNum(date, &year, &month, &day, &hour, &minute, &second, &msecond);
		if(hour != 0){
			_stprintf_s(str, 99, L"%02d:%02d", hour, minute);
		}
		else{
			_stprintf_s(str, 99, L"%d-%02d-%02d", year, month, day);
		}
	}

	String FCStr::getFormatDate(const String& format, int year, int month, int day, int hour, int minute, int second){
		String strDate = format;
		wchar_t sz[100] ={0};
		_stprintf_s(sz, 99, L"%d", year);
		String strYear = sz;
		memset(sz, '\0', 100);
		_stprintf_s(sz, 99, L"%02d", month);
		String strMonth = sz;
		memset(sz, '\0', 100);
		_stprintf_s(sz, 99, L"%02d", day);
		String strDay = sz;
		memset(sz, '\0', 100);
		_stprintf_s(sz, 99, L"%d", day);
		String strHour = sz;
		memset(sz, '\0', 100);
		_stprintf_s(sz, 99, L"%02d", minute);
		String strMinute = sz;
		memset(sz, '\0', 100);
		_stprintf_s(sz, 99, L"%02d", second);
		String strSecond = sz;
		if((int)format.find(L"yyyy") != -1){
			strDate = FCStr::replace(strDate, L"yyyy", strYear);
		}
		if((int)format.find(L"MM") != -1){
			strDate = FCStr::replace(strDate, L"MM", strMonth);
		}
		if((int)format.find(L"dd") != -1){
			strDate = FCStr::replace(strDate, L"dd", strDay);
		}
		if((int)format.find(L"HH") != -1){
			strDate = FCStr::replace(strDate, L"HH", strHour);
		}
		if((int)format.find(L"mm") != -1){
			strDate = FCStr::replace(strDate, L"mm", strMinute);
		}
		if((int)format.find(L"ss") != -1){
			strDate = FCStr::replace(strDate, L"ss", strSecond);
		}
		return strDate;
	}

	void FCStr::getValueByDigit(double value, int digit, wchar_t *str){
		if(!_isnan(value)){
			if(digit == 0){
				double newValue = round(value);
				if(abs(value - newValue) < 1){
					value = newValue;
				}
			}
			switch(digit){
			case 0:
				_stprintf_s(str, 99, L"%d", (int)value);
				break;
			case 1:
				_stprintf_s(str, 99, L"%.1f", value);
				break;
			case 2:
				_stprintf_s(str, 99, L"%.2f", value);
				break;
			case 3:
				_stprintf_s(str, 99, L"%.3f", value);
				break;
			case 4:
				_stprintf_s(str, 99, L"%.4f", value);
				break;
			case 5:
				_stprintf_s(str, 99, L"%.5f", value);
				break;
			case 6:
				_stprintf_s(str, 99, L"%.6f", value);
				break;
			case 7:
				_stprintf_s(str, 99, L"%.7f", value);
				break;
			case 8:
				_stprintf_s(str, 99, L"%.8f", value);
				break;
			case 9:
				_stprintf_s(str, 99, L"%.9f", value);
				break;
			default:
				_stprintf_s(str, 99, L"%f", value);
			}
		}
		str = 0;
	}

	int FCStr::hexToDec(const char *str){
		bool IsHex = false;
		int result = 0;
		int i = 0, szLen = 0, nHex;
		if(str[0] == '0'){
			if((str[1] == 'x') || (str[1] == 'X')){
				IsHex = true;
				i = 2;
			}
		}
	    szLen = strlen(str);
	    nHex = '0';
	    for (; i < szLen; i++){
			if(IsHex){
				if ((str[i] >= '0') && (str[i] <= '9')){
					nHex = '0';
				}
				else if ((str[i] >= 'A') && (str[i] <= 'F')){
					nHex = 'A' - 10;
				}
		        else if ((str[i] >= 'a') && (str[i] <= 'f')){
					nHex = 'a' - 10;
				}
				else{
					return 0;
				}
				result = result * 16 + str[i] - nHex;
			}
			else{
				if ((str[i] < '0') || (str[i] > '9')){
					return 0;
				}
				result = result * 10 + str[i] - nHex;
			}
		}
		return result;
	}

	string FCStr::replace(const string& str, const string& src, const string& dest)
	{
		string newStr = str;
		std::string::size_type pos = 0;
		while( (pos = newStr.find(src, pos)) != string::npos )
		{
			newStr.replace(pos, src.length(), dest);
			pos += dest.length();
		}
		return newStr;
	}

	String FCStr::replace(const String& str, const String& src, const String& dest){
		String newStr = str;
		String::size_type pos = 0;
		while( (pos = newStr.find(src, pos)) != string::npos ){
			newStr.replace(pos, src.length(), dest);
			pos += dest.length();
		}
		return newStr;
	}

	float FCStr::safeDoubleToFloat(double value, int digit){
		wchar_t szValue[100] ={0};
		getValueByDigit(value, digit, szValue);
		return convertStrToFloat(szValue);
	}

	vector<String> FCStr::split(const String& str, const String& pattern){
		 int pos = -1;
		 vector<String> result;
		 String newStr = str;
		 newStr += pattern;
		 int size = (int)str.size();
		 for(int i = 0; i < size; i++){
			pos = (int)newStr.find(pattern, i);
			if((int)pos <= size){
				String s = newStr.substr(i, pos - i);
				if(s.length() > 0){
					result.push_back(s);
				}
				i = pos + (int)pattern.size() - 1;
			}
		}
		return result;
	}

	String FCStr::stringTowstring(const string& strSrc){
		int  unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, strSrc.c_str(), -1, 0, 0);
		wchar_t *pUnicode = new  wchar_t[unicodeLen + 1];
		memset(pUnicode,0,(unicodeLen + 1) * sizeof(wchar_t));
		::MultiByteToWideChar(CP_ACP, 0, strSrc.c_str(), - 1, (LPWSTR)pUnicode, unicodeLen);
		String strDest = ( wchar_t* )pUnicode;
		delete[] pUnicode;
		return strDest;
	}

	String FCStr::toLower(const String& str){
		String lowerStr = str;
		transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), tolower);
		return lowerStr;
	}

	String FCStr::toUpper(const String& str){
		String upperStr = str;
		transform(upperStr.begin(), upperStr.end(), upperStr.begin(), toupper);
		return upperStr;
	}

	void FCStr::unicodeToANSC(string& out, const char* inSrc){
		if (!inSrc){
			return ;
		}
		out = CodeConvert_Win(inSrc, CP_ACP, CP_UTF8).toString();
	}

	string FCStr::urlEncode(const std::string& szToEncode)
	{
		string src = szToEncode;  
		char hex[] = "0123456789ABCDEF";  
		string dst; 
		for (size_t i = 0; i < src.size(); ++i)  
		{  
			unsigned char cc = src[i];  
			if ( cc >= 'A' && cc <= 'Z'   
					 || cc >='a' && cc <= 'z'  
					 || cc >='0' && cc <= '9'  
					 || cc == '.'  
					 || cc == '_'  
					 || cc == '-'  
					 || cc == '*')  
			{  
				if (cc == ' ')  
				{  
					dst += "+";  
				}  
				else  
					dst += cc;  
			}  
			else  
			{  
				unsigned char c = static_cast<unsigned char>(src[i]);  
				dst += '%';  
				dst += hex[c / 16];  
				dst += hex[c % 16];  
			}  
		}  
		return dst;  
	}

	string FCStr::urlDecode(const std::string& szToDecode)
	{
		string result;  
		int hex = 0;  
		for (size_t i = 0; i < szToDecode.length(); ++i)  
		{  
			switch (szToDecode[i])  
			{  
			case '+':  
				result += ' ';  
				break;  
			case '%':  
				if (isxdigit(szToDecode[i + 1]) && isxdigit(szToDecode[i + 2]))  
				{  
					std::string hexStr = szToDecode.substr(i + 1, 2);  
					hex = strtol(hexStr.c_str(), 0, 16);  
					//字母和数字[0-9a-zA-Z]、一些特殊符号[$-_.+!*'(),] 、以及某些保留字[$&+,/:;=?@]  
					//可以不经过编码直接用于URL  
					if (!((hex >= 48 && hex <= 57) || //0-9  
						(hex >=97 && hex <= 122) ||   //a-z  
						(hex >=65 && hex <= 90) ||    //A-Z  
						//一些特殊符号及保留字[$-_.+!*'(),]  [$&+,/:;=?@]  
						hex == 0x21 || hex == 0x24 || hex == 0x26 || hex == 0x27 || hex == 0x28 || hex == 0x29  
						|| hex == 0x2a || hex == 0x2b|| hex == 0x2c || hex == 0x2d || hex == 0x2e || hex == 0x2f  
						|| hex == 0x3A || hex == 0x3B|| hex == 0x3D || hex == 0x3f || hex == 0x40 || hex == 0x5f  
						))  
					{  
						result += char(hex);  
						i += 2;  
					}  
					else result += '%';  
				}else {  
					result += '%';  
				}  
				break;  
			default:  
				result += szToDecode[i];  
				break;  
			}  
		}  
		return result;  
	}

	string FCStr::wstringTostring(const String& strSrc){
		int iTextLen = WideCharToMultiByte(CP_ACP, 0, strSrc.c_str(), -1, 0, 0, 0, 0);
		char *pElementText = new char[iTextLen + 1];
		memset( ( void* )pElementText, 0, sizeof( char ) * ( iTextLen + 1 ) );
		::WideCharToMultiByte(CP_ACP, 0, strSrc.c_str(), - 1, pElementText, iTextLen, 0, 0);
		string strDest = pElementText;
		delete[] pElementText;
		return strDest;
	}
}