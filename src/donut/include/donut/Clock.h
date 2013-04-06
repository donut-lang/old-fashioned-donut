/**
 * Donut
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
#include <cinamo/Handler.h>
#include <cinamo/ClassUtil.h>
#include <cinamo/XVal.h>
#include <cinamo/Logger.h>

namespace donut {
using namespace cinamo;

class Donut;

class Clock final : public HandlerBody<Clock> {
	DEFINE_MEMBER_REF(private, Logger, log)
private:
	std::vector<unsigned int> clockStack_;
	unsigned int last_;
	unsigned int first_;
	unsigned int now_;
private:
	HandlerW<Donut> donut_;
private:
	bool enter_;
	bool discardFutureRequested_;
	bool discardHistoryRequested_;
	bool tickRequested_;
private:
	void processRequest();
	void backStep();
	void forwardStep();
	void tickStep();
	void discardFutureStep();
	void discardHistoryStep();
public:
	Clock( Donut* const donut );
	virtual ~Clock() noexcept = default;
	bool onFree() noexcept { return false; };
private: /* VMから呼ばれる特殊ファンクション */
	enum MachineRequest {
		NONE = 0,
		TICK = 1,
		SEEK = 2,
		BACK = 3,
		FORWARD = 4,
		DISCARD_FUTURE = 5,
		DISCARD_HISTORY = 6
	} machineRequest_;
	unsigned int machineRequestedSeekTime_;
public: /* Machineから呼ばれる、普通は呼ばない */
	void tickFromMachine();
	void seekFromMachine( unsigned int const& time );
	void backFromMachine();
	void forwardFromMachine();
	void discardFutureFromMachine();
	void discardHistoryFromMachine();
	/**
	 * 予約しておいたオペレーションを実際に実行する。
	 * 時間が不自然な動きをした時は、trueを返す
	 */
	inline bool invokeMahcineRequest() { return this->machineRequest_ && this->invokeMahcineRequestInner(); };
private:
	bool invokeMahcineRequestInner();
public: /* 処理系の保存・復帰をします。 */
	void bootstrap();
	XValue save();
	void load( XValue const& data);
public: /* time functions */
	void tick();
	void seek( unsigned int const& time );
	void back();
	void forward();
	void discardFuture();
	void discardHistory();
public: /* 各種Getter */
	inline unsigned int now() const noexcept { return this->now_; };
	inline unsigned int lastTime() const noexcept { return this->last_; };
	inline unsigned int firstTime() const noexcept { return this->first_; };
	inline bool canBack() const noexcept { return this->first_ < this->now_; };
	inline bool canAdvance() const noexcept { return this->now_ < this->last_; };
};

}
