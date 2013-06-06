/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <cinamo/Handler.h>
#include <cinamo/Logger.h>
#include <cinamo/ClassUtil.h>
#include "audio/Quartet.h"
#include "input/JoystickManager.h"
#include "input/Joystick.h"
#include "geom/Area.h"

namespace chisa {
using namespace cinamo;

class Chisa;
class PlatformFairy : public HandlerBody<PlatformFairy> {
	DEFINE_MEMBER_REF(protected, Logger, log);
private:
	Handler<Quartet> quartet_;
	Handler<JoystickManager> joystickManager_;
protected:
	PlatformFairy(Logger& log):log_(log){};
	virtual ~PlatformFairy() noexcept = default;
public:
	inline bool onFree() noexcept { return false; };
public:
	virtual void init(std::string const& windowTitle, int width, int height, bool isFullScreen, int redbits, int greenbits, int bluebits, int alphabits, int depthbits, int stencilbits ) = 0;
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
public:
	Handler<JoystickManager> joystickManager();
protected:
	virtual Handler<Quartet> createQuartet() = 0;
	virtual Handler<JoystickManager> createJoystickManager() = 0;
};

}
