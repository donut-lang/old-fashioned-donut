/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#include <algorithm>
#include <cinamo/String.h>
#include <donut/vm/Machine.hpp>

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
	int idx = this->contextRevs_.empty() ? 0 : this->contextRevs_.size()-1;
	const int max = this->contextRevs_.size();
	for(int i=0; i<max-1;++i){
		Context& c = this->contextRevs_[i];
		if( c.time_ >= time ){
			idx = i;
			break;
		}
	}
	this->contextRevs_.erase( this->contextRevs_.begin(), this->contextRevs_.begin()+idx );
}

}
