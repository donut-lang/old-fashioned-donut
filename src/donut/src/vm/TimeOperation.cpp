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

#include <algorithm>
#include <tarte/String.h>
#include <donut/vm/Machine.h>

namespace donut {

const static std::string TAG("Machine/TimeOp");

/**********************************************************************************
 * time functions
 **********************************************************************************/

void Machine::onTickNotify()
{
	if( this->running() ) {
		if(this->log().t()){
			this->log().t(TAG, "New context created.");
		}
		Context& last = this->contextRevs_.back();
		this->contextRevs_.push_back(Context(clock_, last));
	}
}
// 特に何もしない　問題ないはず
// 実行時にdiscardFuture/discardHistoryされるので実行時に駄目になることはないはず
void Machine::onBackNotify()
{
	if( this->running() ) {
		clock_->tick(); //discardHistoryされて、その後tickされる。上のonTickが最後に呼ばれるはず。
	}
}
void Machine::onForwardNotify()
{
}

/**
 * 現在の時刻以降のコンテキストを消し去る。
 * これが実際に必要なのは、ヒープを共有する他のマシンがシークされた後に動き出した時。
 *  あくまでクロックからのみ呼ばれる。自分からは呼び出さない。
 */
void Machine::onDiscardFutureNotify()
{
	timestamp_t const time = clock_->now();
	int idx = 0;
	for(int i=this->contextRevs_.size()-1; i>=0;--i){
		Context& c = this->contextRevs_[i];
		if(time >= c.time_){
			idx = i+1;
			break;
		}
	}
	this->contextRevs_.erase( this->contextRevs_.begin()+idx, this->contextRevs_.end() );
}

/**
 * 現在の時刻以前のコンテキストを消し去ってしまう。
 *  あくまでクロックからのみ呼ばれる。自分からは呼び出さない。
 */
void Machine::onDiscardHistoryNotify()
{
	timestamp_t const time = clock_->now();
	int idx = this->contextRevs_.size();
	const int max = this->contextRevs_.size();
	for(int i=0; i<max;++i){
		Context& c = this->contextRevs_[i];
		if( c.time_ >= time ){
			idx = i;
			break;
		}
	}
	this->contextRevs_.erase( this->contextRevs_.begin(), this->contextRevs_.begin()+idx );
}

}

