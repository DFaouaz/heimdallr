#pragma once

#include <malloc.h>

namespace hmdl
{
	struct MetaObject
	{
	public:
		MetaObject(size_t size, size_t alignment, const void* defaultValue = nullptr) : 
			m_Data(nullptr, std::free),
			m_Size(size),
			m_Alignment(alignment)
		{
			void* data = malloc(size);
			if (data == nullptr) return;
			if (defaultValue == nullptr)
			{
				std::memset(data, 0, size);
			}
			else
			{
				std::memcpy(data, defaultValue, size);
			}
			size_t space = size;
			data = std::align(alignment, size, data, space);
			m_Data.reset(data);
		};
		MetaObject(const MetaObject& other) :
			m_Data(nullptr, std::free),
			m_Size(other.m_Size),
			m_Alignment(other.m_Alignment)
		{
			void* data = std::malloc(other.m_Size);
			if (data == nullptr) return;
			size_t space = other.m_Size;
			data = std::align(other.m_Alignment, other.m_Size, data, space);

			std::memcpy(data, other.m_Data.get(), other.m_Size);

			m_Data.reset(data);
		};
		~MetaObject() { };

		template<typename T>
		T& GetAs() const
		{
			return *reinterpret_cast<T*>(m_Data.get());
		}

		/** Whether or not is a valid object (valid memory) */
		inline bool IsValid() const { return m_Data != nullptr; }

		/** Whether or not is a valid object (valid memory) */
		inline const void* Data() const { return m_Data.get(); }

		operator void* () const { return m_Data.get(); }
		operator const void* () const { return m_Data.get(); }
		operator bool() const { return IsValid(); }

	private:
		std::unique_ptr<void, void(*)(void*)> m_Data;
		const size_t m_Size;
		const size_t m_Alignment;
	};
}
