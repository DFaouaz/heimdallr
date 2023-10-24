#pragma once

#include <heimdallr.h>


struct RangeSlider : public hmdl::Attribute<RangeSlider>
{
	RangeSlider(float min, float max) : min(min), max(max) {};
	virtual ~RangeSlider() override {};

private:
	float min;
	float max;
};

struct HideInEditor : public hmdl::Attribute<HideInEditor, false>
{
	HideInEditor() {};
	virtual ~HideInEditor() override {};
};

struct Category : public hmdl::Attribute<Category, false>
{
	Category(const char* category) : str(category) {};
	virtual ~Category() override {};

private:
	const char* str;
};

struct DisplayName : public hmdl::Attribute<DisplayName, false>
{
	DisplayName(const char* name) : str(name) {};
	virtual ~DisplayName() override {};

private:
	const char* str;
};

