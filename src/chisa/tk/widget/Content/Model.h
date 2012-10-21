/*
 * Model.h
 *
 *  Created on: Oct 21, 2012
 *      Author: psi
 */

#pragma once
#include "../../../logging/Logger.h"
#include "../../../util/ClassUtil.h"
#include "Decl.h"
#include "ModelWalker.h"
#include <memory>
#include <vector>

namespace chisa {
namespace tk {
namespace widget {

class Model {
	DISABLE_COPY_AND_ASSIGN(Model);
	DEFINE_MEMBER_CONST(protected, std::weak_ptr<Model>, root);
	DEFINE_MEMBER_CONST(protected, std::weak_ptr<Model>, parent);
public:
	Model(std::weak_ptr<Model> root, std::weak_ptr<Model> parent);
	virtual ~Model() noexcept = default;
public:
	virtual void walked(ModelWalker& walker) = 0;
};

#define MODEL_SUBKLASS_PARAM_LIST std::weak_ptr<Model> root, std::weak_ptr<Model> parent
#define MODEL_SUBKLASS_PARAM_APPLY root, parent

#define MODEL_SUBKLASS_DESTRUCTOR(Klass) virtual ~Klass() noexcept = default;
#define MODEL_SUBKLASS_WALK(Klass) 	virtual void walked(ModelWalker& walker) override { walker.walk(this); };

#define MODEL_SUBKLASS(Klass)\
public:\
	Klass(MODEL_SUBKLASS_PARAM_LIST);\
	MODEL_SUBKLASS_DESTRUCTOR(Klass);

#define MODEL_SUBKLASS_WITH_IMPL(Klass,Derived)\
public:\
	Klass(MODEL_SUBKLASS_PARAM_LIST):Derived(MODEL_SUBKLASS_PARAM_APPLY){}\
	MODEL_SUBKLASS_DESTRUCTOR(Klass);\
	MODEL_SUBKLASS_WALK(Klass);

#define MODEL_SUBKLASS_WITH_IMPL_PARENT(Klass,Derived)\
public:\
	Klass(MODEL_SUBKLASS_PARAM_LIST):Derived(MODEL_SUBKLASS_PARAM_APPLY){}\
	MODEL_SUBKLASS_DESTRUCTOR(Klass);

#define MODEL_SUBKLASS_CONST_DERIVED(Klass, Derived) Klass::Klass(MODEL_SUBKLASS_PARAM_LIST):Derived(MODEL_SUBKLASS_PARAM_APPLY)

class TreeModel : public Model {
	MODEL_SUBKLASS_WITH_IMPL_PARENT(TreeModel, Model);
	DEFINE_MEMBER_CONST(protected, std::vector<std::shared_ptr<Model> >, children);
public:
	typedef std::vector<std::shared_ptr<Model> >::iterator Iterator;
	typedef std::vector<std::shared_ptr<Model> >::const_iterator ConstIterator;
public:
	inline Iterator begin() { return children().begin(); };
	inline Iterator end() { return children().end(); };
	inline ConstIterator cbegin() const { return children().cbegin(); };
	inline ConstIterator cend() const { return children().cend(); };
	inline void add(std::shared_ptr<Model> child) { children().push_back(child); };
	inline size_t count() const { return children().size(); };
	inline std::shared_ptr<Model> at(size_t idx) const { return children().at(idx); };
};

class BlockModel : public TreeModel {
	MODEL_SUBKLASS_WITH_IMPL_PARENT(BlockModel, TreeModel);
};

class InlineModel : public TreeModel {
	MODEL_SUBKLASS_WITH_IMPL_PARENT(InlineModel, TreeModel);
};

class Paragraph : public BlockModel {
	MODEL_SUBKLASS_WITH_IMPL(Paragraph, BlockModel);
};

class Heading : public BlockModel {
	DEFINE_MEMBER_CONST(public, int, level);
public:
	Heading(MODEL_SUBKLASS_PARAM_LIST, int const level);
	MODEL_SUBKLASS_DESTRUCTOR(Heading);
	MODEL_SUBKLASS_WALK(Heading);
};

class Text : public Model {
private:
	std::string text_;
public:
	Text(MODEL_SUBKLASS_PARAM_LIST, std::string text);
	MODEL_SUBKLASS_DESTRUCTOR(Text);
	MODEL_SUBKLASS_WALK(Text);
};

}}}
