#include "../stdafx.h"
#include "StringHelper.h"

void StringHelper::SplitString(std::vector<std::string>* result, std::string origin, std::string tok)
{
	result->clear();

	int cutAt = 0; //�ڸ� ��ġs
	while ((cutAt = (int)origin.find_first_of(tok)) != origin.npos)
	{
		if (cutAt > 0) //�ڸ��� ��ġ�� 0����ũ��
			result->push_back(origin.substr(0, cutAt));

		origin = origin.substr(cutAt + 1);
	}

	if (origin.length() > 0) //�ڸ��� ���� ���� �ִٸ�
		result->push_back(origin.substr(0, cutAt));
}

void StringHelper::SplitString(std::vector<std::wstring>* result, std::wstring origin, std::wstring tok)
{
	result->clear();

	int cutAt = 0; //�ڸ� ��ġs
	while ((cutAt = (int)origin.find_first_of(tok)) != origin.npos)
	{
		if (cutAt > 0) //�ڸ��� ��ġ�� 0����ũ��
			result->push_back(origin.substr(0, cutAt));

		origin = origin.substr(cutAt + 1);
	}

	if (origin.length() > 0) //�ڸ��� ���� ���� �ִٸ�
		result->push_back(origin.substr(0, cutAt));
}


//////////////////////////////////////////////////////////////////////////
///@brief ���� ���ڿ��� ������ üũ
bool StringHelper::StartsWith(std::string str, std::string comp)
{
	std::wstring::size_type index = str.find(comp);
	if (index != std::wstring::npos && (int)index == 0)
		return true;

	return false;
}

bool StringHelper::StartsWith(std::wstring str, std::wstring comp)
{
	std::wstring::size_type index = str.find(comp);
	if (index != std::wstring::npos && (int)index == 0)
		return true;

	return false;
}

//////////////////////////////////////////////////////////////////////////
///@brief �ش� ���ڿ��� ���ԵǾ� �ִ���
bool StringHelper::Contain(std::string str, std::string comp)
{
	size_t found = str.find(comp);

	return found != std::wstring::npos;
}

bool StringHelper::Contain(std::wstring str, std::wstring comp)
{
	size_t found = str.find(comp);

	return found != std::wstring::npos;
}


//////////////////////////////////////////////////////////////////////////
///@brief �ش� ���ڿ����� comp ���ڸ� rep�� ����
void StringHelper::Replace(std::string * str, std::string comp, std::string rep)
{
	std::string temp = *str;

	size_t start_pos = 0;
	while ((start_pos = temp.find(comp, start_pos)) != std::wstring::npos)
	{
		temp.replace(start_pos, comp.length(), rep);
		start_pos += rep.length();
	}

	*str = temp;
}

void StringHelper::Replace(std::wstring* str, std::wstring comp, std::wstring rep)
{
	std::wstring temp = *str;

	size_t start_pos = 0;
	while ((start_pos = temp.find(comp, start_pos)) != std::wstring::npos)
	{
		temp.replace(start_pos, comp.length(), rep);
		start_pos += rep.length();
	}
	*str = temp;
}


///@brief string���� wstring������ ����
std::wstring StringHelper::StringToWString(std::string value)
{
	std::wstring temp = L"";
	temp.assign(value.begin(), value.end());

	return temp;
}

///@brief wstring���� string������ ����
std::string StringHelper::WStringToString(std::wstring value)
{
	std::string temp = "";
	temp.assign(value.begin(), value.end());

	return temp;
}
