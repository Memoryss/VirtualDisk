#pragma once

#include <string>

class ZBase64
{
public:
	ZBase64();

	//编码 DataByte 输入的数据长度,以字节为单位
	static std::string saveData(char const* bytes_to_encode,unsigned int in_len);

	//解码  OutByte 输出的数据长度,以字节为单位,请不要通过返回值计算 输出数据的长度
	static std::string parseData(std::string const& encoded_string);

	static inline bool isData(unsigned char c)
	{
		return (isalnum(c) || (c == '+') || (c == '/'));
	}
	
private:
	char* m_Base64_Table;

};