/**
 * Chisa
 * Copyright (C) 2012 psi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef Chisa_TK_LAYOUT_SPLITLAYOUT_H__CPP_
#define Chisa_TK_LAYOUT_SPLITLAYOUT_H__CPP_

#include "../Layout.h"
#include <tinyxml2.h>
#include <vector>
#include <memory>

namespace chisa {
namespace tk {
namespace layout {

using namespace tinyxml2;
using namespace std;
class LayoutFactory;

struct SplitDef
{
public:
	const float weight;
	const float min;
	const float max;
	SplitDef(const float weight, const float min=NAN, const float max=NAN)
	:weight(weight), min(min), max(max)
	{
	}
};
struct SplitCtx
{
	const SplitDef def;
	shared_ptr<Layout> layout;
	float size;
	SplitCtx(const SplitDef& def)
	:def(def)
	,layout(nullptr)
	,size(NAN)
	{
	}
};


class SplitLayout: public chisa::tk::Layout {
public:
	enum SplitMode {
		Vertical,
		Horizontal
	};
	struct AttrName{
		static const std::string Weight;
		static const std::string Max;
		static const std::string Min;
	};
protected:
	SplitLayout(enum SplitMode splitMode, CHISA_LAYOUT_SUBKLASS_CONSTRUCTOR_PARAM_LIST);
public:
	virtual ~SplitLayout();
private:
	DEFINE_MEMBER(private, private, enum SplitMode, splitMode);
	DEFINE_MEMBER(private, private, vector<shared_ptr<SplitCtx> >, children)
	float totalSize_;
private:
	inline float wrapSize(float changedSize, const SplitDef& def) const
	{
		changedSize = std::min(changedSize, def.max);
		changedSize = std::max(changedSize, def.min);
		return changedSize;
	}
private:
	float (Box::*changed_getter)() const;
	void (Box::*changed_setter)(float);
	float (Box::*fixed_getter)() const;
	void (Box::*fixed_setter)(float);
	float (Point::*point_getter)(void) const;
	void (Point::*point_setter)(float);
private:
	void addChild(const SplitDef& def, shared_ptr<Layout> layout);
private:
	void resetChildrenLayout();
	float calcTotalSize();
public:
	virtual string toString() override;
	virtual void idle(const float delta_ms) override;
	virtual weak_ptr<Layout> getChildAt(const size_t index) const override;
	virtual size_t getChildCount() const override;
private:
	virtual void renderImpl(gl::Canvas& canvas, const Area& screenArea, const Area& area) override;
	virtual Box onMeasure(const Box& constraint) override;
	virtual void onLayout(const Box& size) override;
	virtual void loadXMLimpl(layout::LayoutFactory* const factory, tinyxml2::XMLElement* const element) override;
	virtual weak_ptr<Layout> getLayoutByIdImpl(const std::string& id) override;
public:
	static shared_ptr<Layout> constructorProxy(enum SplitMode splitMode, CHISA_LAYOUT_SUBKLASS_CONSTRUCTOR_PARAM_LIST)
	{
		return shared_ptr<Layout>(new SplitLayout(splitMode, CHISA_LAYOUT_SUBKLASS_CONSTRUCTOR_PARAM_APPLY));
	}
};

}}}
#endif /* INCLUDE_GUARD */
