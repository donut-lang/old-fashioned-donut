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

#ifndef Chisa_TK_LAYOUT_HORIZONTALLAYOUT_H__CPP_
#define Chisa_TK_LAYOUT_HORIZONTALLAYOUT_H__CPP_

#include "./SplitLayout.h"
#include <tinyxml2.h>
#include <memory>

namespace chisa {
namespace tk {
namespace layout {
using namespace tinyxml2;
using namespace std;
class LayoutFactory;

class HorizontalLayout: public chisa::tk::layout::SplitLayout {
	CHISA_LAYOUT_SUBKLASS_FINAL(HorizontalLayout);
public:
	virtual void render(const Area& area) override;
	virtual Box measure(const Box& constraint) override;
	virtual string toString() override;
protected:
	virtual void reshapeImpl(const Area& area) override;
};
}}}
#endif /* INCLUDE_GUARD */
