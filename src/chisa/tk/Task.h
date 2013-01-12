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
#include <unordered_set>
#include <vector>
#include <functional>
#include <tarte/Logger.h>
#include <tarte/TypeTrans.h>

namespace chisa {
using namespace tarte;
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
	std::unordered_set<Task*> taskPool;
	std::vector<std::function<bool(float)> > lambas_;
public:
	TaskHandler(Logger& log);
	virtual ~TaskHandler();
public:
	void registerTask(Task* task);
	void unregisterTask(Task* task);
public:
	void send( std::function<bool()> const& lambda );
	void send( std::function<bool(float)> const& lambda );
	void send( std::function<void()> const& lambda );
	template <typename __Lambda>
	void send( __Lambda lambda ){
		this->send( ::tarte::makeFunctor(lambda) );
	}
	std::size_t numTasks() const noexcept { return this->lambas_.size(); };
	std::size_t getPostCount() const{ return taskPool.size(); };
public:
	void run(const float delta_ms);
};

}}
