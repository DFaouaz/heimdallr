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
// Esta clase es una clase muy chida
struct NonAnonymNS
{
	HEIMDALLR_STRUCT(DisplayName = "MY_NOT_THAT_SUPER_STRUCT")
		struct TestStruct
	{
		/* Que chulada de comentario*/
		HEIMDALLR_PROPERTY()
			bool colors[5];

		/* 
		Que chulada de comentario
		Pero no sabria decirte
		*/
		HEIMDALLR_PROPERTY()
			char c;

		// jaja
		char hola;

		/* Esto es un comentario para una funcion muy chula xD*/
		HEIMDALLR_FUNCTION()
			void MyVoidFunction(char c) const
		{
			printf("Printing a void function\n");
		}

		// JEJEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
		// JEJEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
		// JEJEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
		HEIMDALLR_FUNCTION()
			void MyVoidFunction(int c) const
		{
			printf("Printing a void function\n");
		}

		HEIMDALLR_FUNCTION()
		// JEJEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
			void MyVoidFunction(bool c) const
		{
			printf("Printing a void function\n");
		}
	};
};

/*
* Me encantaria poder coger las estrellas con la mano
*/
HEIMDALLR_CLASS(Category = "Test Class pero antes")
class TestClass : public NonAnonymNS::TestStruct
{
public:
	// Comentario de un enum dentro de una clase TestClass
	HEIMDALLR_ENUM(Category = "Mi Categoria Enumerada")
		enum MyCXXEnum
	{
		NONE, // None de que no hace nada
		NUNU, // Nunu de que es mi amor
		NONO  // Nono de que no quiero pero voy a hacerlo igual
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
		bool nnoasd[8] = { true, false, false, true, false, false, true, false };

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
	/**
	 * Te amo mi amor.
	 * 
	 * @param c Coas que Pasan
	 * @param isAwaken Estas despierto o no?
	 * @return  Lo que devuelve
	 */
	HEIMDALLR_FUNCTION(Category = "Hola a todos", HideInEditor)
	inline bool MyFunctionWithNameX(char c, bool isAwaken) const
	{
		return false;
	}

	HEIMDALLR_FUNCTION(Category = "Hola a todos", HideInEditor)
	inline bool MyFunctionWithNameXYZ(char c, bool isAwaken) const
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
