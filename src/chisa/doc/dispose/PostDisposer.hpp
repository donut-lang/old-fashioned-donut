/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <cinamo/ClassUtil.h>
#include <cinamo/Logger.h>
#include <vector>

#include "../node/NodeWalker.hpp"
#include "../../geom/Vector.hpp"

namespace chisa {
namespace doc {

class PostDisposer: public chisa::doc::NodeWalker {
	DEFINE_MEMBER_REF(private, Logger, log);
private:
	std::vector<geom::Point> offsetStack_;
public:
	PostDisposer(Logger& log);
	virtual ~PostDisposer() noexcept = default;
private:
	template <typename T, typename U> void walkBlock(T* block, U clos);
	geom::Point offset();
public:
	virtual void walk(Document* doc) override;
	virtual void walk(Paragraph* para) override;
	virtual void walk(Heading* head) override;
	virtual void walk(Text* text) override;
	virtual void walk(Link* link) override;
	virtual void walk(Font* font) override;
	virtual void walk(BreakLine* br) override;
};

}}
