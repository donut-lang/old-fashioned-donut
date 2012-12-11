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

#include "DonutHelper.h"
#include <math.h>

namespace chisa {
namespace donut {
namespace {
class SampleObject : public ReactiveNativeObject {
public:
	bool futureDiscarded;
	bool historyDiscarded;
	bool backable_but_non_fowardable;
	int backable_and_forwardable;
	int backable_only_once;
	SampleObject(std::string const& provicerName)
	:ReactiveNativeObject(provicerName)
	,futureDiscarded(false)
	,historyDiscarded(false)
	,backable_but_non_fowardable(false)
	,backable_and_forwardable(0)
	,backable_only_once(0)
	{}
	virtual util::XValue onBack(Handler<Heap> const& heap, util::XValue const& val) override
	{
		std::string v(val.as<std::string>());
		if(v == "backable_but_non_fowardable"){
			backable_but_non_fowardable = true;
			return util::XValue(); //non-fowardable
		}else if(v == "backable_and_forwardable"){
			++backable_and_forwardable;
			return util::XValue("backable_and_forwardable");
		}else if(v == "backable_only_once") {
			return util::XValue("backable_only_once");
		}
		return util::XValue(2);
	}
	virtual util::XValue onForward(Handler<Heap> const& heap, util::XValue const& val) override
	{
		std::string v(val.as<std::string>());
		if(v == "backable_and_forwardable"){
			++backable_and_forwardable;
			return util::XValue("backable_and_forwardable");
		}else if(v == "backable_only_once"){
			return util::XValue();
		}
		return util::XValue(1);
	}
	virtual void onFutureDiscarded(Handler<Heap> const& heap) {
		this->futureDiscarded = true;
	}
	virtual void onHistoryDiscarded(Handler<Heap> const& heap) {
		this->historyDiscarded = true;
	}
	virtual util::XValue saveImpl( Handler<Heap> const& heap ) override {
		return util::XValue();
	}
	virtual void loadImpl( Handler<Heap> const& heap, util::XValue const& data ) override {

	}
};
class SampleProvider : public HeapProviderImpl<SampleObject>{
public:
	SampleProvider(Handler<Heap> const& heap ):HeapProviderImpl<SampleObject>(heap, "SampleObject"){
		this->registerReactiveNativeClosure("unrecoverable", std::function<std::tuple<std::string, util::XValue>(SampleObject*)>([](SampleObject* obj){
			return std::make_tuple("hey!", util::XValue());
		}));
		this->registerReactiveNativeClosure("backable_but_non_fowardable", std::function<std::tuple<std::string, util::XValue>(SampleObject*)>([](SampleObject* obj){
			return std::make_tuple("hey!", util::XValue("backable_but_non_fowardable"));
		}));
		this->registerReactiveNativeClosure("backable_and_forwardable", std::function<std::tuple<std::string, util::XValue>(SampleObject*)>([](SampleObject* obj){
			return std::make_tuple("hey!", util::XValue("backable_and_forwardable"));
		}));
		this->registerReactiveNativeClosure("backable_only_once", std::function<std::tuple<std::string, util::XValue>(SampleObject*)>([](SampleObject* obj){
			return std::make_tuple("hey!", util::XValue("backable_only_once"));
		}));
	}
};

class ReactiveObjectTest : public ::testing::Test
{
protected:
	Handler<Donut> donut;
	Handler<Clock> clock;
	Handler<Heap> heap;
	Handler<Machine> machine;
protected:
	Handler<SampleProvider> provider;
public:
	void SetUp(){
		donut = Handler<Donut>(new Donut(log_trace));
		machine = donut->queryMachine();
		clock = donut->clock();
		heap = donut->heap();

		provider = Handler<SampleProvider>(new SampleProvider(heap));
		heap->registerProvider(provider);

		donut->bootstrap();
	}
	void TearDown(){
	}
};
TEST_F(ReactiveObjectTest, RegisterTest)
{
	Handler<SampleObject> obj( provider->createDerived() );
	obj->bootstrap(heap);
	heap->setGlobalObject("sample", obj);
	ASSERT_TRUE( heap->hasGlobalObject("sample") );
	ASSERT_TRUE( heap->getGlobalObject("sample")->isObject() );
	Handler<Object> result = machine->start( donut->parse("sample.unrecoverable();") );
	ASSERT_TRUE(result->isObject());
	ASSERT_EQ("hey!", result->toString(heap));
	ASSERT_EQ( donut->nowTime(), donut->firstTime() );
	ASSERT_FALSE( obj->futureDiscarded );
	ASSERT_TRUE( obj->historyDiscarded );
}


TEST_F(ReactiveObjectTest, BackTest)
{
	Handler<SampleObject> obj( provider->createDerived() );
	obj->bootstrap(heap);
	heap->setGlobalObject("sample", obj);
	unsigned int const t1 = donut->nowTime();
	Handler<Object> result = machine->start( donut->parse("sample.backable_but_non_fowardable();") );
	{ //結果と副作用の確認
		ASSERT_TRUE(result->isObject());
		ASSERT_EQ("hey!", result->toString(heap));
		ASSERT_LT( t1, donut->nowTime() );
		ASSERT_EQ( t1, donut->firstTime());
		ASSERT_FALSE( obj->futureDiscarded );
		ASSERT_FALSE( obj->historyDiscarded );
	}
	//シーク
	ASSERT_FALSE( obj->backable_but_non_fowardable );
	donut->seek(t1);
	ASSERT_TRUE( obj->backable_but_non_fowardable ); //副作用の確認
	{//戻れるけど、先には進めない。
		ASSERT_EQ( t1, donut->nowTime() );
		ASSERT_EQ( t1, donut->firstTime());
		ASSERT_EQ( t1, donut->lastTime() );
	}
}

TEST_F(ReactiveObjectTest, BackAndForwardTest)
{
	Handler<SampleObject> obj( provider->createDerived() );
	obj->bootstrap(heap);
	heap->setGlobalObject("sample", obj);
	unsigned int const t1 = donut->nowTime();
	ASSERT_EQ(0, obj->backable_and_forwardable);
	Handler<Object> result = machine->start( donut->parse("sample.backable_and_forwardable();") );
	ASSERT_EQ(0, obj->backable_and_forwardable);
	unsigned int const t2 = donut->nowTime();
	{ //結果と副作用の確認
		ASSERT_LT( t1, donut->nowTime() );
		ASSERT_EQ( t1, donut->firstTime());
		ASSERT_FALSE( obj->futureDiscarded );
		ASSERT_FALSE( obj->historyDiscarded );
	}
	//シーク
	ASSERT_EQ(0, obj->backable_and_forwardable);
	donut->seek(t1);
	{//戻れるし、先に進める
		ASSERT_EQ(1, obj->backable_and_forwardable);
		ASSERT_EQ( t1, donut->nowTime() );
		ASSERT_EQ( t1, donut->firstTime());
		ASSERT_EQ( t2, donut->lastTime() );
	}
	donut->seek(t2);
	{//戻れるし、先に進める
		ASSERT_EQ(2, obj->backable_and_forwardable);
		ASSERT_EQ( t2, donut->nowTime() );
		ASSERT_EQ( t1, donut->firstTime());
		ASSERT_EQ( t2, donut->lastTime() );
	}
	donut->seek(t1);
	{//戻れるし、先に進める
		ASSERT_EQ(3, obj->backable_and_forwardable);
		ASSERT_EQ( t1, donut->nowTime() );
		ASSERT_EQ( t1, donut->firstTime());
		ASSERT_EQ( t2, donut->lastTime() );
	}
	//打ち消しは起こらない。
	ASSERT_FALSE( obj->futureDiscarded );
	ASSERT_FALSE( obj->historyDiscarded );
}

TEST_F(ReactiveObjectTest, BackableOnlyOnceTest)
{
	Handler<SampleObject> obj( provider->createDerived() );
	obj->bootstrap(heap);
	heap->setGlobalObject("sample", obj);
	unsigned int const t1 = donut->nowTime();
	Handler<Object> result = machine->start( donut->parse("sample.backable_only_once();") );
	unsigned int const t2 = donut->nowTime();
	{ //結果と副作用の確認
		ASSERT_LT( t1, donut->nowTime() );
		ASSERT_EQ( t1, donut->firstTime());
		ASSERT_FALSE( obj->futureDiscarded );
		ASSERT_FALSE( obj->historyDiscarded );
	}
	//シーク
	donut->seek(t1);
	{//戻れる
		ASSERT_EQ( t1, donut->nowTime() );
		ASSERT_EQ( t1, donut->firstTime());
		ASSERT_EQ( t2, donut->lastTime() );
		ASSERT_FALSE( obj->futureDiscarded );
		ASSERT_FALSE( obj->historyDiscarded );
	}
	donut->seek(t2);
	{//先に進める
		ASSERT_EQ( t2, donut->nowTime() );
		ASSERT_EQ( t2, donut->firstTime()); //もう戻れなくなる
		ASSERT_EQ( t2, donut->lastTime() );
		ASSERT_FALSE( obj->futureDiscarded );
		ASSERT_TRUE( obj->historyDiscarded );
	}
	ASSERT_ANY_THROW(donut->seek(t1)); //しかし、戻れない
	{
		ASSERT_EQ( t2, donut->nowTime() );
		ASSERT_EQ( t2, donut->firstTime());
		ASSERT_EQ( t2, donut->lastTime() );
	}
	//往復するとDiscardされる
	ASSERT_FALSE( obj->futureDiscarded );
	ASSERT_TRUE( obj->historyDiscarded );
}

}}}


