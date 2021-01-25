/*基于捂脸猫FaceCat框架 v1.0
上海卷卷猫信息技术有限公司
 */

#ifndef __CLIST_H__
#define __CLIST_H__
#pragma once
#include "..\\stdafx.h"

namespace FaceCat{
	template <class numtype>
	/*
	* 自定义集合
	*/
	class ArrayList{
	public:
		/*
		* 数组
		*/
		numtype *m_ary;
		/*
		* 大小
		*/
		int m_size;
		/*
		* 容量
		*/
		int m_capacity;
		/*
		* 模式
		*/
		int m_mode;
		/*
		* 增长步长
		*/
		int m_step;
	public:
		/*
		* 构造函数
		*/
		ArrayList(){
			m_size = 0;
			m_ary = 0;
			m_capacity = 4;
			m_step = 4;
			m_mode = 0;
		}
		/*
		* 构造函数
		* @param capacity 容量
		*/
		ArrayList(int capacity){
			m_size = 0;
            m_ary = 0;
            m_capacity = capacity;
            m_step = 4;
			m_mode = 0;
		}
		/*
		* 添加
		* @param value 数据
		*/
		void add(numtype value){
			m_size += 1;
			if(!m_ary){
				if(m_mode == 0){
					m_ary = new numtype[m_capacity];
				}
				else{
					m_ary = (numtype*)VirtualAlloc(0, sizeof(numtype) * m_capacity, MEM_COMMIT, PAGE_READWRITE); 
				}
			}
			else{
				if(m_size > m_capacity){
					if(m_mode == 0){
						m_capacity = (int)(m_size * 1.5);
						numtype *newAry = new numtype[m_capacity];
						for(int i = 0;i < m_size - 1; i++){
							newAry[i] = m_ary[i];
						}
						delete[] m_ary;
						m_ary = newAry;
					}
					else{
						m_capacity += m_step;
						numtype *newAry = (numtype*)VirtualAlloc(m_ary, sizeof(numtype) * m_capacity, MEM_COMMIT, PAGE_READWRITE);
						if(newAry){
							m_ary = newAry;
						}
						else{
							newAry = (numtype*)VirtualAlloc(0, sizeof(numtype) * m_capacity, MEM_COMMIT, PAGE_READWRITE);
							MoveMemory(newAry, m_ary, sizeof(numtype) * (m_size - 1));
							VirtualFree(m_ary, 0, MEM_RELEASE);
							m_ary = newAry;
						}
					}
				}
			}
			m_ary[m_size - 1] = value;
		}

