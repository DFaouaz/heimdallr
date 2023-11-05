

#include <iostream>
#include <heimdallr.h>
#include <sstream>
#include <fstream>

#include "Test.h"

namespace json
{
	std::string ToJSON(const void* data, const hmdl::ClassInfo* classInfo, int indentation = 0, bool nested = false);

	void FromJSON(const std::string& json, const hmdl::ClassInfo* classInfo)
	{

	}

	std::string ToJSONArray(const void* data, const hmdl::PropertyInfo* propertyInfo, int indentation = 0, bool nested = false)
	{
		auto indent = [&indentation]() { return std::string().insert(0, indentation, '\t'); };
		bool hasFirstFieldSerialized = false;
		std::stringstream ss;
		ss << (nested ? "" : indent()) << "[";
		ss << "\n";
		++indentation;
		for (size_t i = 0; i < propertyInfo->GetArraySize(); ++i)
		{
			const hmdl::PropertyInfo& pInfo = *propertyInfo;

			// add comman and new line
			if (hasFirstFieldSerialized)
			{
				ss << ",\n";
			}

			if (pInfo.GetType() == hmdl::GetType<bool>())						ss << indent() << (pInfo.GetPtr<bool>(data)[i] ? "true" : "false");
			else if (pInfo.GetType() == hmdl::GetType<char>())					ss << indent() << "\"" << (pInfo.GetPtr<char>(data)[i] != '\0' ? std::string() + pInfo.GetPtr<char>(data)[i] : "\\u0000") << "\"";
			else if (pInfo.GetType() == hmdl::GetType<short>())					ss << indent() << pInfo.GetPtr<short>(data)[i];
			else if (pInfo.GetType() == hmdl::GetType<int>())					ss << indent() << pInfo.GetPtr<int>(data)[i];
			else if (pInfo.GetType() == hmdl::GetType<long>())					ss << indent() << pInfo.GetPtr<long>(data)[i];
			else if (pInfo.GetType() == hmdl::GetType<long long>())				ss << indent() << pInfo.GetPtr<long long>(data)[i];
			else if (pInfo.GetType() == hmdl::GetType<float>())					ss << indent() << pInfo.GetPtr<float>(data)[i];
			else if (pInfo.GetType() == hmdl::GetType<double>())				ss << indent() << pInfo.GetPtr<double>(data)[i];
			else if (pInfo.GetType() == hmdl::GetType<long double>())			ss << indent() << pInfo.GetPtr<long double>(data);
			else if (pInfo.GetType() == hmdl::GetType<unsigned char>())			ss << indent() << pInfo.GetPtr<unsigned char>(data)[i];
			else if (pInfo.GetType() == hmdl::GetType<unsigned short>())		ss << indent() << pInfo.GetPtr<unsigned short>(data)[i];
			else if (pInfo.GetType() == hmdl::GetType<unsigned int>())			ss << indent() << pInfo.GetPtr<unsigned int>(data)[i];
			else if (pInfo.GetType() == hmdl::GetType<unsigned long>())			ss << indent() << pInfo.GetPtr<unsigned long>(data)[i];
			else if (pInfo.GetType() == hmdl::GetType<unsigned long long>())	ss << indent() << pInfo.GetPtr<unsigned long long>(data)[i];
			else if (pInfo.GetType() == hmdl::GetType<std::string>())			ss << indent() << "\"" << pInfo.GetPtr<std::string>(data)[i] << "\"";
			else
			{	// this means it is a class/struct/enum
				// if it is a enum then serialize it as a string
				if (const hmdl::EnumInfo* eInfo = dynamic_cast<const hmdl::EnumInfo*>(pInfo.GetType()))
				{
					void* enumAddr = ((char*)data) + pInfo.GetOffset() + eInfo->GetSize() * i;
					ss << indent() << "\"" << eInfo->GetStringByValueAddress(enumAddr) << "\"";
				}
				else if (const hmdl::ClassInfo* cInfo = dynamic_cast<const hmdl::ClassInfo*>(pInfo.GetType()))
				{
					void* classAddr = ((char*)data) + pInfo.GetOffset() + cInfo->GetSize() * i;
					ss << indent() << ToJSON(classAddr, cInfo, indentation, true);
				}
			}
			hasFirstFieldSerialized = true;
		}
		--indentation;
		ss << "\n" << indent() << "]";

		return ss.str();
	}

