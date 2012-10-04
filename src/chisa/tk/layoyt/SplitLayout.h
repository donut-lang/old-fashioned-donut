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
private:
	vector<shared_ptr<SplitCtx> > children_;
	float totalSize_;
public:
	SplitLayout(logging::Logger& log, weak_ptr<World> world, weak_ptr<Layout> root, weak_ptr<Layout> parent);
	virtual ~SplitLayout();
private:
	void addChild(const SplitDef& def, shared_ptr<Layout> layout);
protected:
	inline vector<shared_ptr<SplitCtx> >& children() { return children_; };
	void resetChildrenLayout();
	float calcTotalSize();
public:
	virtual void loadXML(LayoutFactory* const factory, XMLElement* const element) override;
	virtual void idle(const float delta_ms) override;
	virtual weak_ptr<Layout> getChildAt(const size_t index) const override;
	virtual size_t getChildCount() const override;
	virtual weak_ptr<Widget> getWidgetById(const string& id) override;
	inline static float max(const float a, const float b)
	{
		return isnan(b) ? a : (a > b ? a : b);
	}
	inline static float min(const float a, const float b)
	{
		return isnan(a) ? b : (a > b ? b : a);
	}
};

}}}
#endif /* INCLUDE_GUARD */
