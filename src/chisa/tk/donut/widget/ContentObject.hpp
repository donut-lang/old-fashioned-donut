/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "../WidgetObject.hpp"
#include "../../widget/ContentWidget.hpp"

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
