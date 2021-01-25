#include "..\\stdafx.h"
#include "FCKV3.h"
#include "io.h"
#include <direct.h>
#include <fstream>
#include <sys/stat.h>
#include "..\\FCFile.h"
#include "..\\FCStr.h"	


namespace FaceCat{
	DWORD WINAPI workerThread3(LPVOID lpParam){
		KVData3 *data = (KVData3*)lpParam;
		while(true){
			int state = data->checkLoop();
			if(state == -1){
				break;
			}else{
				Sleep(1000);
			}
		}
		return 0;
	}

	int hashKey3(const char* chPtr){
		int len = (int)strlen(chPtr);
		int hash = 0, offset = 0;
		int h = hash; 
		if (h == 0 && len > 0) {
			int off = offset; 
			for (int i = 0; i < len; i++) {
				h = 31 * h + chPtr[off++]; 							
			}
			hash = h;
		}
		return h;
	}

	KVData3::KVData3(){
		m_file = 0;	
		m_kv = 0;
		m_pos = 0;
		m_totalSize = 0;
		m_loaded = false;
		m_lastTime = ::GetTickCount();
	}

	KVData3::~KVData3(){
	}

	int KVData3::checkLoop(){
		int state = fallFile();
		if(m_kv->m_autoClose || m_kv->m_forceClose){
			DWORD nowTime = ::GetTickCount();
			int sub = nowTime - m_lastTime;
			if(sub > m_kv->getCloseTime() || m_kv->m_forceClose){
				closeCache();
				state = -1;
			}
		}
		return state;
	}

	void KVData3::closeCache(){
		printf("%d号长时间无人访问，自动关闭\r\n", m_pos);
		m_lock.lock();
		fallFile();
		int mapSize = m_map.size();
		for(int i = 0; i < mapSize; i++){
			delete m_map.getValue(i);
		}
		m_map.clear();
		m_emptyInfos.clear();	
		if(m_kv->isFall()){
			::CloseHandle(m_file);
			m_file = 0;
		}
		m_loaded = false;
		m_lock.unLock();
		m_lastTime = ::GetTickCount();
	}

	int KVData3::fallFile(){
		int state = 0;
		if(m_kv->isFall()){
			vector<KVWriteInfo3> writeInfos;
			m_lock2.lock();
			writeInfos = m_writeInfos;
			m_writeInfos.clear();
			m_lock2.unLock();
			if(writeInfos.size() > 0){
				vector<KVWriteInfo3>::iterator sIter = writeInfos.begin();
				for(; sIter != writeInfos.end(); ++sIter){
					KVWriteInfo3 writeInfo = *sIter;
					if(writeInfo.m_oldStart != -1){
						int kvSize = writeInfo.m_oldEnd - writeInfo.m_oldStart + 1;
						char *kv = new char[kvSize];
						memset(kv, '\0', kvSize);
						kv[0] = '0';
						int kvSize2 = kvSize - 1;
						memcpy(kv + 1, &kvSize2, 4); 
						SetFilePointer(m_file, writeInfo.m_oldStart ,0, FILE_BEGIN);
						DWORD dwBytesWritten = 0;
						WriteFile(m_file, kv, kvSize, &dwBytesWritten, NULL);
						delete[] kv;
						kv = 0;
					}
					if(writeInfo.m_start != -1){
						int kvSizeT =  writeInfo.m_end - writeInfo.m_start + 1;
						char *kv2 = new char[kvSizeT];
						memset(kv2, '\0', kvSizeT);
						kv2[0] = '1';
						int kvSizeT2 = kvSizeT - 1;
						memcpy(kv2 + 1, &kvSizeT2, 4); 
						int keyLength = (int)writeInfo.m_key.length() + 1;
						memcpy(kv2 + 5, &keyLength, 4);
						memcpy(kv2 + 9, writeInfo.m_key.c_str(), (int)writeInfo.m_key.length() + 1);
						memcpy(kv2 + 9 + keyLength, writeInfo.m_value.c_str(), (int)writeInfo.m_value.length() + 1);
						SetFilePointer(m_file, writeInfo.m_start ,0, FILE_BEGIN);
						DWORD dwBytesWritten = 0;
						WriteFile(m_file, kv2, kvSizeT, &dwBytesWritten, NULL);
						delete[] kv2;
						kv2 = 0;
					}
				}
				FlushFileBuffers(m_file);
				state = 1;
				m_lastTime = ::GetTickCount();
			}
		}
		return state;
	}

