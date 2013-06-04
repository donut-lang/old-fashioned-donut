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
#include "../WidgetObject.h"
#include "../../widget/ContentWidget.h"

namespace chisa {
namespace tk {

struct ContentSideEffect : public WidgetSideEffect {
	enum {
		None=0,
		LoadDocument
	} op;
	std::string docId;
	ContentSideEffect()
	:op(None)
	{
	}
	template <typename Arc>
	void serialize(Arc& arc) {
		Super::serialize(arc);
	}
};

class ContentProvider;
class ContentObject : public WidgetObjectBaseT<ContentProvider, ContentObject, ContentWidget, ContentSideEffect> {
public:
	ContentObject(ContentProvider* provider);
	virtual ~ContentObject() noexcept = default;
private:
	ResultType execAntiSideEffect(Handler<Heap> const& heap, AntiSideEffect const& val);
public:
	void onFutureDiscarded(Handler<Heap> const& heap);
	void onHistoryDiscarded(Handler<Heap> const& heap);
	ResultType onBack(Handler<Heap> const& heap, AntiSideEffect const& val);
	ResultType onForward(Handler<Heap> const& heap, AntiSideEffect const& val);
	XValue saveImpl( Handler<Heap> const& heap ) override final;
	void loadImpl( Handler<Heap> const& heap, XValue const& data ) override final;
public:
	std::tuple<std::string, Maybe<ContentSideEffect> > loadDocument( std::string const& id );
};

class ContentProvider : public WidgetProviderBaseT<ContentProvider, ContentObject, ContentWidget, ContentSideEffect> {
public:
	ContentProvider(Handler<Heap> const& heap, Handler<World> const& world);
	virtual ~ContentProvider() noexcept = default;
};

}}
