/*��������èFaceCat��� v1.0
�Ϻ����è��Ϣ�������޹�˾
 */

#ifndef __FCFILE_H__
#define __FCFILE_H__
#pragma once
#include "stdafx.h"
#include "io.h"
#include <direct.h>
#include <fstream>
#include <sys/stat.h>

namespace FaceCat{
	class FCFile{
	public:
		//׷������
		static bool append(const char *file, const char *content);
		//׷������-���ַ�
		static bool append(const String &file, const String &content);
		//����Ŀ¼
		static void createDirectory(const char *dir);
		//����Ŀ¼-���ַ�
		static void createDirectory(const String &dir);
		//�ж�Ŀ¼�Ƿ����
		static bool isDirectoryExist(const char *dir);
		//�ж�Ŀ¼�Ƿ����-���ַ�
		static bool isDirectoryExist(const String &dir);
		//�ļ��Ƿ����
		static bool isFileExist(const char *file);
		//�ļ��Ƿ����-���ַ�
		static bool isFileExist(const String &file);
		//��ȡĿ¼
		static bool getDirectories(const char *dir, vector<string> *dirs);
		//��ȡĿ¼-���ַ�
		static bool getDirectories(const String &file, vector<String> *dirs);
		//��ȡ�ļ�����
		static int getFileLength(const char *file);
		//��ȡ�ļ�����-���ַ�
		static int getFileLength(const String &file);
		//��ȡ�ļ�
		static bool getFiles(const char *dir, vector<string> *files);
		//��ȡ�ļ�-���ַ�
		static bool getFiles(const String &dir, vector<String> *files);
		//��ȡ�ļ�״̬
		static int getFileState(const char *file, struct stat *buf);
		//��ȡ�ļ�״̬-���ַ�
		static int getFileState(const String &file, struct stat *buf);
		//��ȡ�ļ�
		static bool read(const char *file, string *content);
		//��ȡ�ļ�-���ַ�
		static bool read(const String &file, String *content);
		//�Ƴ��ļ�
		static void removeFile(const char *file);
		//�Ƴ��ļ�-���ַ� 
		static void removeFile(const String &file);
		//д���ļ�
		static bool write(const char *file, const char *content);
		//д���ļ�-���ַ�
		static bool write(const String &file, const String &content);
	};
}
#endif