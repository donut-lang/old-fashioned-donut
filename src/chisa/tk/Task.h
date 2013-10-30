/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <unordered_set>
#include <list>
#include <functional>
#include <cinamo/Logger.h>
#include <cinamo/functional/Base.h>

namespace chisa {
using namespace cinamo;
namespace tk {

class Task {
private:
	Logger& log_;
protected:
	inline Logger& log() const { return log_; };
public:
	Task(Logger& log):log_(log){};
	virtual ~Task() {};
public:
	// タスクが実行される。
	// boolがTrueを返す間は、実行され続ける。アニメーションなどにご活用下さい。
	virtual bool exec(const float delta_ms) = 0;
};

class TaskHandler {
private:
	Logger& log;
private:
	std::list<std::function<bool(float)> > lambas_;
public:
	TaskHandler(Logger& log);
	virtual ~TaskHandler() = default;
public:
	void send( std::function<bool()> const& lambda );
	void send( std::function<bool(float)> const& lambda );
	void send( std::function<void()> const& lambda );
	template <typename R, typename... Args>
	void send( std::function<R(Args...)> const& lambda ) = delete;
	template <typename __Lambda>
	void send( __Lambda lambda ){
		this->send( ::cinamo::makeFunctor(lambda) );
	}
	std::size_t numTasks() const noexcept { return this->lambas_.size(); };
public:
	void run(const float delta_ms);
};

}}
