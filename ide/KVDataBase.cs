/*
* FaceCatͼ��ͨѶ���(�ǿ�Դ)
* ����Ȩ���:2015SR229355+2020SR0266727
* �Ϻ����è��Ϣ�������޹�˾
*/

using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace FaceCat {
    public class KVDataBase {
        /// <summary>
        /// ��ʼ����ֵ�����ݿ�
        /// </summary>
        /// <param name="path">·��</param>
        /// <param name="name">����</param>
        /// <param name="number">�ļ���</param>
        /// <returns>״̬</returns>
        [DllImport("iKv.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern int initKv(String path, String name, int number);

        /// <summary>
        /// ͨ������ȡֵ
        /// </summary>
        /// <param name="id">���</param>
        /// <param name="key">��</param>
        /// <param name="value">ֵ</param>
        /// <returns>״̬</returns>
        [DllImport("iKv.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern int getKv(int id, String key, IntPtr value);

        /// <summary>
        /// �ر����ݿ�
        /// </summary>
        /// <param name="id">���</param>
        /// <param name="key">��</param>
        /// <param name="value">ֵ</param>
        /// <returns>״̬</returns>
        [DllImport("iKv.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern int closeKv(int id);

        /// <summary>
        /// �ر��������ݿ�
        /// </summary>
        /// <param name="id">���</param>
        /// <param name="key">��</param>
        /// <param name="value">ֵ</param>
        /// <returns>״̬</returns>
        [DllImport("iKv.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern int closeAllKvs();


        /// <summary>
        /// ͨ������ȡֵ
        /// </summary>
        /// <param name="id">���</param>
        /// <param name="key">��</param>
        /// <param name="value">ֵ</param>
        /// <returns>״̬</returns>
        [DllImport("iKv.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern int getKv2(int id, int index, int number, IntPtr key, IntPtr value);

        /// <summary>
        /// ͨ������ȡֵ
        /// </summary>
        /// <param name="id">���</param>
        /// <param name="key">��</param>
        /// <returns>ֵ</returns>
        public static String getKv(int id, String key) {
            IntPtr bufferIntPtr = Marshal.AllocHGlobal(10240);
            getKv(id, key, bufferIntPtr);
            String sbResult = Marshal.PtrToStringAnsi(bufferIntPtr);
            Marshal.FreeHGlobal(bufferIntPtr);
            return sbResult;
        }

        /// <summary>
        /// ͨ��������ȡֵ
        /// </summary>
        /// <param name="id">���</param>
        /// <param name="index">����</param>
        /// <param name="number">����</param>
        /// <param name="key">��</param>
        /// <param name="value">ֵ</param>
        public static void getKv2(int id, int index, int number, ref String key, ref String value) {
            IntPtr bufferIntPtr = Marshal.AllocHGlobal(1024);
            IntPtr bufferIntPtr2 = Marshal.AllocHGlobal(102400);
            getKv2(id, index, number, bufferIntPtr, bufferIntPtr2);
            key = Marshal.PtrToStringAnsi(bufferIntPtr);
            value = Marshal.PtrToStringAnsi(bufferIntPtr2);
            Marshal.FreeHGlobal(bufferIntPtr);
            Marshal.FreeHGlobal(bufferIntPtr2);
        }

        /// <summary>
        /// �Ƴ���
        /// </summary>
        /// <param name="id">���</param>
        /// <param name="key">��</param>
        /// <returns>״̬</returns>
        [DllImport("iKv.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern int removeKv(int id, String key);

        /// <summary>
        /// �����ֵ��
        /// </summary>
        /// <param name="id">���</param>
        /// <param name="key">��</param>
        /// <param name="value">ֵ</param>
        /// <returns>״̬</returns>
        [DllImport("iKv.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern int putKv(int id, String key, String value);

        /// <summary>
        /// ��ȡ������
        /// </summary>
        /// <param name="id">���</param>
        /// <param name="number">���2</param>
        /// <returns></returns>
        [DllImport("iKv.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern int getKvCount(int id, int number);
    }
}
