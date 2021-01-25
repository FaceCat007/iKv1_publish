from ctypes import *
import struct
import time

#创建dll
kv = cdll.LoadLibrary(r"C:\\Miao\\db1\\iKv.dll")
cdll.argtypes = [c_char_p, c_int, c_double, c_long, c_wchar_p]
#初始化iKv内存数据库 指定文件夹,数据库名,分区数量
id = kv.initKv(c_char_p(b"C:\\Py\\db1"), c_char_p(b"database"), 10)
#通过键获取值
def getValue(id, key):
    recvData = create_string_buffer(102400)
    kv.getKv(id, key, recvData)
    result = str(recvData.value, encoding="gbk")
    return result
#存入键值对
kv.putKv(id, c_char_p(b"SuZhou"), c_char_p(b"19000"))
kv.putKv(id, c_char_p(b"ShangHai"), c_char_p(b"30000"))
kv.putKv(id, c_char_p(b"BeiJing"), c_char_p(b"29000"))
kv.putKv(id, c_char_p(b"ShenZheng"), c_char_p(b"20000"))
#通过键获取值
strValue = getValue(id, c_char_p(b"SuZhou"))
print(strValue)
strValue = getValue(id, c_char_p(b"ShangHai"))
print(strValue)
strValue = getValue(id, c_char_p(b"BeiJing"))
print(strValue)
strValue = getValue(id,c_char_p(b"ShenZheng"))
print(strValue)
#移除键值对
kv.removeKv(id, c_char_p(b"SuZhou"))
strValue = getValue(id, c_char_p(b"SuZhou"))
print(strValue)
#循环遍历
for i in range(0,9):
	kvCount = kv.getKvCount(id, i)
	for j in range(0,kvCount):
		recvKeyData = create_string_buffer(102400)
		recvValueData = create_string_buffer(102400)
		kv.getKv2(id, j, i, recvKeyData, recvValueData)
		strKey = str(recvKeyData.value, encoding="gbk")
		strValue = str(recvValueData.value, encoding="gbk")
		print(strKey + '=' + strValue)
#关闭库
kv.closeKv(id)
#关闭所有库
kv.closeAllKvs()
