#pragma once

#include "ClassInfo.h"
#include "TypeInfo.h"
#include "PropertyInfo.h"
#include "FunctionInfo.h"
#include "EnumInfo.h"
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
#define HEIMDALLR_FUNCTION(...)		template<typename Func> friend struct hmdl::__internal__::FunctionInvoker; \
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
				m_Fields(),
				m_Functions(),
				m_TemplateArgs(),
				m_Attributes()
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
		};

		template<typename Enum, size_t N>
		struct EnumStorage
		{
			static_assert(std::is_enum<Enum>::value, "EnumStorage only admits enum o enum classes");

			template<typename Lambda>
			EnumStorage(Lambda&& ctr) noexcept :
				m_Values(),
				m_Strings(),
				m_ValueToString(),
				m_StringToValue(),
				m_Attributes()
			{
				ctr(this);
			};

			std::array<Enum, N> m_Values;
			std::array<const char*, N> m_Strings;
			std::unordered_map<Enum, const char*> m_ValueToString;
			std::unordered_map<const char*, Enum> m_StringToValue;

			std::function<const char* (void*)> m_ValueAddrToStringFunc;
			std::function<bool(const char*)> m_StringEnumExistsFunc;
			std::function<const char* (Enum)> m_EnumToStringFunc;
			std::function<Enum(const char*)> m_StringToEnumFunc;
			std::vector<const IAttribute*> m_Attributes;
		};

		template<size_t NParams, typename ReturnType, typename ...Args>
		struct FunctionStorage
		{
			template<typename Lambda>
			FunctionStorage(Lambda&& ctr) :
				m_Name(nullptr),
				m_ReturnType(nullptr),
				m_Parameters(),
				m_InvokeFunc()
			{
				ctr(this);
			};

			const char* m_Name;
			const TypeInfo* m_ReturnType;
			std::array<PropertyInfo, NParams> m_Parameters;
			std::function<ReturnType(void*, Args...)> m_InvokeFunc;
			// todo: Qualifiers
		};

		template<typename Func>
		struct FunctionInvoker
		{
			constexpr static Func Invoke;
		};

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
	}

	template<typename Enum>
	struct EnumRange
	{
		static_assert(std::is_enum<Enum>::value, "EnumRange only admits enum o enum classes");

		Enum* begin() const;
		Enum* end() const;
	};

	template<typename T>
	inline const ClassInfo* GetClass() noexcept
	{
		return __internal__::GetClassImpl(__internal__::TemplatedTag<T>{});
	}

	template<typename T>
	inline const TypeInfo* GetType() noexcept
	{
		return __internal__::GetTypeImpl(__internal__::TemplatedTag<T>{});
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

#ifndef _HEIMDALLR_PRIMITIVE_TYPES_
#define _HEIMDALLR_PRIMITIVE_TYPES_

	// set primitive definitions
#define TYPES                                       \
    X(bool, Bool, "%d")                             \
    X(char, Char, "%d")                             \
    X(short, Short, "%d")                           \
    X(int, Int, "%d")                               \
    X(long, Long, "%ld")                            \
    X(long long, LongLong, "%lld")                  \
    X(float, Float, "%.9g")                         \
    X(double, Double, "%.17g")                      \
    X(long double, LongDouble, "%.21Lg")            \
    X(unsigned char, UnsignedChar, "%u")            \
    X(unsigned short, UnsignedShort, "%u")          \
    X(unsigned int, UnsignedInt, "%u")              \
    X(unsigned long, UnsignedLong, "%lu")           \
    X(unsigned long long, UnsignedLongLong, "%llu") \
    X(std::string, String, "%s")                    \

#define X(Type, Name, Format) \
	template <> inline const TypeInfo* GetTypeImpl(TemplatedTag<Type>) noexcept \
	{ \
		struct Name : public TypeInfo { Name(uint64_t id, const char* name, const char* fullname, size_t size) : TypeInfo({ id, name, fullname, size }) {}; }; \
		static Name type(TypeInfo::InvalidID, #Type, #Type, sizeof(Type)); \
		return &type; \
	}\

	namespace __internal__
	{
		TYPES
#undef X

		// definitions of void type (as it is a special type)
		template <> inline const TypeInfo* GetTypeImpl(TemplatedTag<void>) noexcept
		{
			struct Void : public TypeInfo { Void(uint64_t id, const char* name, const char* fullname, size_t size) : TypeInfo({ id, name, fullname, size }) {}; };
			static Void type(TypeInfo::InvalidID, "void", "void", 0);
			return &type;
		}
	}

#endif // _HEIMDALLR_PRIMITIVE_TYPES_
}