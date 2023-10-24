#pragma once

#include <functional>
#include <string>
#include <set>
#include "Attribute.h"
#include "Flags.h"

namespace hmdl
{
	struct PropertyInfo : __internal__::IAttributeHolder
	{
	public:
		PropertyInfo() = default;

	public:
		inline const TypeInfo* GetType() const { return m_Type; };
		inline void SetType(const TypeInfo* type) { m_Type = type; };
		inline const TypeInfo* GetOwnerType() const { return m_OwnerType; };
		inline void SetOwnerType(const TypeInfo* type) { m_OwnerType = type; };
		inline const char* GetName() const { return m_Name; };
		inline void SetName(const char* name) { m_Name = name; };
		inline size_t GetOffset() const { return m_Offset; };
		inline void SetOffset(size_t offset) { m_Offset = offset; };
		inline AccessSpecifier GetAccessSpecifier() const { return m_AccessSpecifier; };
		inline void SetAccessSpecifier(AccessSpecifier accessSpecifier) { m_AccessSpecifier = accessSpecifier; };
		inline int16_t GetQualifiers() const { return m_Qualifiers; };
		inline void SetQualifiers(int16_t qualifiers) { m_Qualifiers = qualifiers; };
		inline bool HasQualifier(Qualifier qualifier) const { return m_Qualifiers & qualifier; };
		inline void AddQualifier(Qualifier qualifier) { m_Qualifiers |= qualifier; };
		inline void AddQualifiers(int16_t qualifiers) { m_Qualifiers |= qualifiers; };
		inline void RemoveQualifier(int16_t qualifier) { m_Qualifiers &= ~qualifier; };
		inline void RemoveQualifiers(int16_t qualifiers) { m_Qualifiers &= ~qualifiers; };
		inline int16_t GetTypeSpecialization() const { return m_TypeSpecialization; };
		inline void SetTypeSpecialization(int16_t specs) { m_TypeSpecialization = specs; };
		inline bool HasTypeSpecialization(TypeSpec spec) const { return m_TypeSpecialization & spec; };
		inline void AddTypeSpecialization(TypeSpec spec) { m_TypeSpecialization |= spec; };
		inline void AddTypeSpecializations(int16_t specs) { m_TypeSpecialization |= specs; };
		inline void RemoveTypeSpecialization(TypeSpec spec) { m_TypeSpecialization &= ~spec; };
		inline void RemoveTypeSpecializations(int16_t specs) { m_TypeSpecialization &= ~specs; };
		inline size_t GetPointerIndirectionLevel() const { return m_PointerIndirectionLevel; };
		inline void SetPointerIndirectionLevel(size_t indirectionLevel) { m_PointerIndirectionLevel = indirectionLevel; };
		inline size_t GetArraySize() const { return m_ArraySize; };
		inline void SetArraySize(size_t size) { m_ArraySize = size; };

	public:
		template<class T>
		void SetValue(void* instance, const T& value) const noexcept
		{
			// todo: check type
			memcpy(((char*)instance + m_Offset), &value, sizeof(T));
		}

		template<class T>
		T GetValueAs(const void* instance) const noexcept
		{
			// todo: check type
			T ret;
			memcpy(&ret, ((char*)instance + m_Offset), sizeof(T));
			return ret;
		}

		template<class T>
		void GetValueAs(const void* instance, T& outValue) const noexcept
		{
			// todo: check type
			memcpy(&outValue, ((char*)instance + m_Offset), sizeof(T));
		}

		template<class T>
		const T* GetPtr(const void* instance) const noexcept
		{
			// todo: check type
			return reinterpret_cast<const T*>((char*)instance + m_Offset);
		}

		template<class T>
		T* GetPtr(void* instance) const noexcept
		{
			// todo: check type
			return reinterpret_cast<T*>((char*)instance + m_Offset);
		}

		const void* GetRawPtr(const void* instance) const noexcept
		{
			// todo: check type
			return reinterpret_cast<const void*>((char*)instance + m_Offset);
		}

		static std::string ToString(const PropertyInfo& propertyInfo)
		{
			std::string str;

			if ((propertyInfo.m_Qualifiers & Qualifier::CONST) != 0)
			{
				str += "const ";
			}

			str += propertyInfo.m_Type->GetName();

			if ((propertyInfo.m_TypeSpecialization & TypeSpec::ARRAY) != 0)
			{
				str += "[" + std::to_string(propertyInfo.m_ArraySize) + "]";
			}

			if ((propertyInfo.m_TypeSpecialization & TypeSpec::POINTER) != 0)
			{
				str += "*";
			}

			if ((propertyInfo.m_TypeSpecialization & TypeSpec::REFERENCE) != 0)
			{
				str += "&";
			}

			str += " ";
			str += propertyInfo.m_Name;
			str += ";";

			return str;
		}

	private:
		const TypeInfo* m_Type;
		const TypeInfo* m_OwnerType;

		const char* m_Name;
		size_t m_Offset;

		AccessSpecifier m_AccessSpecifier;
		int16_t m_Qualifiers;
		int16_t m_TypeSpecialization;
		size_t m_PointerIndirectionLevel; // todo: TypeInfo*** info has 3 as indirection level
		size_t m_ArraySize; // non array types has 0 array size

	};

}
