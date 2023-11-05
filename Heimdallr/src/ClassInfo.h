#pragma once


#include "TypeInfo.h"
#include "PropertyInfo.h"
#include "FunctionInfo.h"


namespace hmdl
{
	struct ClassInfo : public TypeInfo, public __internal__::IAttributeHolder
	{
	public:
		ClassInfo(
			uint64_t id,
			const char* name,
			const char* fullname,
			size_t size,
			size_t alignment,
			const void* defaultValue,
			const ClassInfo** parentClassesArr,
			size_t parentClassesSize,
			const PropertyInfo* fieldsArr,
			size_t fieldsSize,
			const FunctionInfo* functionsArr,
			size_t functionSize,
			const std::vector<const IAttribute*>& attrs) :
			TypeInfo({ id, name, fullname, size, alignment, defaultValue }),
			m_ParentClasses(parentClassesArr),
			m_ParentClassesSize(parentClassesSize),
			m_Fields(fieldsArr),
			m_FieldsSize(fieldsSize),
			m_Functions(functionsArr),
			m_FunctionsSize(functionSize),
			IAttributeHolder({ attrs })
		{};

		inline const ClassInfo** GetParentClasses() const { return m_ParentClasses; };
		inline size_t GetParentClassesCount() const { return m_ParentClassesSize; };
		inline const PropertyInfo* GetFields() const { return m_Fields; };
		inline size_t GetFieldsCount() const { return m_FieldsSize; };
		inline const FunctionInfo* GetFuntions() const { return m_Functions; };
		inline size_t GetFuntionsCount() const { return m_FunctionsSize; };

		const PropertyInfo* GetField(const char* name) const
		{
			if (m_Fields == nullptr) return nullptr;

			for (size_t i = 0; i < m_FieldsSize; ++i)
			{
				if (m_Fields[i].GetName() == name)
					return &m_Fields[i];
			}

			// check in parent classes
			for (size_t i = 0; i < m_ParentClassesSize; ++i)
			{
				if (const PropertyInfo* pInfo = m_ParentClasses[i]->GetField(name))
				{
					return pInfo;
				}
			}

			return nullptr;
		}

		const FunctionInfo* GetMethod(const char* name) const
		{
			if (m_Functions == nullptr) return nullptr;

			for (int i = 0; i < m_FunctionsSize; ++i)
			{
				if (m_Functions[i].GetName() == name)
					return &m_Functions[i];
			}

			// check in parent classes
			for (size_t i = 0; i < m_ParentClassesSize; ++i)
			{
				if (const FunctionInfo* fInfo = m_ParentClasses[i]->GetMethod(name))
				{
					return fInfo;
				}
			}

			return nullptr;
		}

		static std::string ToString(const ClassInfo& classInfo)
		{
			std::string str;
			str += std::string() + "class " + classInfo.GetName();
			for (int i = 0; i < classInfo.GetParentClassesCount(); ++i)
			{
				if (i == 0) str += " : ";
				else str += ", ";
				str += classInfo.GetParentClasses()[i]->GetName();
			}
			str += "\n";
			str += "{\n";

			for (int i = 0; i < classInfo.GetFieldsCount(); ++i)
			{
				const PropertyInfo* fInfo = classInfo.GetFields() + i;
				str += "\t" + PropertyInfo::ToString(*fInfo) + "\n";
			}
			str += "};";

			return str;
		}

	private:
		const ClassInfo** m_ParentClasses;
		size_t m_ParentClassesSize;
		const PropertyInfo* m_Fields;
		size_t m_FieldsSize;
		const FunctionInfo* m_Functions;
		size_t m_FunctionsSize;

	};

}
