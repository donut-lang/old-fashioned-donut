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

#pragma once

#include "../Element.h"
#include <tinyxml2.h>
#include <vector>
#include <memory>

namespace chisa {
namespace tk {
namespace element {

class ElementFactory;

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
	shared_ptr<Element> element;
	float size;
	float weight;
	SplitCtx(const SplitDef& def)
	:def(def)
	,element(nullptr)
	,size(NAN)
	,weight(def.weight)
	{
	}
};


class SplitCombo: public chisa::tk::Element {
	CHISA_ELEMENT_SUBKLASS_FINAL(SplitCombo);
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
private:
	DEFINE_MEMBER(private, private, enum SplitMode, splitMode);
	DEFINE_CONTAINER(private, private, std::vector<shared_ptr<SplitCtx> >, children)
	float totalSize_;
	void setMode(enum SplitMode mode);
private:
	inline float wrapSize(float changedSize, const SplitDef& def) const
	{
		if(geom::isUnspecified(changedSize)){
			return def.min;
		}
		changedSize = std::min(changedSize, def.max);
		changedSize = std::max(changedSize, def.min);
		return changedSize;
	}
private:
	float (geom::Box::*changed_getter)() const;
	void (geom::Box::*changed_setter)(float);
	float (geom::Box::*fixed_getter)() const;
	void (geom::Box::*fixed_setter)(float);
	float (geom::Point::*point_getter)(void) const;
	void (geom::Point::*point_setter)(float);
private:
	void addChild(const SplitDef& def, shared_ptr<Element> element);
private:
	void resetChildren();
	float calcTotalSize();
public:
	virtual string toString() const override;
	virtual void idle(const float delta_ms) override;
	virtual std::weak_ptr<Element> getChildAt(const std::size_t index) const override;
	virtual std::size_t getChildCount() const override;
private:
	virtual void renderImpl(gl::Canvas& canvas, const geom::Area& screenArea, const geom::Area& area) override;
	virtual geom::Box onMeasure(const geom::Box& constraint) override;
	virtual void onLayout(const geom::Box& size) override;
	virtual void loadXMLimpl(element::ElementFactory* const factory, tinyxml2::XMLElement* const element) override;
	virtual std::weak_ptr<Element> getElementByIdImpl(const std::string& id) override;
};

}}}
