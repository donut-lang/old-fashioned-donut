/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <vector>
#include <cinamo/Handler.h>
#include <cinamo/ClassUtil.h>
#include <cinamo/XVal.h>
#include "../Decl.hpp"

namespace donut {
using namespace cinamo;
class Heap;
class Object;

class Slot final {
	STACK_OBJECT(Slot);
private:
	std::vector<std::pair<timestamp_t, Object*> > rev_;
	int index_;
public:
	Slot();
	~Slot() noexcept = default;
public:
	Slot(Slot const& other):rev_(other.rev_),index_(other.index_){}
	Slot(Slot&& other):index_(other.index_){ other.rev_.swap(rev_); }
	Slot& operator=(Slot const& other) { rev_=other.rev_;index_=other.index_;return *this; }
	Slot& operator=(Slot&& other) { rev_.swap(other.rev_);index_=other.index_;return *this; }
private:
	timestamp_t firstGen() const noexcept;
	timestamp_t lastGen() const noexcept;
public:
	void mark(Handler<Heap> const& heap, int color);
	Object* load() const;
	Object* store( Handler<Heap> const& heap, Object* obj );
	bool have() const;
public: /* 処理系の保存・復帰をします。 */
	XValue save();
	Slot( Handler<Heap> const& heap, XValue const& data );
public: /* クロックから呼ばれる */
	void onDiscardHistoryNotify( Handler<Heap> const& heap );
	void onDiscardFutureNotify( Handler<Heap> const& heap );
	void onBackNotify(Handler<Heap> const& heap);
	void onForwardNotify(Handler<Heap> const& heap);
};

}
