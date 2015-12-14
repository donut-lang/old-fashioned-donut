/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <cinamo/Handler.h>
#include <cinamo/ClassUtil.h>
#include <cinamo/Logger.h>
#include "../geom/Vector.hpp"

struct SDL_Keysym;

namespace chisa {
using namespace cinamo;

namespace tk {
class World;
class Element;

class ActionListener {
	DISABLE_COPY_AND_ASSIGN(ActionListener);
public:
	ActionListener() = default;
	virtual ~ActionListener() noexcept = default;
public:
	virtual bool onDownRaw(float const& timeMs, geom::Point const& ptInScreen){return false;};
	virtual bool onUpRaw(float const& timeMs, geom::Point const& ptInScreen){return false;};
	virtual bool onMoveRaw(float const& timeMs, geom::Point const& ptInScreen){return false;};
	virtual bool onSingleTapUp(float const& timeMs, geom::Point const& ptInScreen){return false;};
	virtual bool onFling(float const& timeMs, geom::Point const& start, geom::Point const& end, geom::Velocity const& velocity){return false;};
	virtual bool onScroll(float const& timeMs, geom::Point const& start, geom::Point const& end, geom::Distance const& distance){return false;};
	virtual bool onZoom(float const& timeMs, geom::Point const& center, const float ratio){return false;};
	virtual void onFocusGained(float const& timeMs, geom::Point const& lastPtInScreen){};
	virtual void onFocusLost(float const& timeMs){};
	virtual void onTextInput(float const& timeMs, std::string const& text){};
	virtual void onTextEdit(float const& timeMs, std::string const& text, int const start, int const length){};
	virtual bool onKeyDown(float const& timeMs, bool isRepeat, SDL_Keysym const& sym){return false;};
	virtual bool onKeyUp(float const& timeMs, SDL_Keysym const& sym){return false;};
	// 実装用
	//	virtual bool onDownRaw(float const& timeMs, geom::Point const& ptInScreen) override;
	//	virtual bool onUpRaw(float const& timeMs, geom::Point const& ptInScreen) override;
	//	virtual bool onMoveRaw(float const& timeMs, geom::Point const& ptInScreen) override;
	//	virtual bool onSingleTapUp(float const& timeMs, geom::Point const& ptInScreen) override;
	//	virtual bool onFling(float const& timeMs, geom::Point const& start, geom::Point const& end, geom::Velocity const& velocity) override;
	//	virtual bool onScroll(float const& timeMs, geom::Point const& start, geom::Point const& end, geom::Distance const& distance) override;
	//	virtual bool onZoom(float const& timeMs, geom::Point const& center, const float ratio) override;
	// virtual bool onFocusGained(float const& timeMs, geom::Point const& lastPtInScreen) override;
	// virtual bool onFocusLost(float const& timeMs) override;
	// virtual void onTextInput(float const& timeMs, std::string const& text) override;
	// virtual void onTextEdit(float const& timeMs, std::string const& text, int const start, int const length) override;
	// virtual bool onKeyDown(float const& timeMs, bool isRepeat, SDL_Keysym const& sym) override;
	// virtual bool onKeyUp(float const& timeMs, SDL_Keysym const& sym) override;
};

class ActionSession {
	DISABLE_COPY_AND_ASSIGN(ActionSession);
	DEFINE_MEMBER_REF(private, Logger, log)
public:
	ActionSession(Logger& log, const unsigned int pointerIndex, HandlerW<Element> targetElement, geom::Point const& startPoint, const float startTimeMs);
	virtual ~ActionSession();
private:
	//static constexpr float MaxFlingVelocity=4000;
	static constexpr float MinFlingVelocity=500.0f/1000;
	static constexpr float MaxSingleTapupDistance=120;
private:
	HandlerW<Element> target_;
	std::vector<HandlerW<Element> > elementChain_;
	std::vector<HandlerW<Element> > elementChainSorted_;
	typedef std::vector<HandlerW<Element> >::iterator ElementIterator;
	const unsigned int pointerIndex_;
	const geom::Point startPoint_;
	const float startTimeMs_;
	geom::Point lastPoint_;
	float lastTimeMs_;
private:
	geom::Distance totalMoved_;
public:
	void onTouchUp(float const& timeMs, geom::Point const& pt);
	void onTouchMove(float const& timeMs, geom::Point const& pt);
	void onScroll(float const& timeMs, float const ratio);
	void onFocusGained(float const& timeMs, ActionSession* const last);
	void onFocusLost(float const& timeMs, ActionSession* const last);
	void onTextInput(float const& timeMs, std::string const& text);
	void onTextEdit(float const& timeMs, std::string const& text, int const& start, int const& length);
	void onKeyDown(float const& timeMs, bool isRepeat, SDL_Keysym const& sym);
	void onKeyUp(float const& timeMs, SDL_Keysym const& sym);
private:
	void invokeDownRaw(float const& timeMs, geom::Point const& pt);
	void invokeUpRaw(float const& timeMs, geom::Point const& pt);
	void invokeMoveRaw(float const& timeMs, geom::Point const& pt);
	void invokeFling(float const& timeMs, geom::Point const& start, geom::Point const& end, geom::Velocity const& velocity);
	void invokeScroll(float const& timeMs, geom::Point const& start, geom::Point const& end, geom::Distance const& distance);
	void invokeZoom(float const& timeMs, geom::Point const& center, const float ratio);
	void invokeFocusGained(float const& timeMs,ActionSession* const last);
	void invokeFocusLost(float const& timeMs, ActionSession* const last);
	void invokeTextInput(float const& timeMs, std::string const& text);
	void invokeTextEdit(float const& timeMs, std::string const& text, int const start, int const length);
	void invokeKeyDown(float const& timeMs, bool isRepeat, SDL_Keysym const& sym);
	void invokeKeyUp(float const& timeMs, SDL_Keysym const& sym);
};

class ActionMediator {
	DISABLE_COPY_AND_ASSIGN(ActionMediator);
	DEFINE_MEMBER_REF(private, Logger, log)
	DEFINE_MEMBER_CONST(private, HandlerW<World>, world)
private:
	static constexpr std::size_t MaxTouxhPoint = 5;
	ActionSession* session_[MaxTouxhPoint];
	ActionSession* lastSession_;
private:
	void releaseSession(float const& timeMs, unsigned int const pointerIndex);
public:
	ActionMediator(Logger& log, const HandlerW<World> world);
	virtual ~ActionMediator();
public:
	void onTouchDown(float const& timeMs, const unsigned int pointerIndex, geom::Point const& screenPoint);
	void onTouchUp(float const& timeMs, const unsigned int pointerIndex, geom::Point const& screenPoint);
	void onTouchMove(float const& timeMs, const unsigned int pointerIndex, geom::Point const& screenPoint);
	void onScroll(float const& timeMs, const float ratio);
	void onTextInput(float const& timeMs, std::string const& text);
	void onTextEdit(float const& timeMs, std::string const& text, int const start, int const length);
	void onKeyDown(float const& timeMs, bool isRepeat, SDL_Keysym const& sym);
	void onKeyUp(float const& timeMs, SDL_Keysym const& sym);
};

}}
