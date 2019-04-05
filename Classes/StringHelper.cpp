#include "../stdafx.h"
#include "StringHelper.h"

void StringHelper::SplitString(std::vector<std::string>* result, std::string origin, std::string tok)
{
	result->clear();

	int cutAt = 0; //자를 위치s
	while ((cutAt = (int)origin.find_first_of(tok)) != origin.npos)
	{
		if (cutAt > 0) //자르는 위치가 0보다크면
			result->push_back(origin.substr(0, cutAt));

		origin = origin.substr(cutAt + 1);
	}

	if (origin.length() > 0) //자르고도 남은 것이 있다면
		result->push_back(origin.substr(0, cutAt));
}

void StringHelper::SplitString(std::vector<std::wstring>* result, std::wstring origin, std::wstring tok)
{
	result->clear();

	int cutAt = 0; //자를 위치s
	while ((cutAt = (int)origin.find_first_of(tok)) != origin.npos)
	{
		if (cutAt > 0) //자르는 위치가 0보다크면
			result->push_back(origin.substr(0, cutAt));

		origin = origin.substr(cutAt + 1);
	}

	if (origin.length() > 0) //자르고도 남은 것이 있다면
		result->push_back(origin.substr(0, cutAt));
}


//////////////////////////////////////////////////////////////////////////
///@brief 시작 문자열이 같은지 체크
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
///@brief 해당 문자열이 포함되어 있는지
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
///@brief 해당 문자열에서 comp 문자를 rep로 변경
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


///@brief string형을 wstring형으로 변경
std::wstring StringHelper::StringToWString(std::string value)
{
	std::wstring temp = L"";
	temp.assign(value.begin(), value.end());

	return temp;
}

///@brief wstring형을 string형으로 변경
std::string StringHelper::WStringToString(std::wstring value)
{
	std::string temp = "";
	temp.assign(value.begin(), value.end());

	return temp;
}
