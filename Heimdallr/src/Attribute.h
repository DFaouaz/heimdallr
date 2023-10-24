#pragma once


namespace hmdl
{
	struct IAttribute;

	namespace __internal__
	{
		struct IAttributeHolder
		{
			template<typename T>
			bool HasAttribute() const
			{
				static_assert(std::is_base_of<IAttribute, T>::value, "Must be a derived class of Attribute");

				for (const IAttribute* attr : m_Attributes)
				{
					const T* t = dynamic_cast<const T*>(attr);
					if (t != nullptr) return true;
				}

				return false;
			}

			template<typename T>
			const T* GetAttribute() const
			{
				static_assert(std::is_base_of<IAttribute, T>::value, "Must be a derived class of Attribute");

				for (const IAttribute* attr : m_Attributes)
				{
					const T* t = dynamic_cast<const T*>(attr);
					if (t != nullptr) return t;
				}

				return nullptr;
			}

			template<typename T>
			std::vector<const T*> GetAttributes() const
			{
				static_assert(std::is_base_of<IAttribute, T>::value, "Must be a derived class of Attribute");

				std::vector<const T*> result;
				for (const IAttribute* attr : m_Attributes)
				{
					const T* t = dynamic_cast<const T*>(attr);
					if (t != nullptr) result.push_back(t);
				}

				return result;
			}

			const std::vector<const IAttribute*>& GetAllAttributes() const
			{
				return m_Attributes;
			}

		public:
			std::vector<const IAttribute*> m_Attributes;

		};

	}
	struct IAttribute {
		virtual ~IAttribute() {};
	};

	template<typename T, bool AllowMultiple = false>
	struct Attribute : public IAttribute
	{
	public:
		Attribute() {};
		virtual ~Attribute() {};
		inline static constexpr bool CanAllowMultiple() { return AllowMultiple; };
	};

}
