#pragma once


#include "PropertyInfo.h"
#include "Attribute.h"

namespace hmdl
{
	struct FunctionInfo : public __internal__::IAttributeHolder
	{
	public:
		FunctionInfo() = default;
		FunctionInfo(
			const char* name,
			const TypeInfo* returnType,
			const PropertyInfo* parameters,
			size_t parametersSize,
			std::function<void()>* invokeFuncPtr,
			const char* comment,
			const char* returnComment,
			const std::vector<const IAttribute*>& attrs
		) :
			m_Name(name),
			m_ReturnType(returnType),
			m_Parameters(parameters),
			m_ParametersSize(parametersSize),
			m_InvokeFuncPtr(invokeFuncPtr),
			m_Comment(comment),
			m_ReturnComment(returnComment),
			IAttributeHolder({ attrs })
		{};

	public:
		inline const char* GetName() const { return m_Name; };
		inline void SetName(const char* name) { m_Name = name; }
		inline const TypeInfo* GetReturnType() const { return m_ReturnType; };
		inline void SetReturnType(const TypeInfo* type) { m_ReturnType = type; };
		inline const PropertyInfo* GetParameters() const { return m_Parameters; };
		inline size_t GetParametersSize() const { return m_ParametersSize; };
		inline void SetParameters(const PropertyInfo* params, size_t size) { m_Parameters = params; m_ParametersSize = size; };
		inline void SetInvokeFuncPtr(std::function<void()>* invokeFuncPtr) { m_InvokeFuncPtr = invokeFuncPtr; };
		
		template<typename ReturnType = void, typename ...Args>
		ReturnType Invoke(void* const obj, Args... args) const
		{
			std::function<ReturnType(void*, Args...)>& func =
				*((std::function<ReturnType(void*, Args...)>*)m_InvokeFuncPtr);

			return func(obj, std::forward<Args>(args)...);
		}

		inline const char* GetComment() const { return m_Comment; };
		inline void SetComment(const char* comment) { m_Comment = comment; };
		inline const char* GetReturnComment() const { return m_ReturnComment; };
		inline void SetReturnComment(const char* returnComment) { m_ReturnComment = returnComment; };

	private:
		const char* m_Name;
		const TypeInfo* m_ReturnType;
		const PropertyInfo* m_Parameters;
		size_t m_ParametersSize;
		std::function<void()>* m_InvokeFuncPtr;
		const char* m_Comment;
		const char* m_ReturnComment;

	};

}