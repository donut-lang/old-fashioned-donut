/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <donut/object/ReactiveNativeObject.h>
#include <donut/object/Heap.h>
#include "../../chisa/tk/donut/GeistObject.h"
#include "../NesGeist.h"

namespace nes {

struct NesGeistSideEffect {
	enum Operation{
		None=0,
		LoadSave,
		LoadSaveAndRun
	};
	Operation op_before;
	Operation op_after;
	::cinamo::XValue before;
	::cinamo::XValue after;
	NesGeistSideEffect()
	:op_before(None),op_after(None)
	{
	}
	template <typename Arc>
	void serialize(Arc& arc) {
	}
};

class NesGeistProvider;
class NesGeistObject : public ::chisa::tk::GeistObjectBaseT<NesGeistProvider, NesGeistObject, NesGeist, NesGeistSideEffect> {
public:
	NesGeistObject(NesGeistProvider* provider);
	virtual ~NesGeistObject() noexcept = default;
public:
	void onFutureDiscarded(Handler< ::donut::Heap> const& heap);
	void onHistoryDiscarded(Handler< ::donut::Heap> const& heap);
	ResultType onBack(Handler< ::donut::Heap> const& heap, AntiSideEffect const& val);
	ResultType onForward(Handler< ::donut::Heap> const& heap, AntiSideEffect const& val);
	::cinamo::XValue saveImpl( Handler< ::donut::Heap> const& heap ) override final;
	void loadImpl( Handler< ::donut::Heap> const& heap, ::cinamo::XValue const& data ) override final;
public:
	std::tuple<break_id_t, Maybe<NesGeistSideEffect> > addExecBreak(uint16_t addr_first, uint16_t addr_end);
	std::tuple<bool, Maybe<NesGeistSideEffect> > removeExecBreak(break_id_t id);
	std::tuple<std::nullptr_t, Maybe<NesGeistSideEffect> > stepRunning();
	std::tuple<std::nullptr_t, Maybe<NesGeistSideEffect> > continueRunning();
	std::tuple<std::string, Maybe<NesGeistSideEffect> > writeAsmNES(uint16_t addr, std::string val);
	std::tuple<std::string, Maybe<NesGeistSideEffect> > writeMemNES(uint16_t addr, std::string val);
};

class NesGeistProvider : public ::chisa::tk::GeistProviderBaseT<NesGeistProvider, NesGeistObject, NesGeist, NesGeistSideEffect> {
public:
	NesGeistProvider(Handler< ::donut::Heap> const& heap);
	virtual ~NesGeistProvider() noexcept = default;
};

}
