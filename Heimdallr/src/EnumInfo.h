#pragma once


#include <functional>
#include <cassert>
#include "Attribute.h"

namespace hmdl
{
	struct EnumInfo : public TypeInfo, public __internal__::IAttributeHolder
	{
	public:
		/** Main constructor */
		EnumInfo(
			uint64_t id,
			const char* name,
			const char* fullname,
			size_t size,
			const char** values,
			size_t count,
			const TypeInfo* owner,
			std::function<const char* (void*)>* valueAddrToStringFunc,
			std::function<bool(const char*)>* stringEnumExistsFunc,
			std::function<void()>* enumToStringFunc,
			std::function<void()>* stringToEnumFunc,
			const std::vector<const IAttribute*>& attrs) :
			TypeInfo({ id, name, fullname, size }),
			m_Values(values),
			m_Count(count),
			m_OwnerType(owner),
			m_ValueAddrToStringFunc(valueAddrToStringFunc),
			m_StringEnumExistsFunc(stringEnumExistsFunc),
			m_EnumToStringFunc(enumToStringFunc),
			m_StringToEnumFunc(stringToEnumFunc),
			IAttributeHolder({ attrs })
		{
		};

	public:
		/** Get values of the enum as strings */
		inline const char** GetValues() const { return m_Values; };

		/** Get value of the enum as string given a index */
		inline const char* GetValue(size_t index) const { assert(index >= 0 && index < m_Count); return m_Values[index]; };

		/** Get the size in bytes of the enum */
		inline size_t GetCount() const { return m_Count; };

		/** Get the type info of the type owner type in which it was declared */
		inline const TypeInfo* GetOwnerType() const { return m_OwnerType; };

		/** Check whether or not an enum value exists with a given string */
		inline bool HasValue(const char* str) const
		{
			assert(m_StringEnumExistsFunc != nullptr);
			return (*m_StringEnumExistsFunc)(str);
		}

		template<typename Enum>
		inline Enum GetValueByString(const char* str) const
		{
			static_assert(std::is_enum<Enum>::value, "GetValueByString() should used with an enum or enum class!");
			assert(HasValue(str) && m_StringToEnumFunc != nullptr);

			std::function<Enum(const char*)>* func = reinterpret_cast<std::function<Enum(const char*)>*>(m_StringToEnumFunc);
			return (*func)(str);
		}

		template<typename Enum>
		inline bool TryGetValueByString(const char* str, Enum& outResult) const
		{
			static_assert(std::is_enum<Enum>::value, "GetValueByString() should used with an enum or enum class!");
			if (!HasValue(str)) return false;
			if (m_StringToEnumFunc == nullptr) return false;

			std::function<Enum(const char*)>* func = reinterpret_cast<std::function<Enum(const char*)>*>(m_StringToEnumFunc);
			outResult = (*func)(str);
			return true;
		}

		template<typename Enum>
		inline const char* GetStringByValue(Enum value) const
		{
			static_assert(std::is_enum<Enum>::value, "GetStringByValue() should used with an enum or enum class!");
			assert(m_EnumToStringFunc != nullptr && "EnumInfo enum to string function pointer is null");

			std::function<const char*(Enum)>* func = reinterpret_cast<std::function<const char*(Enum)>*>(m_EnumToStringFunc);
			return (*func)(value);
		}

		template<typename Enum>
		inline bool TryGetStringByValue(Enum value, const char*& outResult) const
		{
			static_assert(std::is_enum<Enum>::value, "GetStringByValue() should used with an enum or enum class!");
			if (m_EnumToStringFunc == nullptr) return false;

			std::function<const char* (Enum)>* func = reinterpret_cast<std::function<const char* (Enum)>*>(m_EnumToStringFunc);
			outResult = (*func)(value);
			return true;
		}

		/** Gets a string of the enum value given the value address */
		inline const char* GetStringByValueAddress(void* valueAddr) const
		{
			assert(m_ValueAddrToStringFunc != nullptr && "EnumInfo value address to string function pointer is null");
			return (*m_ValueAddrToStringFunc)(valueAddr);
		};

	private:
		/** Values of the enum as strings */
		const char** m_Values;

		/** Number of members in the enum */
		size_t m_Count;

		/** Type in which the enum was declared */
		const TypeInfo* m_OwnerType;

		/** Function pointer used to return the corresponding string given the value address */
		std::function<const char* (void*)>* m_ValueAddrToStringFunc;

		/** Function pointer used to check whether an enum value exists with a given string */
		std::function<bool(const char*)>* m_StringEnumExistsFunc;

		/** Function pointer used to convert a enum value to a string */
		std::function<void()>* m_EnumToStringFunc;

		/** Function pointer used to convert a string to a string value */
		std::function<void()>* m_StringToEnumFunc;
	};

}