	string KVData3::getValue(const string& key){
		string value = "nil";
		m_lock.lock();
		if(!m_loaded){
			load(m_kv->m_number);
			CreateThread(NULL, 0, workerThread3, this, 0, 0);
		}
		FCValue3 *fcValue = m_map.get(key);
		if(fcValue){
			value = fcValue->m_text;
		}
		m_lock.unLock();
		m_lastTime = ::GetTickCount();
		return value;
	}

	int KVData3::getKv(int index, string *key, string *value){
		int state = 0;
		m_lock.lock();
		if(!m_loaded){
			load(m_kv->m_number);
			CreateThread(NULL, 0, workerThread3, this, 0, 0);
		}
		if(index < m_map.size()){
			*key = m_map.getKey(index);
			*value = m_map.getValue(index)->m_text;
			state = 1;
		}
		m_lock.unLock();
		m_lastTime = ::GetTickCount();
		return state;
	}

	int KVData3::getKvCount(){
		int count = 0;
		m_lock.lock(); 
		if(!m_loaded){
			load(m_kv->m_number);
			CreateThread(NULL, 0, workerThread3, this, 0, 0);
		}
		count = m_map.size();
		m_lock.unLock();
		m_lastTime = ::GetTickCount();
		return count;
	}

	void KVData3::load(int number){
		printf("%d号有人访问，重新开始工作\r\n", m_pos);
		if(m_kv->isFall()){
			m_file = 0;
			String strI = FCStr::convertIntToStr(m_pos);
			string sStrI = FCStr::wstringTostring(strI);
			string dir = m_kv->m_path;
			if(!FCFile::isDirectoryExist(dir.c_str())){
				FCFile::createDirectory(dir.c_str());
			}
			String strNumber = FCStr::convertIntToStr(number);
			string fPath = dir + "\\" + m_kv->m_name + "_" + FCStr::wstringTostring(strNumber) + "_" + sStrI + ".db";
			int size = FCFile::getFileLength(fPath.c_str());
			if(size > 0){
				m_totalSize = size;
				char *szContent = new char[size];
				memset(szContent, '\0', size);
				ifstream fs(fPath.c_str(), ios::binary); 
				if(fs){
					if(size > 0){
						while(!fs.eof()){
							fs.read(szContent, size); 
						}
					}
					fs.close();
				}
				int pos = 0;
				while(pos < size){
					int start = pos;
					char state[1] = {0};
					memcpy(state, szContent + pos, STATEFIELD);
					pos += 1;
					int totalLength = 0, keyLength = 0;
					memcpy(&totalLength, szContent + pos, 4);
					pos += 4;
					if(state[0] == '1'){
						memcpy(&keyLength, szContent + pos, 4);
						pos += 4;
						char *key = new char[keyLength];
						char *value = new char[totalLength - keyLength];
						memcpy(key, szContent + pos, keyLength);
						pos += keyLength;
						memcpy(value, szContent + pos, totalLength - keyLength);
						FCValue3 *fcValue = new FCValue3;
						fcValue->m_text = value;
						m_map.put(key, fcValue);
						fcValue->m_start = start;
						fcValue->m_end = start + totalLength;
						delete[] key;
						delete[] value;
						key = 0;
						value = 0;
					}else{
						EmptyInfo emptyInfo;
						emptyInfo.m_start = start;
						emptyInfo.m_end = start + totalLength;
						m_emptyInfos.put(start, emptyInfo);
					}
					pos = start + 1 + totalLength;
				}
				delete[] szContent;
				szContent = 0;
			}
			String wPath = FCStr::stringTowstring(fPath);
			m_file = CreateFile(wPath.c_str(), GENERIC_WRITE,          
			FILE_SHARE_READ,
			NULL,               
			OPEN_ALWAYS,        
			FILE_ATTRIBUTE_NORMAL, 
			NULL);
		}
		m_lastTime = ::GetTickCount();
		m_loaded = true;
	}

	void KVData3::removeValue(const string& key){
		m_lock.lock();
		if(!m_loaded){
			load(m_kv->m_number);
			CreateThread(NULL, 0, workerThread3, this, 0, 0);
		}
		FCValue3 *fcValue = m_map.get(key);
		if(fcValue){
			if(m_kv->isFall())
			{
				int oldStart = fcValue->m_start;
				int oldEnd = fcValue->m_end;
				EmptyInfo emptyInfo;
				emptyInfo.m_start = oldStart;
				emptyInfo.m_end = oldEnd;
				m_emptyInfos.put(oldStart, emptyInfo);
				KVWriteInfo3 writeInfo;
				writeInfo.m_key = key; 
				writeInfo.m_start = -1;
				writeInfo.m_end = -1;
				writeInfo.m_oldStart = oldStart;
				writeInfo.m_oldEnd = oldEnd;
				m_lock2.lock();
				m_writeInfos.push_back(writeInfo);
				m_lock2.unLock();
			}
			m_map.remove(key);
			delete fcValue;
			fcValue = 0;
		}
		m_lock.unLock();
		m_lastTime = ::GetTickCount();
	}

