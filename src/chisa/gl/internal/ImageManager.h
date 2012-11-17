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
#include <deque>
#include "../../Handler.h"
#include "../../logging/Logger.h"
#include "../../util/ClassUtil.h"

namespace chisa {
namespace gl {
class Sprite;

namespace internal {
class SpriteManager;

class ImageManager : public HandlerBody<ImageManager> {
	DEFINE_MEMBER_REF(private, logging::Logger, log);
private:
	std::deque<std::pair<std::string, Handler<Sprite> > > imageCache_;
	Handler<Sprite> loadPNG(const std::string& filename);
	HandlerW<SpriteManager> spriteManager_;
public:
	ImageManager(logging::Logger& log, Handler<SpriteManager> spriteManager);
	virtual ~ImageManager() noexcept = default;
	Handler<Sprite> queryImage(const std::string& filename);
public:
	inline bool onFree() noexcept { return false; };
};

}}}
