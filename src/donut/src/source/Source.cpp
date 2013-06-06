/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#include <algorithm>
#include <cstddef>
#include <cinamo/String.h>
#include <donut/source/Source.h>
#include <donut/source/Closure.h>
#include <donut/Exception.h>
#include <donut/object/Heap.h>

namespace donut {

Source::Source(int id)
:id_(id),entrypoint_id_(-1)
{

}

Source::Source(XValue const& data)
{
	Handler<XObject> xobj(data.as<XObject>());
	this->entrypoint_id_ = xobj->get<int>("entrypoint_id");
	this->id_ = xobj->get<int>("id");
	this->intTable_.load( xobj->get<XValue>("intTable") );
	this->floatTable_.load( xobj->get<XValue>("floatTable") );
	this->stringTable_.load( xobj->get<XValue>("stringTable") );
	this->closureTable_.load( xobj->get<XValue>("closureTable") );
}

void Source::onRegisterToHeap(Heap* const& heap, int const& id) noexcept
{
	this->id_ = id;
	this->heap_ = HandlerW<Heap>(heap);
}
bool Source::onFree() noexcept
{
	if (Handler<Heap> heap = this->heap_.lock()) {
		heap->unregisterSource(this);
	}
	return false;
}

template<>
void ConstTable<Handler<Closure> >::load(XValue const& data)
{
	for(XValue& v : *(data.as<XArray>())){
		this->table_.push_back(Handler<Closure>(new Closure(v)));
	}
}

template<>
XValue ConstTable<Handler<Closure> >::save()
{
	Handler<XArray> top(new XArray);
	for(Handler<Closure>& clos : this->table_){
		top->append(clos->save());
	}
	return top;
}

template<>
bool ConstTable<Handler<Closure> >::operator==(ConstTable<Handler<Closure> > const& other) const noexcept
{
	struct Unref : public std::binary_function<Handler<Closure>&, Handler<Closure>&, bool>{
		bool operator()(Handler<Closure> const& p, Handler<Closure> const& q){
			return *p == *q;
		}
	};
	return table_.size() == other.table_.size() &&
		std::equal(this->table_.begin(), this->table_.end(), other.table_.begin(), Unref() );
}


XValue Source::save()
{
	Handler<XObject> top(new XObject);
	top->set("id", this->id_);
	top->set("entrypoint_id", this->entrypoint_id_);
	top->set("intTable", this->intTable_.save());
	top->set("floatTable", this->floatTable_.save());
	top->set("stringTable", this->stringTable_.save());
	top->set("closureTable", this->closureTable_.save());
	return top;
}

std::string Source::disasm( Instruction inst )
{
	Instruction opcode;
	Instruction constKind;
	int constIndex;
	Source::disasm(inst, opcode, constKind, constIndex);

	std::string repl=format("#%08x ", inst);

	switch(opcode) {
	case Nop:
		repl += "Nop";
		break;
	case Push:
		repl += "Push";
		break;
	case PushCopy:
		repl += "PushCopy";
		break;
	case ReplaceCopy:
		repl += "ReplaceCopy";
		break;
	case Pop:
		repl += "Pop";
		break;
	case SearchScope:
		repl += "SearchScope";
		break;
	case StoreObj:
		repl += "StoreObj";
		break;
	case LoadObj:
		repl += "LoadObj";
		break;
	case Apply:
		repl += "Apply";
		break;
	case ConstructArray:
		repl += "ConstructArray";
		break;
	case ConstructObject:
		repl += "ConstructObject";
		break;
	case Branch:
		repl += "Branch";
		break;
	case BranchTrue:
		repl += "BranchTrue";
		break;
	case BranchFalse:
		repl += "BranchFalse";
		break;
	case PushSelf:
		repl += "PushSelf";
		break;
	case Interrupt:
		repl += "Interrupt";
		break;
	case Return:
		repl += "Return";
		break;
	default:
		repl += "????????";
		break;
	}

	while(repl.size() < 26){
		repl += " ";
	}

	switch(constKind) {
	case Inst::ConstOperand:
		repl += format("%d", constIndex);
		break;
	case Inst::ConstInt:
		repl += format("int:%d -> %d", constIndex, this->intTable_.get(constIndex));
		break;
	case Inst::ConstBool:
		repl += format("bool:%d -> %s", constIndex, constIndex == 0 ? "false" : "true");
		break;
	case Inst::ConstFloat:
		repl += format("float:%d -> %f", constIndex, this->floatTable_.get(constIndex));
		break;
	case Inst::ConstClosure:
		repl += format("closure:%d -> %d", constIndex, constIndex); //TODO
		break;
	case Inst::ConstString:
		repl += format("string:%d -> %s", constIndex, this->stringTable_.get(constIndex).c_str());
		break;
	case Inst::ConstNull:
		repl += format("null");
		break;
	default:
		repl += "????????";
		break;
	}
	return repl;
}


template<>
Instruction Source::constCode<bool>(bool const& val)
{
	return (Inst::ConstBool) | (val ? 1 : 0);
}

template<>
Instruction Source::constCode<int>(int const& val)
{
	return (Inst::ConstInt) | this->intTable_.regist(val);
}

template<>
Instruction Source::constCode<float>(float const& val)
{
	return (Inst::ConstFloat) | this->floatTable_.regist(val);
}

template<>
Instruction Source::constCode<Handler<donut::Closure> >(Handler<donut::Closure> const& val)
{
	return (Inst::ConstClosure) | this->closureTable_.regist(val);
}

template<>
Instruction Source::constCode<std::nullptr_t>(std::nullptr_t const& null)
{
	return (Inst::ConstNull);
}

template<>
Instruction Source::constCode<std::string>(std::string const& val)
{
	return (Inst::ConstString) | this->stringTable_.regist(val);
}

}
