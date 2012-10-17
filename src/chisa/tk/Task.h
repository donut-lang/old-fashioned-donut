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
#include <set>
#include <memory>
#include "../logging/Logger.h"

namespace chisa {
namespace tk {

class Task {
private:
	logging::Logger& log_;
protected:
	inline logging::Logger& log() const { return log_; };
public:
	Task(logging::Logger& log):log_(log){};
	virtual ~Task() {};
public:
	// タスクが実行される。
	// boolがTrueを返す間は、実行され続ける。アニメーションなどにご活用下さい。
	virtual bool exec(const float delta_ms) = 0;
};

class TaskHandler {
private:
	logging::Logger& log;
private:
	std::set<Task*> taskPool;
public:
	TaskHandler(logging::Logger& log);
	virtual ~TaskHandler();
public:
	void registerTask(Task* task);
	void unregisterTask(Task* task);
	std::size_t getPostCount() const{ return taskPool.size(); };
public:
	void run(const float delta_ms);
};

}}