	std::string ToJSON(const void* data, const hmdl::ClassInfo* classInfo, int indentation, bool nested)
	{
		auto indent = [&indentation]() { return std::string().insert(0, indentation, '\t'); };
		bool hasFirstFieldSerialized = false;
		std::stringstream ss;
		ss << (nested ? "" : indent()) << "{";
		ss << "\n";
		++indentation;
		for (size_t i = 0; i < classInfo->GetFieldsCount(); ++i)
		{
			const hmdl::PropertyInfo& pInfo = classInfo->GetFields()[i];
			if (pInfo.HasTypeSpecialization(TypeSpec::POINTER) || pInfo.HasTypeSpecialization(TypeSpec::REFERENCE))
				continue;

			// add comman and new line
			if (hasFirstFieldSerialized)
			{
				ss << ",\n";
			}

			// if is it is an array, use custom serializer for it
			if (pInfo.HasTypeSpecialization(TypeSpec::ARRAY))
			{
				ss << indent() << "\"" << pInfo.GetName() << "\": " << ToJSONArray(data, &pInfo, indentation, true);
				hasFirstFieldSerialized = true;
				continue;
			}

			if (pInfo.GetType() == hmdl::GetType<bool>())						ss << indent() << "\"" << pInfo.GetName() << "\": " << (pInfo.GetValueAs<bool>(data) ? "true" : "false");
			else if (pInfo.GetType() == hmdl::GetType<char>())					ss << indent() << "\"" << pInfo.GetName() << "\": \"" << (pInfo.GetValueAs<char>(data) != '\0' ? std::string() + pInfo.GetValueAs<char>(data) : "\\u0000") << "\"";
			else if (pInfo.GetType() == hmdl::GetType<short>())					ss << indent() << "\"" << pInfo.GetName() << "\": " << pInfo.GetValueAs<short>(data);
			else if (pInfo.GetType() == hmdl::GetType<int>())					ss << indent() << "\"" << pInfo.GetName() << "\": " << pInfo.GetValueAs<int>(data);
			else if (pInfo.GetType() == hmdl::GetType<long>())					ss << indent() << "\"" << pInfo.GetName() << "\": " << pInfo.GetValueAs<long>(data);
			else if (pInfo.GetType() == hmdl::GetType<long long>())				ss << indent() << "\"" << pInfo.GetName() << "\": " << pInfo.GetValueAs<long long>(data);
			else if (pInfo.GetType() == hmdl::GetType<float>())					ss << indent() << "\"" << pInfo.GetName() << "\": " << pInfo.GetValueAs<float>(data);
			else if (pInfo.GetType() == hmdl::GetType<double>())				ss << indent() << "\"" << pInfo.GetName() << "\": " << pInfo.GetValueAs<double>(data);
			else if (pInfo.GetType() == hmdl::GetType<long double>())			ss << indent() << "\"" << pInfo.GetName() << "\": " << pInfo.GetValueAs<long double>(data);
			else if (pInfo.GetType() == hmdl::GetType<unsigned char>())			ss << indent() << "\"" << pInfo.GetName() << "\": " << pInfo.GetValueAs<unsigned char>(data);
			else if (pInfo.GetType() == hmdl::GetType<unsigned short>())		ss << indent() << "\"" << pInfo.GetName() << "\": " << pInfo.GetValueAs<unsigned short>(data);
			else if (pInfo.GetType() == hmdl::GetType<unsigned int>())			ss << indent() << "\"" << pInfo.GetName() << "\": " << pInfo.GetValueAs<unsigned int>(data);
			else if (pInfo.GetType() == hmdl::GetType<unsigned long>())			ss << indent() << "\"" << pInfo.GetName() << "\": " << pInfo.GetValueAs<unsigned long>(data);
			else if (pInfo.GetType() == hmdl::GetType<unsigned long long>())	ss << indent() << "\"" << pInfo.GetName() << "\": " << pInfo.GetValueAs<unsigned long long>(data);
			else if (pInfo.GetType() == hmdl::GetType<std::string>())
			{
				ss << indent() << "\"" << pInfo.GetName() << "\": \"" << *pInfo.GetPtr<std::string>(data) << "\"";
			}
			else
			{	// this means it is a class/struct/enum
				// if it is a enum then serialize it as a string
				if (const hmdl::EnumInfo* eInfo = dynamic_cast<const hmdl::EnumInfo*>(pInfo.GetType()))
				{
					void* enumAddr = ((char*)data) + pInfo.GetOffset();
					ss << indent() << "\"" << pInfo.GetName() << "\": " << "\"" << eInfo->GetStringByValueAddress(enumAddr) << "\"";
				}
				else if (const hmdl::ClassInfo* cInfo = dynamic_cast<const hmdl::ClassInfo*>(pInfo.GetType()))
				{
					void* classAddr = ((char*)data) + pInfo.GetOffset();
					ss << indent() << "\"" << pInfo.GetName() << "\": " << ToJSON(classAddr, cInfo, indentation, true);
				}
			}
			hasFirstFieldSerialized = true;
		}
		--indentation;
		ss << "\n" << indent() << "}";

		return ss.str();
	}

