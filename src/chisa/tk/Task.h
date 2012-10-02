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

#ifndef Chisa_TK_TASK_H__CPP_
#define Chisa_TK_TASK_H__CPP_

#include <vector>
#include <memory>

namespace chisa {
namespace tk {
using std::vector;
using std::shared_ptr;

class Task {
public:
	Task();
	virtual ~Task();
public:
	// タスクが実行される。
	// boolがTrueを返す間は、実行され続ける。アニメーションなどにご活用下さい。
	virtual bool exec(const float delta_ms) = 0;
};

class TaskHandler {
private:
	vector<shared_ptr<Task> > taskPool;
public:
	TaskHandler();
	virtual ~TaskHandler();
public:
	bool post(shared_ptr<Task> task);
	std::size_t getPostCount() const{ return taskPool.size(); };
};

}}
#endif /* INCLUDE_GUARD */
