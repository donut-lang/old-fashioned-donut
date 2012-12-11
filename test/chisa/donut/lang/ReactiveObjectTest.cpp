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
	bool futureDiscarded = false;
	bool historyDiscarded = false;
	SampleObject(std::string const& provicerName)
	:ReactiveNativeObject(provicerName)
	,futureDiscarded(false)
	,historyDiscarded(false)
	{}
	virtual util::XValue onBack(Handler<Heap> const& heap, util::XValue const& val) override
	{
		return util::XValue(2);
	}
	virtual util::XValue onForward(Handler<Heap> const& heap, util::XValue const& val) override
	{
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
class SampleProvider : public HeapObjectProviderImpl<SampleObject>{
public:
	SampleProvider(Handler<Heap> const& heap ):HeapObjectProviderImpl<SampleObject>(heap, "SampleObject"){
		this->registerReactiveNativeClosure("do", std::function<std::tuple<std::string, util::XValue>(SampleObject*)>([](SampleObject* obj){
			return std::make_tuple("hey!", util::XValue());
		}));
	}
};

TEST(ReactiveObjectTest, RegisterTest)
{
	Handler<Donut> donut(new Donut(log_trace));
	Handler<Heap> heap( donut->heap() );
	Handler<Machine> machine = donut->queryMachine();

	Handler<SampleProvider> provider(new SampleProvider(heap));

	heap->registerProvider( provider );
	donut->bootstrap();

	Handler<SampleObject> obj( provider->createDerived() );
	obj->bootstrap(heap);
	heap->setGlobalObject("sample", obj);
	ASSERT_TRUE( heap->hasGlobalObject("sample") );
	ASSERT_TRUE( heap->getGlobalObject("sample")->isObject() );
	Handler<Object> result = machine->start( donut->parse("sample.do();") );
	ASSERT_TRUE(result->isObject());
	ASSERT_EQ("hey!", result->toString(heap));
	ASSERT_EQ( donut->nowTime(), donut->firstTime() );
	ASSERT_FALSE( obj->futureDiscarded );
	ASSERT_TRUE( obj->historyDiscarded );

}
}}}


