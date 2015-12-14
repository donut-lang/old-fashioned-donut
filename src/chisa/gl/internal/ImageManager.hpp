/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <deque>
#include <cinamo/Handler.h>
#include <cinamo/Logger.h>
#include <cinamo/ClassUtil.h>

namespace chisa {
using namespace cinamo;
namespace gl {
class Sprite;

namespace internal {
class SpriteManager;

class ImageManager : public HandlerBody<ImageManager> {
	DEFINE_MEMBER_REF(private, Logger, log);
private:
	std::deque<std::pair<std::string, Handler<Sprite> > > imageCache_;
	Handler<Sprite> loadPNG(std::string const& filename);
	HandlerW<SpriteManager> spriteManager_;
public:
	ImageManager(Logger& log, Handler<SpriteManager> spriteManager);
	virtual ~ImageManager() noexcept = default;
	Handler<Sprite> queryImage(std::string const& filename);
public:
	inline bool onFree() noexcept { return false; };
};

}}}
