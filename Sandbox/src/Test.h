#pragma once

#include "heimdallr.h"
#include "CustomAttributes.h"

namespace MyNameSpace
{
	namespace
	{
		HEIMDALLR_STRUCT(DisplayName = "MY_SUPER_STRUCT")
		struct TestStruct
		{
			HEIMDALLR_PROPERTY()
				bool colors[5];

#ifdef _DEBUG
			HEIMDALLR_PROPERTY()
				char c = ',';
#endif
		};

	}
}

HEIMDALLR_STRUCT()
struct NonAnonymNS
{
	HEIMDALLR_STRUCT(DisplayName = "MY_NOT_THAT_SUPER_STRUCT")
		struct TestStruct
	{
		HEIMDALLR_PROPERTY()
			bool colors[5];

		HEIMDALLR_PROPERTY()
			char c;

		HEIMDALLR_FUNCTION()
			void MyVoidFunction(char c) const
		{
			printf("Printing a void function\n");
		}
	};
};


HEIMDALLR_CLASS(Category = "Test Class pero antes")
class TestClass : public NonAnonymNS::TestStruct
{
public:
	HEIMDALLR_ENUM(Category = "Mi Categoria Enumerada")
		enum MyCXXEnum
	{
		NONE,
		NUNU,
		NONO
	};


	HEIMDALLR_PROPERTY(RangeSlider(5.0f, 10.0f), Category = "Hola a todos", HideInEditor)
		char name2[7] = "Nuria";

	HEIMDALLR_PROPERTY()
		std::string nunuName = "NunuMiNinyaBonita";

	HEIMDALLR_PROPERTY()
		MyCXXEnum jahant[5] = { NONE , NONE , NONE , NONO, NUNU };

	HEIMDALLR_PROPERTY()
		MyNameSpace::TestStruct jaat[5];

	HEIMDALLR_PROPERTY()
		bool nnoasd[8] = { true , false , false , true, false, false , true, false };

	HEIMDALLR_PROPERTY()
		int count;

	HEIMDALLR_PROPERTY()
		float x;

	HEIMDALLR_PROPERTY()
		double y;

	HEIMDALLR_PROPERTY()
		char character;

	HEIMDALLR_PROPERTY()
		const char* name = "Roberto";

	HEIMDALLR_PROPERTY()
		MyNameSpace::TestStruct testStruct;

private:
	HEIMDALLR_FUNCTION(Category = "Hola a todos", HideInEditor)
	inline bool MyFunctionWithNameX(char c, bool isAwaken) const
	{
		return false;
	}

	HEIMDALLR_FUNCTION(Category = "Hola a nadie", HideInEditor)
	inline bool MyFunctionWithNameX(char c, int a) const
	{
		return false;
	}

	HEIMDALLR_FUNCTION()
		void MyVoidFunction(char c) const
	{
		printf("Printing a void function\n");
	}

	HEIMDALLR_FUNCTION()
		int PrintXReturnY(int X, int Y) const
	{
		printf("Printing X = %d\n", X);
		return Y;
	}
};