		/*
		* 批量添加
		* @param ary 数据
		* @param size 大小
		*/
		void addranges(numtype *ary, int size){
            m_ary = ary;
			m_size = size;
            m_capacity = m_size;
            m_step = 4;
		}
		/*
		* 获取容量
		*/
		int capacity(){
			return m_capacity;
		}
		/*
		* 清除
		*/
		void clear(){
			if(m_ary){
				if(m_mode == 0){
					delete[] m_ary;
				}
				else{
					VirtualFree(m_ary, 0, MEM_RELEASE);
				}
				m_ary = 0;
			}
			m_size = 0;
		}
		/*
		* 获取数据
		*/
		numtype get(int index){
			return m_ary[index];
		}
		/*
		* 插入数据
		* @param index 索引
		* @param value 数据
		*/
		void insert(int index,numtype value){
			m_size += 1;
			if(!m_ary){
				if(m_mode == 0){
					m_ary = new numtype[m_capacity];
				}
				else{
					m_ary = (numtype*)VirtualAlloc(0, sizeof(numtype) * m_capacity, MEM_COMMIT, PAGE_READWRITE); 
				}
			}
			else{
				bool build = false;
				if(m_size > m_capacity){
					if(m_mode == 0){
						m_capacity = (int)(m_size * 1.5);
						numtype *newAry = new numtype[m_capacity];
						for(int i = 0;i < m_size - 1;i++){
							if(i < index){
								newAry[i] = m_ary[i];
							}
							else if( i >= index){
								newAry[i + 1] = m_ary[i];
							}
						}
						delete[] m_ary;
						m_ary = newAry;
						build = true;
					}
					else{
						m_capacity += m_step;
						numtype *newAry = (numtype*)VirtualAlloc(m_ary, sizeof(numtype) * m_capacity, MEM_COMMIT, PAGE_READWRITE);
						if(newAry){
							m_ary = newAry;
						}
						else{
							newAry = (numtype*)VirtualAlloc(0, sizeof(numtype) * m_capacity, MEM_COMMIT, PAGE_READWRITE);
							MoveMemory(newAry, m_ary, sizeof(numtype) * (m_size - 1));
							VirtualFree(m_ary, 0, MEM_RELEASE);
							m_ary = newAry;
						}
					}
				}
				if(!build){
					numtype last;
					for(int i = index; i < m_size; i++){
						if(i == index){
							last = m_ary[i];
						}
						else if(i > index){
							numtype temp = m_ary[i];
							m_ary[i] = last;
							last = temp;
						}
					}
				}
			}
			m_ary[index] = value;
		}
		/*
		* 移除数据
		* @param index 索引
		*/
		void removeAt(int index){
			m_size -= 1;
			for(int i = index;i < m_size; i++){
				m_ary[i] = m_ary[i + 1];
			}
			bool reduce = false;
            if(m_mode == 0){
                if(m_capacity > 4 && m_size > 0){
                    if(m_capacity > (int)(m_size * 1.5)){
                        m_capacity = (int)(m_size * 1.5);
                        reduce = true;
                    }
                }
            }
            else{
                if(m_capacity - m_size > m_step){
                    m_capacity -= m_step;
                    reduce = true;
                }
            }
			if(reduce){
				if(m_capacity > 0){
					if(m_mode == 0){
						numtype *newAry = new numtype[m_capacity];
						for(int i = 0;i < m_size; i++){
							newAry[i] = m_ary[i];
						}
						delete[] m_ary;
						m_ary = newAry;
					}
					else{
						numtype *newAry = (numtype*)VirtualAlloc(m_ary, sizeof(numtype) * m_capacity, MEM_COMMIT, PAGE_READWRITE);
						if(newAry){
							m_ary = newAry;
						}
						else{
							newAry = (numtype*)VirtualAlloc(0, sizeof(numtype) * m_capacity, MEM_COMMIT, PAGE_READWRITE);
							MoveMemory(newAry, m_ary, sizeof(numtype) * m_size);
							VirtualFree(m_ary, 0, MEM_RELEASE);
							m_ary = newAry;
						}
					}
				}
				else{
					if(m_ary){
						if(m_mode == 0){
							delete[] m_ary;
						}
						else{
							VirtualFree(m_ary, 0, MEM_RELEASE);
						}
						m_ary = 0;
					}
				}
			}
		}
		/*
		* 设置数据
		* @param index 索引
		* @param value 值
		*/
		void set(int index,numtype value){
			m_ary[index] = value;
		}
		/*
		* 设置容量
		* @param capacity 容量
		*/
		void setCapacity(int capacity){
			m_capacity = capacity;
			if(m_ary){
				if(m_mode == 0){
					numtype *newAry = new numtype[m_capacity];
					for(int i = 0; i < m_size - 1; i++){
						newAry[i] = m_ary[i];
					}
					delete[] m_ary;
					m_ary = newAry;
				}
				else{
					numtype *newAry = (numtype*)VirtualAlloc(m_ary, sizeof(numtype) * m_capacity, MEM_COMMIT, PAGE_READWRITE);
					if(newAry){
						m_ary = newAry;
					}
					else{
						newAry = (numtype*)VirtualAlloc(0, sizeof(numtype) * m_capacity, MEM_COMMIT, PAGE_READWRITE);
						MoveMemory(newAry, m_ary, sizeof(numtype) * (m_size - 1));
						VirtualFree(m_ary, 0, MEM_RELEASE);
						m_ary = newAry;
					}
				}
			}
		}
		/*
		* 设置步长
		* @param step 步长
		*/
		void setStep(int step){
			m_step = step;
		}
		/*
		* 获取尺寸
		* @param step 尺寸
		*/
		int size(){
			return m_size;
		}
		/*
		* 析构函数
		*/
		virtual ~ArrayList(){
			clear();
		}
		/*
		* 拷贝构造函数
		*/
		ArrayList(const ArrayList& rhs){
            if (this != &rhs) {
                m_size = rhs.m_size;
                m_capacity = rhs.m_capacity;
                m_mode = rhs.m_mode;
                m_step = rhs.m_step;
                if(m_mode == 0){
                    m_ary = new numtype[m_capacity];
                }
                else{
                    m_ary = (numtype*)malloc(sizeof(numtype) * m_capacity);
                }
                for (int i = 0; i < m_size; i++){
                    m_ary[i] = rhs.m_ary[i];
                }
            }
        }
		/*
		* 拷贝函数
		*/
		const ArrayList& operator=(const ArrayList& rhs){
            if (this != &rhs) {
                clear();
                m_size = rhs.m_size;
                m_capacity = rhs.m_capacity;
                m_mode = rhs.m_mode;
                m_step = rhs.m_step;
				if(m_mode == 0){
					m_ary = new numtype[m_capacity];
				}
				else{
					m_ary = (numtype*)VirtualAlloc(0, sizeof(numtype) * m_capacity, MEM_COMMIT, PAGE_READWRITE); 
				}
                for (int i = 0; i < m_size; i++){
                    m_ary[i] = rhs.m_ary[i];
                }
            }
            return *this;
        }
	};

