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
#include <tarte/Handler.h>
#include <tarte/Logger.h>
#include <tarte/ClassUtil.h>
#include "audio/Quartet.h"
#include "geom/Area.h"

namespace chisa {
using namespace tarte;

class Chisa;
class PlatformFairy : public HandlerBody<PlatformFairy> {
	DEFINE_MEMBER_REF(protected, Logger, log);
private:
	Handler<Quartet> quartet_;
protected:
	PlatformFairy(Logger& log):log_(log){};
	virtual ~PlatformFairy() noexcept = default;
public:
	inline bool onFree() noexcept { return false; };
public:
	virtual void init(std::string const& windowTitle, int width, int height, int redbits, int greenbits, int bluebits, int alphabits, int depthbits, int stencilbits ) = 0;
	virtual void setTitle(std::string const& name) = 0;
public: /* 画面操作 */
	virtual void swapBuffer() = 0;
public: /* リアル時間操作 */
	virtual unsigned int getTimeMs() = 0;
	virtual void sleepMs(unsigned int ms) = 0;
public: /* イベントの処理を行わせるためのコールバック。falseを返すと、アプリ全体の実行を終了する。 */
	virtual bool pollEvent(Chisa& chisa) = 0;
public: /* IME */
	virtual void startIME(geom::Area const& area) = 0;
	virtual void stopIME() = 0;
public: /* Audio */
	Handler<Quartet> quartet(); //カルテットサブシステムの呼び出し
protected:
	virtual Handler<Quartet> createQuartet() = 0;
};

}
