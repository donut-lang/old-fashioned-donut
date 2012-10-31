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

#include "../../../geom/Area.h"

namespace chisa {
namespace tk {
namespace widget {

class RenderCommand;

class RenderTree {
private:
	typedef std::vector<RenderCommand*> ListType;
	ListType objects_;
public:
	RenderTree();
	~RenderTree() noexcept(true);
	void reset() noexcept(true);
	void render(gl::Canvas& cv, const geom::Area& area);
	void append(RenderCommand* const cmd);
public:
	typedef ListType::iterator Iterator;
	typedef ListType::const_iterator ConstIterator;
	typedef ListType::reverse_iterator ReverseIterator;
	typedef ListType::const_reverse_iterator ConstReverseIterator;
public:
	std::size_t size() const noexcept{ return this->objects_.size(); };
	RenderCommand* at(std::size_t idx) const noexcept{ return this->objects_.at(idx); };
	Iterator begin() { return this->objects_.begin(); };
	Iterator end() { return this->objects_.end(); };
	ConstIterator cbegin() const { return this->objects_.cbegin(); };
	ConstIterator cend() const { return this->objects_.cend(); };
	ReverseIterator rbegin() { return this->objects_.rbegin(); };
	ReverseIterator rend() { return this->objects_.rend(); };
	ConstReverseIterator crbegin() const { return this->objects_.crbegin(); };
	ConstReverseIterator crend() const { return this->objects_.crend(); };
};

}}}