	/*
	* 自定义哈希表
	*/
	template <class KEY,class VALUE>
	class HashMap{
	private:
		/*
		* 获取int的哈希
		*/
		int hashKey(int key){
			return key;
		}
		/*
		* 获取char*的哈希
		*/
		int hashKey(char* chPtr){
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
		/*
		* 获取string的哈希
		*/
		int hashKey(string key){
			const char *chPtr = key.c_str();
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
		/*
		* 获取wstring上的哈希
		*/
		int hashKey(wstring pKey){
			string sKey = FCStr::wstringTostring(pKey);
			const char *chPtr = sKey.c_str();
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
		/*
		* 获取double的哈希
		*/
		int hashKey(double key){
			if (key == 0.0){
				return 0;
			}
			long num2 = *((long*) &key);
			return (((int) num2) ^ ((int) (num2 >> 0x20)));
		}
	protected:
		/*
		* 哈希列表
		*/
		ArrayList<int> m_hashs;
		/*
		* 键的列表
		*/
		ArrayList<KEY> m_keys;
		/*
		* 值的列表
		*/
		ArrayList<VALUE> m_rows;
		/*
		* 添加哈希值
		*/
		int addHashCode(int hashCode){
			if(m_hashs.size() == 0 || hashCode > m_hashs.get((int)m_hashs.size() - 1)){
				m_hashs.add(hashCode);
				return m_hashs.size() - 1;
			}
			else{
				int begin = 0;
				int end = m_rows.size() - 1;
				int sub = end - begin;
				while(sub > 1){
					int half = begin + sub / 2;
					int hf = m_hashs.get(half);
					if(hf >= hashCode){
						end = half;
					}
					else if(hf <= hashCode){
						begin = half;
					}
					sub = end - begin;
				}
				if(hashCode < m_hashs.get(begin)){
					m_hashs.insert(begin, hashCode);
					return begin;
				}
				else if(hashCode > m_hashs.get(end)){
					m_hashs.insert(end + 1, hashCode);
					return end + 1;
				}
				else{
					m_hashs.insert(begin + 1, hashCode);
					return begin + 1;
				}
			}
			return -1;
		}
	public:
		/*
		* 清除
		*/
		void clear(){
			m_hashs.clear();
			m_keys.clear();
			m_rows.clear();
		}
		/*
		* 根据哈希代码查找键的索引
		*/
		int findKeyIndex(int hashCode){
			int low = 0;
			int high = m_hashs.size() - 1;
			while (low <= high) {
				int middle = (low + high) / 2;
				double hf = m_hashs.get(middle);
				if (hashCode == hf) {
					return middle;
				}
				else if (hashCode > hf) {
					low = middle + 1;
				}
				else if (hashCode < hf) {
					high = middle - 1;
				}
			}
			return -1;
		}
		/*
		* 根据哈希代码查找键的多个索引
		*/
		void findKeyIndexs(int rowIndex, int hashCode, int *list, int *len, int maxSize){
			*len = 0;
			if(rowIndex != -1){
				int hSize = m_hashs.size();
				int tempIndex = rowIndex;
				while(tempIndex >= 0){
					if(m_hashs.get(tempIndex) == hashCode){
						if(*len < maxSize){
							list[*len] = tempIndex;
						}
						*len = *len + 1;
					}
					else{
						break;
					}
					tempIndex--;
				}
				tempIndex = rowIndex + 1;
				while(tempIndex < hSize){
					if(m_hashs.get(tempIndex) == hashCode){
						if(*len < maxSize){
							list[*len] = tempIndex;
						}
						*len = *len + 1;
					}
					else{
						break;
					}
					tempIndex++;
				}
			}
		}
		/*
		* 是否包含键
		*/
		bool containsKey(KEY key){
			int hashCode = hashKey(key);
			int index = findKeyIndex(hashCode);
			if(index != -1){
				int list[1024];
				int len = 0;
				findKeyIndexs(index, hashCode, list, &len, 1024);
				if(len >= 1){
					if(len > 1024){
						int *bigList = new int[len];
						findKeyIndexs(index, hashCode, bigList, &len, len);
						for(int i = 0; i < len; i++){
							if(m_keys.get(bigList[i]) == key){
								index = bigList[i];
								return true;
							}
						}
						delete[] bigList;
						bigList = 0;
					}else{
						for(int i = 0; i < len; i++){
							if(m_keys.get(list[i]) == key){
								index = list[i];
								return true;
							}
						}
					}
				}
			}
			return false;
		}
		/*
		* 根据键获取值
		*/
		VALUE get(KEY key){
			int hashCode = hashKey(key);
			int index = findKeyIndex(hashCode);
			if(index != -1){
				int list[1024];
				int len = 0;
				findKeyIndexs(index, hashCode, list, &len, 1024);
				if(len >= 1){
					if(len > 1024){
						int *bigList = new int[len];
						findKeyIndexs(index, hashCode, bigList, &len, len);
						for(int i = 0; i < len; i++){
							if(m_keys.get(bigList[i]) == key){
								index = bigList[i];
								return m_rows.get(index);
							}
						}
						delete[] bigList;
						bigList = 0;
					}else{
						for(int i = 0; i < len; i++){
							if(m_keys.get(list[i]) == key){
								index = list[i];
								return m_rows.get(index);
							}
						}
					}
				}			
			}
			return 0;
		}
		/*
		* 根据索引获取键
		*/
		KEY getKey(int index){
			return m_keys.get(index);
		}
		/*
		* 根据索引获取值
		*/
		VALUE getValue(int index){
			return m_rows.get(index);
		}
		/*
		* 保存键值对
		*/
		void put(KEY key, VALUE value){
			int hashCode = hashKey(key);
			int index = findKeyIndex(hashCode);
			bool newIndex = false;
			if(index == -1){
				index = addHashCode(hashCode);
				newIndex = true;
			}
			else{
				int list[1024];
				int len = 0;
				findKeyIndexs(index, hashCode, list, &len, 1024);
				if(len >= 1){
					index = -1;
					if(len > 1024){
						int *bigList = new int[len];
						findKeyIndexs(index, hashCode, bigList, &len, len);
						for(int i = 0; i < len; i++){
							if(m_keys.get(bigList[i]) == key){
								index = bigList[i];
								break;
							}
						}
						delete[] bigList;
						bigList = 0;
					}else{
						for(int i = 0; i < len; i++){
							if(m_keys.get(list[i]) == key){
								index = list[i];
								break;
							}
						}
					}
					if(index == -1){
						index = addHashCode(hashCode);
						newIndex = true;
					}
				}
			}
			if(newIndex){
				m_keys.insert(index, key);
				m_rows.insert(index, value);
			}else{
				m_keys.set(index, key);
				m_rows.set(index, value);
			}
		}
		/*
		* 移除
		*/
		void remove(KEY key){
			int hashCode = hashKey(key);
			int index = findKeyIndex(hashCode);
			if(index != -1){
				int list[1024];
				int len = 0;
				findKeyIndexs(index, hashCode, list, &len, 1024);
				if(len >= 1){
					index = -1;
					if(len > 1024){
						int *bigList = new int[len];
						findKeyIndexs(index, hashCode, bigList, &len, len);
						for(int i = 0; i < len; i++){
							if(m_keys.get(bigList[i]) == key){
								index = bigList[i];
								break;
							}
						}
						delete[] bigList;
						bigList = 0;
					}else{
						for(int i = 0; i < len; i++){
							if(m_keys.get(list[i]) == key){
								index = list[i];
								break;
							}
						}
					}
				}
				if(index != -1){
					m_hashs.removeAt(index);
					m_keys.removeAt(index);
					m_rows.removeAt(index);
				}
			}
		}
		int size(){
			return m_hashs.size();
		}	
	public:
		/*
		* 构造函数
		*/
		HashMap(){}
		/*
		* 析构函数
		*/
		virtual ~HashMap(){clear();}
	public:
		/*
		* 拷贝构造函数
		*/
        HashMap(const HashMap& rhs){
            if (this != &rhs) {
                m_hashs = rhs.m_hashs;
                m_keys = rhs.m_keys;
                m_rows = rhs.m_rows;
            }
        }
		/*
		* 重写等于方法
		*/
        const HashMap& operator=(const HashMap& rhs){
            if (this != &rhs) {
                clear();
                m_hashs = rhs.m_hashs;
                m_keys = rhs.m_keys;
                m_rows = rhs.m_rows;
            }
            return *this;
        }
	};
}
#endif