#pragma once

#include <Windows.h>
#include <vector>
#include <string>

class Utility
{
public:
	/*!
	 * @brief SJIS��UTF16�ɕϊ�
	 *
	 * @param path �t�@�C���̃p�X
	 *
	 * @return �ǂݍ��񂾃o�C�i��
	 */
	static std::wstring SJIStoUTF16(const std::string& s)
	{
		int len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, NULL, 0);
		std::vector<wchar_t> v(len);
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, &v[0], v.size());
		return std::wstring(v.begin(), v.begin() + len - 1);
	}
};
