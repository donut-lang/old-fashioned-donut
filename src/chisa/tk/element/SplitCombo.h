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

#include "ElementGroup.h"

namespace chisa {
namespace tk {

class ElementFactory;

struct SplitComboContext
{
	struct Request{
	public:
		float weight;
		float min;
		float max;
		Request(const float weight, const float min, const float max)
		:weight(weight), min(min), max(max){}
	} def;
	float size;
	float weight;
	SplitComboContext(const float weight, const float min=NAN, const float max=NAN)
	:def(weight, min, max),size(NAN),weight(def.weight){}
	SplitComboContext():def(0, NAN, NAN),size(NAN),weight(def.weight){}
};

class SplitCombo: public ElementGroupBase<SplitComboContext> {
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
	DEFINE_MEMBER(public, private, enum SplitMode, splitMode);
	float totalSize_;
public:
	void setMode(enum SplitMode mode);
private:
	inline float wrapSize(float changedSize, SplitComboContext::Request const& def) const
	{
		if(geom::isUnspecified(changedSize)){
			return def.min;
		}
		changedSize = std::min(changedSize, def.max);
		changedSize = std::max(changedSize, def.min);
		return changedSize;
	}
private: //どちらの方向に伸びるかで、アクセサを使い分ける
	float (geom::Box::*changed_getter)() const;
	void (geom::Box::*changed_setter)(float);
	float (geom::Box::*fixed_getter)() const;
	void (geom::Box::*fixed_setter)(float);
	float (geom::Point::*point_getter)(void) const;
	void (geom::Point::*point_setter)(float);
private:
	void resetChildren();
	float calcTotalSize();
public:
	virtual std::string toString() const override;
private:
	virtual void renderImpl(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask) override;
	virtual geom::Box measureImpl(geom::Box const& constraint) override;
	virtual void layoutImpl(geom::Distance const& offsetFromParent, geom::Box const& size) override;
	virtual void loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element) override;
};

}}