	void KVData3::setValue(const string& key, const string& value){
		m_lock.lock();
		if(!m_loaded){
			load(m_kv->m_number);
			CreateThread(NULL, 0, workerThread3, this, 0, 0);
		}
		FCValue3 *fcValue = m_map.get(key);
		if(!fcValue){
			fcValue = new FCValue3;
			fcValue->m_text = value;
			m_map.put(key, fcValue);
			if(m_kv->isFall()){
				int newLength = 9 + (int)key.length() + (int)value.length() + 2;
				int eSize = m_emptyInfos.size();
				int inputStart = -1, inputEnd = -1;
				for(int i = 0; i < eSize; i++){
					EmptyInfo emptyInfo = m_emptyInfos.getValue(i);
					int length = emptyInfo.m_end - emptyInfo.m_start + 1;
					if(length >= newLength){
						inputStart = emptyInfo.m_start;
						inputEnd = emptyInfo.m_end;
						m_emptyInfos.remove(emptyInfo.m_start);
						break;
					}
				}
				if(inputStart != -1){
					fcValue->m_start = inputStart;
					fcValue->m_end = inputEnd;
				}else{
					fcValue->m_start = m_totalSize;
					m_totalSize += newLength;
					fcValue->m_end = m_totalSize - 1;
				}
				KVWriteInfo3 writeInfo;
				writeInfo.m_key = key;
				writeInfo.m_value = value;
				writeInfo.m_start = fcValue->m_start;
				writeInfo.m_end = fcValue->m_end;
				writeInfo.m_oldStart = -1;
				writeInfo.m_oldEnd = -1;
				m_lock2.lock();
				m_writeInfos.push_back(writeInfo);
				m_lock2.unLock();
			}
		}
		else{
			int oldLength = 9 + (int)key.length() + (int)fcValue->m_text.length() + 2;
			fcValue->m_text = value;
			if(m_kv->isFall()){
				int newLength = 9 + (int)key.length() + (int)value.length() + 2;
				if(newLength > oldLength){
					int oldStart = fcValue->m_start;
					int oldEnd = fcValue->m_end;
					int eSize = m_emptyInfos.size();
					int inputStart = -1, inputEnd = -1;
					for(int i = 0; i < eSize; i++){
						EmptyInfo emptyInfo = m_emptyInfos.getValue(i);
						int length = emptyInfo.m_end - emptyInfo.m_start;
						if(length >= newLength){
							inputStart = emptyInfo.m_start;
							inputEnd = emptyInfo.m_end;
							m_emptyInfos.remove(emptyInfo.m_start);
							break;
						}
					}
					if(inputStart != -1){
						fcValue->m_start = inputStart;
						fcValue->m_end = inputEnd;
					}else{
						EmptyInfo emptyInfo;
						emptyInfo.m_start = oldStart;
						emptyInfo.m_end = oldEnd;
						m_emptyInfos.put(oldStart, emptyInfo);
						fcValue->m_start = m_totalSize;
						m_totalSize += newLength;
						fcValue->m_end = m_totalSize - 1;
					}
					KVWriteInfo3 writeInfo;
					writeInfo.m_key = key;
					writeInfo.m_value = value;
					writeInfo.m_start = fcValue->m_start;
					writeInfo.m_end = fcValue->m_end;
					writeInfo.m_oldStart = oldStart;
					writeInfo.m_oldEnd = oldEnd;
					m_lock2.lock();
					m_writeInfos.push_back(writeInfo);
					m_lock2.unLock();
				}else{
					KVWriteInfo3 writeInfo;
					writeInfo.m_key = key;
					writeInfo.m_value = value;
					writeInfo.m_start = fcValue->m_start;
					writeInfo.m_end = fcValue->m_end;
					writeInfo.m_oldStart = -1;
					writeInfo.m_oldEnd = -1;
					m_lock2.lock();
					m_writeInfos.push_back(writeInfo);
					m_lock2.unLock();
				}
			}
		}
		m_lock.unLock();
		m_lastTime = ::GetTickCount();
	}

