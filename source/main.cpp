// iCTP_cpp.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "service\\FCKV3.h"
#include "service\\ArrayList.h"
#include "FCFile.h"
#include "FCStr.h"
using namespace FaceCat;

#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

vector<FCKV3*> m_kvs;

extern "C" __declspec(dllexport) int initKv(const char *path, const char *name, int number){
	FCKV3 *kv = new FCKV3;
	kv->setName(name);
	kv->setPath(path);
	kv->setNumber(number);
	kv->load();
	m_kvs.push_back(kv);
	return (int)m_kvs.size() - 1;
}

extern "C" __declspec(dllexport) int closeKv(int id){
	if(id >= 0 && id < (int)m_kvs.size()){
		FCKV3 *kv = m_kvs[id];
		if(kv){
			m_kvs[id] = 0;
			kv->close();
			delete kv;
			kv = 0;
			return 1;
		}else{
			return 0;
		}
	}else{
		return 0;
	}
}

extern "C" __declspec(dllexport) int closeAllKvs(){
	int state = 0;
	for(int i = 0; i < (int)m_kvs.size(); i++){
		state += closeKv(i);
	}
	return state;
}

extern "C" __declspec(dllexport) int getKv(int id, const char *key, char *value){
	if(id >= 0 && id < (int)m_kvs.size()){
		string strValue = m_kvs[id]->getValue(key);
		strcpy(value, strValue.c_str());
		return 1;
	}else{
		return 0;
	}
}

extern "C" __declspec(dllexport) int getKvCount(int id, int number){
	if(id >= 0 && id < (int)m_kvs.size()){
		return m_kvs[id]->getKvCount(number);
	}else{
		return 0;
	}
}

extern "C" __declspec(dllexport) int getKv2(int id, int index, int number, char *key, char *value){
	if(id >= 0 && id < (int)m_kvs.size()){
		string strKey, strValue;
		int state = m_kvs[id]->getKv(index, number, &strKey, &strValue);
		strcpy(key, strKey.c_str());
		strcpy(value, strValue.c_str());
		return state;
	}else{
		return 0;
	}
}

extern "C" __declspec(dllexport) int putKv(int id, const char *key, char *value){
	if(id >= 0 && id < (int)m_kvs.size()){
		m_kvs[id]->setValue(key, value);
		return 1;
	}else{
		return 0;
	}
}

extern "C" __declspec(dllexport) int removeKv(int id, const char *key){
	if(id >= 0 && id < (int)m_kvs.size()){
		m_kvs[id]->removeValue(key);
		return 1;
	}else{
		return 0;
	}
}


//创建缓存
extern "C" __declspec(dllexport) char* createBuffer(int len){
	return new char[len];
}

//删除缓存
extern "C" __declspec(dllexport) int destoryBuffer(char *buffer){
	delete buffer;
	buffer = 0;
	return 1;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int id = initKv("C:\\Miao\\a", "ddd", 10);
	for(int i = 0; i < 1000000; i++){
		char sz[10] = {0};
		char sz2[10] = {0};
		sprintf_s(sz, 9, "%d", i);
		sprintf_s(sz2, 9, "%d", i);
		putKv(id, sz, sz2);
	}
	char sz3[100] = {0};
	getKv(id, "1", sz3);
	closeAllKvs();

	while(true)
	{
		char *input = new char[1024];
		cin >> input;
		if(strcmp(input, "exit") == 0)
		{
			break;
		}
	}
	return 0;
}