	template<typename T>
	void FromJSON(const std::string& json, T& obj)
	{
		FromJSON(json, hmdl::GetClass<T>());
	}

	template<typename T>
	std::string ToJSON(const T& obj)
	{
		return ToJSON(&obj, hmdl::GetClass<T>());
	}
}

namespace Tests
{
	namespace Enum
	{
		void GetEnumTest()
		{
			const hmdl::EnumInfo* eEnum = hmdl::GetEnum<TestClass::MyCXXEnum>();
			if (eEnum == nullptr)
			{
				printf("ERROR: GetEnum should never return a nullptr (on auto generated code)\n");
			}
			else
			{
				printf("GetEnumTest terminated successfully\n");
			}
		}

		void EnumHasValueTest()
		{
			const hmdl::EnumInfo* eEnum = hmdl::GetEnum<TestClass::MyCXXEnum>();

			if (!eEnum->HasValue("NENE") && eEnum->HasValue("NUNU"))
			{
				printf("EnumHasValueTest terminated successfully\n");
			}
			else
			{
				printf("ERROR: HasValue is not returning the correct data (on auto generated code)\n");
			}
		}

		void EnumGetValueByStringTest()
		{
			TestClass::MyCXXEnum value;
			const hmdl::EnumInfo* eEnum = hmdl::GetEnum<TestClass::MyCXXEnum>();
			if (eEnum->TryGetValueByString<TestClass::MyCXXEnum>("NENE", value))
			{
				printf("ERROR: NENE value was found\n");
				return;
			}

			if (!eEnum->TryGetValueByString<TestClass::MyCXXEnum>("NUNU", value))
			{
				printf("ERROR: NUNU value was not found\n");
				return;
			}

			printf("EmunGetValueByStringTest terminated successfully\n");
		}

		void EnumGetStringByValueTest()
		{
			const char* str;
			const hmdl::EnumInfo* eEnum = hmdl::GetEnum<TestClass::MyCXXEnum>();
			if (eEnum->TryGetStringByValue(TestClass::MyCXXEnum::NONO, str))
			{
				if (str != "NONO")
				{
					printf("ERROR: NONO string was not correctly returned\n");
					return;
				}
			}
			else
			{
				printf("ERROR: NONO value was not found\n");
				return;
			}

			if (eEnum->TryGetStringByValue(TestClass::MyCXXEnum::NONE, str))
			{
				if (str != "NONE")
				{
					printf("ERROR: NONE string was not correctly returned\n");
					return;
				}
			}
			else
			{
				printf("ERROR: NONE value was not found\n");
				return;
			}

			printf("EmunGetStringByValueTest terminated successfully\n");
		}

		void EnumLoopOverStringValues()
		{
			const hmdl::EnumInfo* eEnum = hmdl::GetEnum<TestClass::MyCXXEnum>();
			for (int i = 0; i < eEnum->GetCount(); ++i)
			{
				const char* str = eEnum->GetValue(i);
				if (str != eEnum->GetStringByValue((TestClass::MyCXXEnum)i))
				{
					printf("ERROR: String values are not the same\n");
					return;
				}
			}

			printf("EnumLoopOverStringValues terminated successfully\n");
		}

		void Test()
		{
			GetEnumTest();
			EnumHasValueTest();
			EnumGetValueByStringTest();
			EnumGetStringByValueTest();
			EnumLoopOverStringValues();
		}
	}
}


int main()
{
	Tests::Enum::Test();

	const hmdl::ClassInfo* cInfo = hmdl::GetClass("TestClass");
	const hmdl::TypeInfo* tInfo = hmdl::GetType("string");
	const hmdl::EnumInfo* eInfo = hmdl::GetEnum("MyCXXEnum");
	auto func = cInfo->GetMethod("MyFunctionWithNameX");
	

	hmdl::MetaObject metaObject = cInfo->ConstructDefault();
	TestClass& a = metaObject.GetAs<TestClass>();
	const hmdl::PropertyInfo* pInfo = cInfo->GetField("nunuName");
	pInfo->SetValue(metaObject, std::string("HolaCaracola"));
	hmdl::MetaObject metaObject2 = metaObject;
	pInfo = cInfo->GetField("c");
	pInfo->SetValue(metaObject, 'h');

	TestClass& b = metaObject2.GetAs<TestClass>();
	printf("%s", a.nunuName.c_str());
	printf("%c", a.c);
	func->Invoke(&a, 'c', 8);

	return 0;
}
