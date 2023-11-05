#pragma once


#include <cstdint>
#include <memory>
#include "MetaObject.h"

namespace hmdl
{
	struct TypeInfo
	{
	public:
		TypeInfo(uint64_t id, const char* name, const char* fullname, size_t size, size_t alignment, const void* defaultValue) :
			m_ID(id),
			m_Name(name),
			m_FullName(fullname),
			m_Size(size),
			m_Alignment(alignment),
			m_DefaultValue(defaultValue)
		{};
		virtual ~TypeInfo() = default;

		inline uint64_t GetID() const { return m_ID; };
		inline const char* GetName() const { return m_Name; };
		inline const char* GetFullName() const { return m_FullName; };
		inline size_t GetSize() const { return m_Size; };
		inline size_t GetAlignment() const { return m_Alignment; };

		MetaObject ConstructZeroed() const
		{
			return MetaObject(m_Size, m_Alignment);
		}

		MetaObject ConstructDefault() const
		{
			return MetaObject(m_Size, m_Alignment, m_DefaultValue);
		}

	public:
		inline static uint64_t InvalidID = 0;

	private:
		uint64_t m_ID;
		const char* m_Name;
		const char* m_FullName;
		size_t m_Size;
		size_t m_Alignment;

		const void* m_DefaultValue;
	};

}