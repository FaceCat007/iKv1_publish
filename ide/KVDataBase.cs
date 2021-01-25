/*
* FaceCat图形通讯框架(非开源)
* 著作权编号:2015SR229355+2020SR0266727
* 上海卷卷猫信息技术有限公司
*/

using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace FaceCat {
    public class KVDataBase {
        /// <summary>
        /// 初始化键值对数据库
        /// </summary>
        /// <param name="path">路径</param>
        /// <param name="name">名称</param>
        /// <param name="number">文件数</param>
        /// <returns>状态</returns>
        [DllImport("iKv.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern int initKv(String path, String name, int number);

        /// <summary>
        /// 通过键获取值
        /// </summary>
        /// <param name="id">编号</param>
        /// <param name="key">键</param>
        /// <param name="value">值</param>
        /// <returns>状态</returns>
        [DllImport("iKv.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern int getKv(int id, String key, IntPtr value);

        /// <summary>
        /// 关闭数据库
        /// </summary>
        /// <param name="id">编号</param>
        /// <param name="key">键</param>
        /// <param name="value">值</param>
        /// <returns>状态</returns>
        [DllImport("iKv.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern int closeKv(int id);

        /// <summary>
        /// 关闭所有数据库
        /// </summary>
        /// <param name="id">编号</param>
        /// <param name="key">键</param>
        /// <param name="value">值</param>
        /// <returns>状态</returns>
        [DllImport("iKv.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern int closeAllKvs();


        /// <summary>
        /// 通过键获取值
        /// </summary>
        /// <param name="id">编号</param>
        /// <param name="key">键</param>
        /// <param name="value">值</param>
        /// <returns>状态</returns>
        [DllImport("iKv.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern int getKv2(int id, int index, int number, IntPtr key, IntPtr value);

        /// <summary>
        /// 通过键获取值
        /// </summary>
        /// <param name="id">编号</param>
        /// <param name="key">键</param>
        /// <returns>值</returns>
        public static String getKv(int id, String key) {
            IntPtr bufferIntPtr = Marshal.AllocHGlobal(10240);
            getKv(id, key, bufferIntPtr);
            String sbResult = Marshal.PtrToStringAnsi(bufferIntPtr);
            Marshal.FreeHGlobal(bufferIntPtr);
            return sbResult;
        }

        /// <summary>
        /// 通过索引获取值
        /// </summary>
        /// <param name="id">编号</param>
        /// <param name="index">索引</param>
        /// <param name="number">号码</param>
        /// <param name="key">键</param>
        /// <param name="value">值</param>
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
        /// 移除键
        /// </summary>
        /// <param name="id">编号</param>
        /// <param name="key">键</param>
        /// <returns>状态</returns>
        [DllImport("iKv.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern int removeKv(int id, String key);

        /// <summary>
        /// 保存键值对
        /// </summary>
        /// <param name="id">编号</param>
        /// <param name="key">键</param>
        /// <param name="value">值</param>
        /// <returns>状态</returns>
        [DllImport("iKv.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern int putKv(int id, String key, String value);

        /// <summary>
        /// 获取数据量
        /// </summary>
        /// <param name="id">编号</param>
        /// <param name="number">编号2</param>
        /// <returns></returns>
        [DllImport("iKv.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern int getKvCount(int id, int number);
    }
}