	FCKV3::FCKV3(){
		m_autoClose = false;
		m_closeTime = 10000;
		m_fall = true;
		m_number = 100;
		m_forceClose = false;
	}

	FCKV3::~FCKV3(){
	}

	bool FCKV3::autoClose(){
		return m_autoClose;
	}

	void FCKV3::setAutoClose(bool autoClose){
		m_autoClose = autoClose;
	}

	int FCKV3::getCloseTime(){
		return m_closeTime;
	}

	void FCKV3::setCloseTime(int closeTime){
		m_closeTime = closeTime;
	}

	bool FCKV3::isFall(){
		return m_fall;
	}

	void FCKV3::setFall(bool fall){
		m_fall = fall;
	}

	string FCKV3::getName(){
		return m_name;
	}

	void FCKV3::setName(const string& name){
		m_name = name;
	}

	int FCKV3::getNumber(){
		return m_number;
	}

	void FCKV3::setNumber(int number){
		m_number = number;
	}

	string FCKV3::getPath(){
		return m_path;
	}

	void FCKV3::setPath(const string& path){
		m_path = path;
	}

	int FCKV3::close(){
		m_forceClose = true;
		while(true){
			bool outLoop = true;
			for(int i = 0; i < m_number; i++){
				if(m_datas[i]->m_loaded){
					outLoop = false;
				}
			}
			if(outLoop){
				break;
			}
			::Sleep(10);
		}
		::Sleep(1000);
		for(int i = 0; i < m_number; i++){
			delete m_datas[i];
			m_datas[i] = 0;
		}
		delete[] m_datas;
		m_datas = 0;
		return 0;
	}

	string FCKV3::getValue(const string& key){
		int mod = abs((int)(hashKey3(key.c_str()) % m_number));
		return m_datas[mod]->getValue(key);
	}

	int FCKV3::getKv(int index, int number, string *key, string *value){
		return m_datas[number]->getKv(index, key, value);
	}

	int FCKV3::getKvCount(int number){
		return m_datas[number]->getKvCount();
	}

	void FCKV3::load(){
		string dir = m_path;
		if(!FCFile::isDirectoryExist(dir.c_str())){
			FCFile::createDirectory(dir.c_str());
		}
		string vFile = dir + "\\info.txt";
		int oldNumber = m_number;
		if(FCFile::isFileExist(vFile.c_str())){
			string content;
			FCFile::read(vFile.c_str(), &content);
			oldNumber = atoi(content.c_str());
		}
		m_datas = new KVData3*[m_number];
		for(int i = 0; i < m_number; i++){
			KVData3 *kvData = new KVData3;
			m_datas[i] = kvData;
			kvData->m_pos = i;
			kvData->m_kv = this;
		}
		if(oldNumber != m_number){
			KVData3 **oldDatas = new KVData3*[oldNumber];
			for(int j = 0; j < oldNumber; j++){
				KVData3 *kvDataOld = new KVData3;
				oldDatas[j] = kvDataOld;
				kvDataOld->m_pos = j;
				kvDataOld->m_kv = this;
				kvDataOld->load(oldNumber);
				int kvDataCount = kvDataOld->getKvCount();
				for(int k = 0; k < kvDataCount; k++){
					string key, value;
					kvDataOld->getKv(k, &key, &value);
					int mod = abs((int)(hashKey3(key.c_str()) % m_number));
					m_datas[mod]->setValue(key, value);
				}
			}	
			for(int j = 0; j < oldNumber; j++){
				KVData3 *kvDataOld = oldDatas[j];
				kvDataOld->closeCache();
				String strNumber = FCStr::convertIntToStr(oldNumber);
				String strI = FCStr::convertIntToStr(j);
				string sStrI = FCStr::wstringTostring(strI);
				string fPath = dir + "\\" + m_name + "_" + FCStr::wstringTostring(strNumber) + "_" + sStrI + ".db";
				FCFile::removeFile(fPath.c_str());
				delete kvDataOld;
			}
			delete[] oldDatas;
			oldDatas = 0;
		}
		if(m_fall){
			char szNumber[100] = {0};
			sprintf_s(szNumber, 99, "%d", m_number);
			FCFile::write(vFile.c_str(), szNumber);
		}
	}

	void FCKV3::removeValue(const string& key){
		int mod = abs((int)(hashKey3(key.c_str()) % m_number));
		m_datas[mod]->removeValue(key);
	}

	void FCKV3::setValue(const string& key, const string& value){
		int mod = abs((int)(hashKey3(key.c_str()) % m_number));
		m_datas[mod]->setValue(key, value);
	}
}