#pragma once

#include "ClassInfo.h"
#include "TypeInfo.h"
#include "PropertyInfo.h"
#include "FunctionInfo.h"
#include "EnumInfo.h"
#include "MetaObject.h"
#include "Attribute.h"

#include <string>
#include <array>
#include <map>


#ifdef __GENERATE_HEIMDALLR_REFLECTION_ANNOTATION__
#define _HEIMDALLR_REFLECTION_ANNOTATION_ "heimdallr-reflect, "
#define _HEIMDALLR_CLASS_(Line) Heimdallrs_#Line
#define _REFLECT_(...) \
__attribute__((annotate(_HEIMDALLR_REFLECTION_ANNOTATION_ #__VA_ARGS__)))

#define _INTERNAL_HEIMDALLR_VARIABLE2_(Line) Heimdallr_##Line
#define _INTERNAL_HEIMDALLR_VARIABLE_(Line) _INTERNAL_HEIMDALLR_VARIABLE2_(Line)
#define HEIMDALLR_CLASS(...)		class _REFLECT_(heimdallr-class, heimdallr-attrs: __VA_ARGS__) _INTERNAL_HEIMDALLR_VARIABLE_(__LINE__);
#define HEIMDALLR_STRUCT(...)		struct _REFLECT_(heimdallr-struct, heimdallr-attrs: __VA_ARGS__) _INTERNAL_HEIMDALLR_VARIABLE_(__LINE__);
#define HEIMDALLR_PROPERTY(...)		_REFLECT_(heimdallr-property, heimdallr-attrs: __VA_ARGS__)
#define HEIMDALLR_FUNCTION(...)		_REFLECT_(heimdallr-function, heimdallr-attrs: __VA_ARGS__)
#define HEIMDALLR_ENUM(...)			enum class _REFLECT_(heimdallr-enum, heimdallr-attrs: __VA_ARGS__) _INTERNAL_HEIMDALLR_VARIABLE_(__LINE__);

#else
#define _REFLECT_(...)
#define HEIMDALLR_CLASS(...)		_REFLECT_(__VA_ARGS__)
#define HEIMDALLR_STRUCT(...)		_REFLECT_(__VA_ARGS__)
#define HEIMDALLR_PROPERTY(...)		_REFLECT_(__VA_ARGS__)
#define HEIMDALLR_FUNCTION(...)		template<typename T, typename Func, size_t Index> friend struct hmdl::__internal__::FunctionInvoker; \
									_REFLECT_(__VA_ARGS__)
#define HEIMDALLR_ENUM(...)			_REFLECT_(__VA_ARGS__)
#endif

namespace hmdl
{
	namespace __internal__
	{
		template<class Type, size_t NParentClasses, size_t NFields, size_t NFunctions, size_t NTemplateArgs = 0 >
		struct ClassStorage
		{
			static_assert(std::is_class<Type>::value, "ClassStorage only admits classes or structs");

			template<typename Lambda>
			ClassStorage(Lambda&& ctr) noexcept :
				m_ParentClasses(),
				m_Fields(),
				m_Functions(),
				m_TemplateArgs(),
				m_Attributes(),
				m_Comment(nullptr)
			{
				ctr(this);
			};

			const size_t ParentClassesCount = NParentClasses;
			const size_t FieldsCount = NFields;
			const size_t FunctionsCount = NFunctions;
			const size_t TemplateArgsCount = NTemplateArgs;

			std::array<const ClassInfo*, NParentClasses> m_ParentClasses;
			std::array<PropertyInfo, NFields> m_Fields;
			std::array<FunctionInfo, NFunctions> m_Functions;
			std::array<PropertyInfo, NTemplateArgs> m_TemplateArgs; // todo: do something with this (maybe)
			std::vector<const IAttribute*> m_Attributes;
			const char* m_Comment;
		};

		template<typename Enum, size_t N>
		struct EnumStorage
		{
			static_assert(std::is_enum<Enum>::value, "EnumStorage only admits enum o enum classes");

			template<typename Lambda>
			EnumStorage(Lambda&& ctr) noexcept :
				m_Values(),
				m_Strings(),
				m_Comments(),
				m_ValueToString(),
				m_StringToValue(),
				m_Attributes(),
				m_Comment(nullptr)
			{
				ctr(this);
			};

			std::array<Enum, N> m_Values;
			std::array<const char*, N> m_Strings;
			std::array<const char*, N> m_Comments;
			std::unordered_map<Enum, const char*> m_ValueToString;
			std::unordered_map<const char*, Enum> m_StringToValue;

			std::function<const char* (void*)> m_ValueAddrToStringFunc;
			std::function<bool(const char*)> m_StringEnumExistsFunc;
			std::function<const char* (Enum)> m_EnumToStringFunc;
			std::function<Enum(const char*)> m_StringToEnumFunc;
			std::vector<const IAttribute*> m_Attributes;
			const char* m_Comment;

		};

		template<size_t NParams, typename ReturnType, typename ...Args>
		struct FunctionStorage
		{
			template<typename Lambda>
			FunctionStorage(Lambda&& ctr) :
				m_Name(nullptr),
				m_ReturnType(nullptr),
				m_Parameters(),
				m_InvokeFunc(),
				m_Attributes(),
				m_Comment(nullptr),
				m_ReturnComment(nullptr)
			{
				ctr(this);
			};

			const char* m_Name;
			const TypeInfo* m_ReturnType;
			std::array<PropertyInfo, NParams> m_Parameters;
			std::function<ReturnType(void*, Args...)> m_InvokeFunc;

			std::vector<const IAttribute*> m_Attributes;
			const char* m_Comment;
			const char* m_ReturnComment;
			// todo: Qualifiers
		};

		template<typename T, typename Func, size_t Index>
		struct FunctionInvoker {};

		template<typename T>
		struct TemplatedTag {};

		template<typename T>
		extern const TypeInfo* GetTypeImpl(TemplatedTag<T>) noexcept;

		template<typename T>
		extern const ClassInfo* GetClassImpl(TemplatedTag<T>) noexcept;

		template<typename Enum>
		extern const EnumInfo* GetEnumImpl(TemplatedTag<Enum>) noexcept;

		template<typename Enum>
		extern const char* EnumToStringImpl(TemplatedTag<Enum>, Enum e) noexcept;

		template<typename Enum>
		extern Enum StringToEnumImpl(TemplatedTag<Enum>, const char* str) noexcept;

		namespace registry
		{
			inline std::set<const TypeInfo*> s_AllTypeInfo;
			inline std::unordered_map<const char*, const TypeInfo*> s_NameToTypeInfo;
			inline std::unordered_map<const char*, const TypeInfo*> s_FullNameToTypeInfo;

			inline std::set<const ClassInfo*> s_AllClassInfo;
			inline std::unordered_map<const char*, const ClassInfo*> s_NameToClassInfo;
			inline std::unordered_map<const char*, const ClassInfo*> s_FullNameToClassInfo;

			inline std::set<const EnumInfo*> s_AllEnumInfo;
			inline std::unordered_map<const char*, const EnumInfo*> s_NameToEnumInfo;
			inline std::unordered_map<const char*, const EnumInfo*> s_FullNameToEnumInfo;

			inline void Register(const TypeInfo* typeInfo)
			{
				if (s_AllTypeInfo.find(typeInfo) != s_AllTypeInfo.end()) return;

				s_AllTypeInfo.insert(typeInfo);
				s_NameToTypeInfo[typeInfo->GetName()] = typeInfo;
				s_FullNameToTypeInfo[typeInfo->GetFullName()] = typeInfo;
			}

			inline void Register(const ClassInfo* classInfo)
			{
				const TypeInfo* typeInfo = classInfo;
				Register(typeInfo);

				if (s_AllClassInfo.find(classInfo) != s_AllClassInfo.end()) return;

				s_AllClassInfo.insert(classInfo);
				s_NameToClassInfo[classInfo->GetName()] = classInfo;
				s_FullNameToClassInfo[classInfo->GetFullName()] = classInfo;
			}

			inline void Register(const EnumInfo* enumInfo)
			{
				const TypeInfo* typeInfo = enumInfo;
				Register(typeInfo);

				if (s_AllEnumInfo.find(enumInfo) != s_AllEnumInfo.end()) return;

				s_AllEnumInfo.insert(enumInfo);
				s_NameToEnumInfo[enumInfo->GetName()] = enumInfo;
				s_FullNameToEnumInfo[enumInfo->GetFullName()] = enumInfo;
			}

			struct RegisterHelper
			{
				RegisterHelper(const ClassInfo* classInfo)
				{
					hmdl::__internal__::registry::Register(classInfo);
				}

				RegisterHelper(const TypeInfo* typeInfo)
				{
					hmdl::__internal__::registry::Register(typeInfo);
				}

				RegisterHelper(const EnumInfo* enumInfo)
				{
					hmdl::__internal__::registry::Register(enumInfo);
				}
			};

		}
	}

	inline const ClassInfo* GetClass(const char* name) noexcept
	{
		using namespace __internal__;
		if (registry::s_FullNameToClassInfo.find(name) != registry::s_FullNameToClassInfo.end())
		{
			return registry::s_FullNameToClassInfo[name];
		}

		if (registry::s_NameToClassInfo.find(name) != registry::s_NameToClassInfo.end())
		{
			return registry::s_NameToClassInfo[name];
		}

		return nullptr;
	}

	template<typename T>
	inline const ClassInfo* GetClass() noexcept
	{
		return __internal__::GetClassImpl(__internal__::TemplatedTag<T>{});
	}

	inline const TypeInfo* GetType(const char* name) noexcept
	{
		using namespace __internal__;
		if (registry::s_FullNameToTypeInfo.find(name) != registry::s_FullNameToTypeInfo.end())
		{
			return registry::s_FullNameToTypeInfo[name];
		}

		if (registry::s_NameToTypeInfo.find(name) != registry::s_NameToTypeInfo.end())
		{
			return registry::s_NameToTypeInfo[name];
		}

		return nullptr;
	}

	template<typename T>
	inline const TypeInfo* GetType() noexcept
	{
		return __internal__::GetTypeImpl(__internal__::TemplatedTag<T>{});
	}

	inline const EnumInfo* GetEnum(const char* name) noexcept
	{
		using namespace __internal__;
		if (registry::s_FullNameToEnumInfo.find(name) != registry::s_FullNameToEnumInfo.end())
		{
			return registry::s_FullNameToEnumInfo[name];
		}

		if (registry::s_NameToEnumInfo.find(name) != registry::s_NameToEnumInfo.end())
		{
			return registry::s_NameToEnumInfo[name];
		}

		return nullptr;
	}

	template<typename T>
	inline const EnumInfo* GetEnum() noexcept
	{
		return __internal__::GetEnumImpl(__internal__::TemplatedTag<T>{});
	}

	template<typename Enum>
	inline const char* EnumToString(Enum e) noexcept
	{
		return __internal__::EnumToStringImpl(__internal__::TemplatedTag<Enum>{}, e);
	}

	template<typename Enum>
	inline Enum StringToEnum(const char* str) noexcept
	{
		return __internal__::StringToEnumImpl(__internal__::TemplatedTag<Enum>{}, str);
	}

	template<typename Enum>
	struct EnumRange
	{
		static_assert(std::is_enum<Enum>::value, "EnumRange only admits enum o enum classes");

		Enum* begin() const;
		Enum* end() const;
	};


#ifndef _HEIMDALLR_PRIMITIVE_TYPES_
#define _HEIMDALLR_PRIMITIVE_TYPES_

	// set primitive definitions
#define TYPES                                            \
    X(bool, bool, "%d")                                  \
    X(char, char, "%d")                                  \
    X(short, short, "%d")                                \
    X(int, int, "%d")                                    \
    X(long, long, "%ld")                                 \
    X(long long, long_long, "%lld")                      \
    X(float, float, "%.9g")                              \
    X(double, double, "%.17g")                           \
    X(long double, long_double, "%.21Lg")                \
    X(unsigned char, unsigned_char, "%u")                \
    X(unsigned short, unsigned_short, "%u")              \
    X(unsigned int, unsigned_int, "%u")                  \
    X(unsigned long, unsigned_long, "%lu")               \
    X(unsigned long long, unsigned_long_long, "%llu")    \

#define X(Type, Name, Format) \
	inline Type Name##_Heimdallr_Type_DefaultValue = (Type)0;\
	inline TypeInfo Name##_Heimdallr_TypeInfo(TypeInfo::InvalidID, #Type, #Type, sizeof(Type), alignof(Type), &Name##_Heimdallr_Type_DefaultValue); \
	inline registry::RegisterHelper Name##_Heimdallr_TypeInfo_Register(&Name##_Heimdallr_TypeInfo); \
	template<> inline const TypeInfo* GetTypeImpl(TemplatedTag<Type>) noexcept \
	{ \
		return &Name##_Heimdallr_TypeInfo; \
	} \

	namespace __internal__
	{
		TYPES
#undef X

		// definitions of void type (as it is a special type)
		inline TypeInfo void_Heimdallr_TypeInfo(TypeInfo::InvalidID, "void", "void", 0, 0, nullptr);
		inline registry::RegisterHelper void_Heimdallr_TypeInfo_Register(&void_Heimdallr_TypeInfo);
		template<> inline const TypeInfo* GetTypeImpl(TemplatedTag<void>) noexcept
		{
			return &void_Heimdallr_TypeInfo;
		}

		// definitions of string type (as it is a special type)
		inline std::string std_string_Heimdallrs_Type_DefaultValue = "";
		inline TypeInfo std_string_Heimdallr_TypeInfo(TypeInfo::InvalidID, "string", "std::string", sizeof(std::string), alignof(std::string), &std_string_Heimdallrs_Type_DefaultValue);
		inline registry::RegisterHelper std_string_Heimdallr_TypeInfo_Register(&std_string_Heimdallr_TypeInfo);
		template<> inline const TypeInfo* GetTypeImpl(TemplatedTag<std::string>) noexcept
		{
			return &std_string_Heimdallr_TypeInfo;
		}
	}

#endif // _HEIMDALLR_PRIMITIVE_TYPES_
}