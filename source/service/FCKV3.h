/*基于捂脸猫FaceCat框架 v1.0
上海卷卷猫信息技术有限公司
 */
#ifndef __FCKV3_H__
#define __FCKV3_H__
#pragma once
#include "..\\stdafx.h"
#include "..\\FCLock.h"
#include "ArrayList.h"

#define STATEFIELD 1

namespace FaceCat{
	class FCKV3;

	struct EmptyInfo{
	public:
		int m_start;
		int m_end;
	};

	class KVWriteInfo3{
	public:
		string m_key;
		string m_value;
	public:
		int m_end;
		int m_start;
	public:
		int m_oldStart;
		int m_oldEnd;
	};

	class FCValue3{
	public:
		int m_end;
		int m_start;
		string m_text;
	public:
		FCValue3(){
		}
	};

	class KVData3{
	public:
		KVData3();
		~KVData3();
		int m_count;
		HashMap<int, EmptyInfo> m_emptyInfos;
		HANDLE m_file;
		DWORD m_lastTime;
		bool m_loaded;
		FCLock m_lock;
		FCLock m_lock2;
		FCKV3 *m_kv;
		HashMap<string, FCValue3*> m_map;
		int m_pos;
		int m_totalSize;
		vector<KVWriteInfo3> m_writeInfos;
	public:
		int checkLoop();
		void closeCache();
		int fallFile();
		string getValue(const string& key);
		int getKv(int index, string *key, string *value);
		int getKvCount();
		void load(int number);
		void removeValue(const string& key);
		void setValue(const string& key, const string& value);
	};

	class FCKV3{
	public:
		bool m_autoClose;
		int m_closeTime;
		KVData3 **m_datas;
		bool m_fall;
		bool m_forceClose;
		string m_name;
		int m_number;
		string m_path;
	public:
		FCKV3();
		virtual ~FCKV3();
		bool autoClose();
		void setAutoClose(bool autoClose);
		int getCloseTime();
		void setCloseTime(int closeTime);
		bool isFall();
		void setFall(bool fall);
		string getName();
		void setName(const string& name);
		int getNumber();
		void setNumber(int number);
		string getPath();
		void setPath(const string& path);
	public:
		int close();
		string getValue(const string& key);
		int getKv(int index, int number, string *key, string *value);
		int getKvCount(int number);
		void load();
		void removeValue(const string& key);
		void setValue(const string& key, const string& value);
	};
}

#endif