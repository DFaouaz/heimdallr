#pragma once


#include <cstdint>

namespace hmdl
{
	struct TypeInfo
	{
	public:
		TypeInfo(uint64_t id, const char* name, const char* fullname, size_t size) :
			m_ID(id),
			m_Name(name),
			m_FullName(fullname),
			m_Size(size)
		{};
		virtual ~TypeInfo() = default;

		inline uint64_t GetID() const { return m_ID; };
		inline const char* GetName() const { return m_Name; };
		inline const char* GetFullName() const { return m_FullName; };
		inline size_t GetSize() const { return m_Size; };

	public:
		inline static uint64_t InvalidID = 0;

	private:
		uint64_t m_ID;
		const char* m_Name;
		const char* m_FullName;
		size_t m_Size;

	};

}