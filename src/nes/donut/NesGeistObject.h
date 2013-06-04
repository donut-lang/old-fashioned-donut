/**
 * Chisa
 * Copyright (C) 2013 psi
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
