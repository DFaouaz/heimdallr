# Heimdallr - CPP Reflection Tool

Heimdallr is both a library and a code generation tool that generates C++17 compatible reflection code.
It is based of Unreal Engine reflection system so if you are familiar with Unreal Engine it would be a walk in the park.

## How to use it (Code Generation)
Download the last version of the tool. It is recommended to add its execution to your compilation pipeline.
You should execute the tool as follows:
```shell
"heimdallr_x64.exe" "cpp_path" -- -D__GENERATE_HEIMDALLR_REFLECTION_ANNOTATION__ -DNDEBUG -D_UNICODE -DUNICODE -fparse-all-comments -x c++ -std=c++17 -I"heimdall_include_path" -I"compiler_include_path" -I"other_compiler_include_path" 
```

It is based of clang so every clang compatible parameter should be compatible too.
It generates two files. A "*YourFile*.heimdallr_generated.h" and a "*YourFile*.heimdallr_generated.cpp". Both in the same folder of the original "*YourFile*.cpp/h". Those files contain the generated code and **SHOULD** be compiled along with your code.

## How to use it (Library)
As mentioned before, the syntax is similar to Unreal Engines. Here is an example of usage:
```cpp
/*
* This is a comment that can be accessed from the reflection data if needed
*/
HEIMDALLR_CLASS(Category = "This is a category attribute")
class TestClass : public MyNameSpace::TestStruct
{
public:
	// Another comment associated with the enum declaration
	HEIMDALLR_ENUM(DisplayName = "This is a category attribute")
    enum TestEnum
	{
		ONE,    // These values can also have a comment associated to them
		TWO,    // And this
		THREE   // And this one too.
	};

	HEIMDALLR_PROPERTY()
    std::string str = "string";

	HEIMDALLR_PROPERTY()
    MyNameSpace::TestStruct structArr[5];

	HEIMDALLR_PROPERTY()
    bool boolArr[8] = { true, false, false, true, false, false, true, false };

	HEIMDALLR_PROPERTY()
    int count;

	HEIMDALLR_PROPERTY()
    const char* name = "Charles";

private:
	/**
	 * Function comment (available from reflection data)
	 * 
	 * @param c Parameter comment (also available from the reflection data)
	 * @param isAwaken Parameter comment 2
	 * @return Return comment (also available)
	 */
	HEIMDALLR_FUNCTION()
	inline bool DoNothing(char c, bool isAwaken) const
	{
		return false;
	}

	HEIMDALLR_FUNCTION()
	void PrintFunction(char c) const
	{
		printf("Printing a void function\n");
	}
};
```

As shown in the previous example, its syntax is minimal but powerfull. All the macros serve as way to let know which info should be generated though the code generation tool. You have the following macros available to use:

- **HEIMDALLR_CLASS() / HEIMDALLR_STRUCT()**: for classes or structs.	
- **HEIMDALLR_PROPERTY()**: for member variables inside a class or a struct.
- **HEIMDALLR_FUNCTION()**: for member functions inside a class or a struct.	
- **HEIMDALLR_ENUM()**: for enums and enum classes declarations.

# Reflection Data
The following classes holds different information for the diferent kinds of data. Usually can be adquired by type or name (it is better to use the full scoped name, it will return the first ocurrence if multiple classes are called "TestClass")
## hdml::TypeInfo
Holds the information of any reflected type.
```cpp
const hmdl::TypeInfo* tInfo = hmdl::GetType<std::string>();
const hmdl::TypeInfo* tInfo = hmdl::GetType("std::string");
const hmdl::TypeInfo* tInfo = hmdl::GetType("string");
```
## hdml::ClassInfo
Holds the information of a given class/struct and its properties and methods.
```cpp
const hmdl::ClassInfo* cInfo = hmdl::GetClass<TestClass>();
const hmdl::ClassInfo* cInfo = hmdl::GetClass("TestNamespace::TestClass");
const hmdl::ClassInfo* cInfo = hmdl::GetClass("TestClass");
```
## hdml::PropertyInfo
Holds the information of a given class/struct and its properties.
```cpp
const hmdl::ClassInfo* cInfo = hmdl::GetClass<TestClass>();
const hmdl::PropertyInfo* pInfo = cInfo->GetField("name");
const hmdl::PropertyInfo* pInfo = cInfo->GetFields()[0];
```
## hdml::FunctionInfo
Holds the information of a given class/struct and its properties.
```cpp
const hmdl::ClassInfo* cInfo = hmdl::GetClass<TestClass>();
const hmdl::FunctionInfo* fInfo = cInfo->GetMethod("PrintFunction");
const hmdl::FunctionInfo* fInfo = cInfo->GetMethods()[0];
```
## hdml::EnumInfo
Holds the information of a given class/struct.
```cpp
const hmdl::EnumInfo* eInfo = hmdl::GetEnum<TestClass::TestEnum>();
const hmdl::EnumInfo* eInfo = hmdl::GetEnum("TestClass::TestEnum");
const hmdl::EnumInfo* eInfo = hmdl::GetEnum("TestEnum");
```
You can iterate over a reflected enum type:
```cpp
for (const TestClass::TestEnum& value : hmdl::EnumRange<TestClass::TestEnum>())
{
    const char* str;
    const hmdl::EnumInfo* eEnum = hmdl::GetEnum<TestClass::MyCXXEnum>();
    if (eEnum->TryGetStringByValue(TestClass::MyCXXEnum::NONO, str))
    {
        printf("Value: %s", str);
    }
}
```
# Attributes (and custom ones)
All macros accepts any kind of attribute. The difference with Unreal is that with Heimdallr you have type safety with attributes. So if you put an attribute with the wrong syntax or even a non existing attribute, a compilation error will appear.
```cpp
struct RangeSlider : public hmdl::Attribute<RangeSlider>
{
	RangeSlider(float min, float max) : min(min), max(max) {};
	virtual ~RangeSlider() override {};

private:
	float min;
	float max;
};
```
In this example we can see how to make a custom attribute. It is important to inherit from `hmdl::Attribute<T, bool>` which is a templated class that needs:
- T: MUST be the type of the custom attribute.
- bool (optional, default to ``false``): whether or not this attribute can have multiple instances for a single declaration.

Attributes can be written in three different formats:
```cpp
HEIMDALLR_PROPERTY(RangeSlider(1.0f, 5.0f))
HEIMDALLR_PROPERTY(RangeSlider = { 1.0f, 5.0f })
HEIMDALLR_PROPERTY(Category = "Some category name") // only for "one paramenter" attribute constructors
```

# TODO
- Add support for templated classes
- Add support for static functions
- Add support for global scoped functions
- Add a HEIMDALLR_PARAM() macro to add attributes to enum values or function parameters (maybe different macros).
- Probably more stuff.

