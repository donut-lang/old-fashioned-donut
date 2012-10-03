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

#ifndef Chisa_TK_LAYOUT_H__CPP_
#define Chisa_TK_LAYOUT_H__CPP_

#include "Widget.h"
#include "../util/class_utils.h"
#include <memory>
#include <string>

namespace chisa {
namespace tk {
using std::string;
using std::weak_ptr;
using std::size_t;

class Layout {
	DISABLE_COPY_AND_ASSIGN(Layout);
private:
	logging::Logger& log_;
	weak_ptr<World> world_;
	weak_ptr<Layout> root_;
	weak_ptr<Layout> parent_;
protected:
	inline const logging::Logger& log() const { return log_; }
	inline const weak_ptr<World>& world() const { return world_; }
	inline const weak_ptr<Layout>& root() const { return root_; }
	inline const weak_ptr<Layout>& parent() const { return parent_; }
public:
	weak_ptr<Layout> getChildAt(const size_t index) const;
	size_t getChildCount() const;
	weak_ptr<Widget> getWidgetById(const string& id);
public:
	virtual void render(const Area& area) = 0; /* OpenGLの座標の設定などを行なってしまう */
	virtual void idle(const float delta_ms) = 0;
	virtual void reshape(const Box& area) = 0;
public:
	Layout(logging::Logger& log, weak_ptr<World> world, weak_ptr<Layout> root, weak_ptr<Layout> parent);
	virtual ~Layout();
	virtual string toString();
};

}}
#endif /* INCLUDE_GUARD */
