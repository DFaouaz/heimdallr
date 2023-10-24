#pragma once

enum Qualifier : int16_t
{
	CONST = 1 << 0,
	VOLATILE = 1 << 1,
	MUTABLE = 1 << 2,
	AUTO = 1 << 3,
	REGISTER = 1 << 4,
	STATIC = 1 << 5,
	EXTERN = 1 << 6,
	INLINE = 1 << 7
};

enum TypeSpec : int16_t
{
	REFERENCE = 1 << 0,
	POINTER = 1 << 1,
	ARRAY = 1 << 2,
};

enum AccessSpecifier : int16_t
{
	NONE,
	PUBLIC,
	PRIVATE,
	PROTECTED
};

