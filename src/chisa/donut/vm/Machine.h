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
#include <vector>
#include "../object/Object.h"
#include "../object/Heap.h"
#include "../source/Source.h"

namespace chisa {
namespace donut {
class Heap;
typedef unsigned int pc_t;

/**
 * クロージャの実行状態
 */
struct Callchain final {
	pc_t pc_; // Program counter
	unsigned int stackBase_;
	Handler<Object> self_; //このクロージャの対象self
	Handler<DonutClosureObject> closure_; //クロージャ本体
	Handler<DonutObject> scope_; //ローカル変数の格納されるオブジェクト
public:
	Callchain(pc_t pc, unsigned int const& stackBase, Handler<Object> const& self, Handler<DonutClosureObject> const& closure, Handler<DonutObject> const& scope)
	:pc_(pc), stackBase_(stackBase), self_(self), closure_(closure), scope_(scope){
	}
	Callchain(Handler<Heap> const& heap, util::XValue const& data);
	util::XValue save();
};

/**
 * 機械の実行状態。
 * 命令実行とは、このコンテキストの状態が変化していくこと
 */
struct Context final {
	unsigned int time_;
	std::vector<Handler<Object> > stack_;
	std::vector<Handler<Object> > local_;
	std::vector<Callchain> callStack_;
	Context(Handler<Clock> const& clk);
	Context(Handler<Clock> const& clk, Context const& other);
	Context(Handler<Heap> const& heap, util::XValue const& data);
	~Context() noexcept = default;
	util::XValue save();
public:
	struct CompareByTime : std::binary_function<Context const&, Context const&, bool> {
		bool operator()(Context const& a, Context const& b) const noexcept
		{
			return a.time_ < b.time_;
		}
	};
};

class Machine final : public HandlerBody<Machine> {
	DEFINE_MEMBER_REF(private, logging::Logger, log);
private: /* 何があっても不変なもの。*/
	Handler<Clock> const& clock_;
	Handler<Heap> const heap_;
private: /* 時とともに変わっていくもの */
	std::vector<Context> contextRevs_;
private: /* それへのアクセス手段の提供。 */
	Handler<Object> const& self();
	Handler<DonutObject> const& scope();
	Handler<DonutClosureObject> const& closureObject();
	Handler<Source> const& src();
	Handler<Closure> const& closure();
	std::vector<Handler<Object> >& local();
	std::vector<Handler<Object> >& stack();
	unsigned int stackBase();
	std::vector<Callchain>& callStack();
	pc_t& pc();
public: /* 生成 */
	Machine(logging::Logger& log, Handler<Clock> const& clock, Handler<Heap> const& heap);
	virtual ~Machine() noexcept = default;
	bool onFree() noexcept { return false; };
public: /* 外部からの実行指示 */
	Handler<Object> start( Handler<Source> const& src );
	Handler<Object> startContinue( Handler<Object> const& obj );
	bool isInterrupted() const noexcept;
private: /* スタック操作 */
	void pushStack( Handler<Object> const& obj );
	Handler<Object> popStack();
	Handler<Object> topStack();
private: /* 実行 */
	void enterClosure(Handler<Object> const& self, Handler<DonutClosureObject> const& clos, Handler<Object> const& args);
	bool leaveClosure();
	bool fetchPC( Instruction& inst );
	Handler<Object> run();
public: /* 処理系の保存・復帰をします。 */
	void bootstrap();
	util::XValue save();
	void load( util::XValue const& data);
public: /* 時間操作 */
	void onSeekNotify();
	void onDiscardFutureNotify();
	void onDiscardHistoryNotify();
};

}}
