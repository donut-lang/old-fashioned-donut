/**
 * Tarte
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

#include <tarte/XVal.h>
#include <tarte/String.h>
#include <tarte/Base64.h>
#include <tinyxml2.h>

namespace tarte {

XArray::XArray( tinyxml2::XMLElement* elm )
{
	if(std::string("xarray") != elm->Name()){
		TARTE_EXCEPTION(Exception, "Element is not xarray. type: %s value: %s", elm->Name(), elm->GetText());
	}
	for( tinyxml2::XMLElement* e = elm->FirstChildElement(); e; e = e->NextSiblingElement() ){
		append( XValue::fromXML(e) );
	}
}

tinyxml2::XMLElement* XArray::toXML( tinyxml2::XMLDocument* doc )
{
	tinyxml2::XMLElement* top = doc->NewElement("xarray");
	for(XValue& x : this->list_){
		top->InsertEndChild(x.toXML(doc));
	}
	return top;
}

XObject::XObject( tinyxml2::XMLElement* elm )
{
	accumlate(elm);
}

void XObject::accumlate( tinyxml2::XMLElement* elm )
{
	if(std::string("xobject") != elm->Name()){
		TARTE_EXCEPTION(Exception, "Element is not xobject. type: %s value: %s", elm->Name(), elm->GetText());
	}
	for( tinyxml2::XMLElement* e = elm->FirstChildElement(); e; e = e->NextSiblingElement() ){
		const char* const name = e->Attribute("name");
		if(!name){
			TARTE_EXCEPTION(Exception, "Object element name not specified. type: %s value: %s", e->Name(), e->GetText());
		}
		set(name, XValue::fromXML(e));
	}
}

tinyxml2::XMLElement* XObject::toXML( tinyxml2::XMLDocument* doc )
{
	tinyxml2::XMLElement* top = doc->NewElement("xobject");
	for(std::pair<std::string, XValue>& x : this->map_){
		tinyxml2::XMLElement* e = x.second.toXML(doc);
		e->SetAttribute("name", x.first.c_str());
		top->InsertEndChild(e);
	}
	return top;
}

XValue XValue::fromXML( tinyxml2::XMLElement* elm ) {
	std::string const name(elm->Name());
	if(name=="xnull"){
		return XValue();
	}else if( name == "xarray" ){
		return XValue(Handler<XArray>( new XArray(elm)));
	}else if( name == "xbinary" ){
		return XValue::fromString<Binary>( elm->GetText() );
	}else if(name=="xobject") {
		return XValue(Handler<XObject>( new XObject(elm)) );
	}else if(name=="xstring") {
		return XValue::fromString<String>( elm->GetText() );
	}else if(name=="xuint") {
		return XValue::fromString<UInt>( elm->GetText() );
	}else if(name=="xsint") {
		return XValue::fromString<SInt>( elm->GetText() );
	}else if(name=="xbool") {
		return XValue::fromString<Bool>( elm->GetText() );
	}else if(name=="xfloat") {
		return XValue::fromString<Float>( elm->GetText() );
	}else{
		TARTE_EXCEPTION(Exception, "[BUG] Unknwon type!: %s", name.c_str());
	}
}

tinyxml2::XMLElement* XValue::toXML( tinyxml2::XMLDocument* doc )
{
	switch(type_){
	case NullT:
		return doc->NewElement("xnull");
	case ArrayT:
		return (*this->spirit_.array_)->toXML(doc);
	case ObjectT:
		return (*this->spirit_.object_)->toXML(doc);
	case BinaryT: {
		tinyxml2::XMLElement* elm = doc->NewElement("xbinary");
		elm->InsertEndChild(doc->NewText(base64::encode(*(this->spirit_.binary_)).c_str()));
		return elm;
	}
	case StringT: {
		tinyxml2::XMLElement* elm = doc->NewElement("xstring");
		elm->InsertEndChild(doc->NewText(this->spirit_.str_->c_str()));
		return elm;
	}
	case UIntT: {
		tinyxml2::XMLElement* elm = doc->NewElement("xuint");
		elm->InsertEndChild(doc->NewText(toString(this->spirit_.uint_).c_str()));
		return elm;
	}
	case SIntT: {
		tinyxml2::XMLElement* elm = doc->NewElement("xsint");
		elm->InsertEndChild(doc->NewText(toString(this->spirit_.int_).c_str()));
		return elm;
	}
	case FloatT: {
		tinyxml2::XMLElement* elm = doc->NewElement("xfloat");
		elm->InsertEndChild(doc->NewText(toString(this->spirit_.float_).c_str()));
		return elm;
	}
	case BoolT: {
		tinyxml2::XMLElement* elm = doc->NewElement("xbool");
		elm->InsertEndChild(doc->NewText(toString(this->spirit_.bool_).c_str()));
		return elm;
	}
	default:
		TARTE_EXCEPTION(Exception, "[BUG] Unknwon type!: %d", type_);
	}
}

std::string XValue::typeString() const noexcept {
	switch ( this->type_ ){
	case Type::NullT:
		return "XNull";
	case Type::ArrayT:
		return "XArray";
	case Type::BinaryT:
		return "XBinary";
	case Type::ObjectT:
		return "XObject";
	case Type::StringT:
		return "XString";
	case Type::UIntT:
		return "XUInt";
	case Type::SIntT:
		return "XSInt";
	case Type::BoolT:
		return "XBool";
	case Type::FloatT:
		return "XFloat";
	default:
		TARTE_EXCEPTION(Exception, "[BUG] Unknwon type!: %d", type_);
	}
}

void XValue::remove()
{
	switch(this->type_){
	case Type::ArrayT:
		delete this->spirit_.array_;
		this->spirit_.array_ = nullptr;
		break;
	case Type::ObjectT:
		delete this->spirit_.object_;
		this->spirit_.object_ = nullptr;
		break;
	case Type::BinaryT:
		delete this->spirit_.binary_;
		this->spirit_.binary_ = nullptr;
		break;
	case Type::StringT:
		delete this->spirit_.str_;
		this->spirit_.str_ = nullptr;
		break;
	default:
		break;
	}
}
void XValue::grab(XValue&& o)
{
	o.type_ = NullT;
	switch(type_){
	case NullT:
		break;
	case ArrayT:
		this->spirit_.array_ = o.spirit_.array_;
		o.spirit_.array_ = nullptr;
		break;
	case ObjectT:
		this->spirit_.object_ = o.spirit_.object_;
		o.spirit_.object_ = nullptr;
		break;
	case BinaryT:
		this->spirit_.binary_ = o.spirit_.binary_;
		o.spirit_.binary_ = nullptr;
		break;
	case StringT:
		this->spirit_.str_ = o.spirit_.str_;
		o.spirit_.str_ = nullptr;
		break;
	case UIntT:
		this->spirit_.uint_ = o.spirit_.uint_;
		break;
	case SIntT:
		this->spirit_.int_ = o.spirit_.int_;
		break;
	case FloatT:
		this->spirit_.float_ = o.spirit_.float_;
		break;
	case BoolT:
		this->spirit_.bool_ = o.spirit_.bool_;
		break;
	}
}
void XValue::copy(XValue const& o)
{
	switch(type_){
	case NullT:
		break;
	case ArrayT:
		this->spirit_.array_ = new Handler<Array>(*o.spirit_.array_);
		break;
	case ObjectT:
		this->spirit_.object_ = new Handler<Object>(*o.spirit_.object_);
		break;
	case BinaryT:
		this->spirit_.binary_ = new Binary(*o.spirit_.binary_);
		break;
	case StringT:
		this->spirit_.str_ = new std::string(*o.spirit_.str_);
		break;
	case UIntT:
		this->spirit_.uint_ = o.spirit_.uint_;
		break;
	case SIntT:
		this->spirit_.int_ = o.spirit_.int_;
		break;
	case FloatT:
		this->spirit_.float_ = o.spirit_.float_;
		break;
	case BoolT:
		this->spirit_.bool_ = o.spirit_.bool_;
		break;
	}
}

void XValue::swap( XValue& o )
{
	using std::swap;
	swap(o.type_, type_);
	swap(o.spirit_, spirit_);
}

template<> XValue& XObject::set<XValue>(std::string const& name, XValue const& obj)
{
	auto it = std::lower_bound( this->map_.begin(), this->map_.end(), name, Comparator() );
	std::pair<std::string, XValue>& p = *it;
	if( it == this->map_.end() || p.first != name) {
		it = this->map_.insert(it, std::pair<std::string, XValue>(name, obj));
		return it->second;
	}else{
		return p.second = obj;
	}
}
template <> XValue XValue::fromString<XValue::UInt>(std::string const& str)
{
	bool success = false;
	XValue::UInt val = parseAs<XValue::UInt>(str, 0, &success);
	if(!success){
		TARTE_EXCEPTION(Exception, "Failed to convert %s to UInt.", str.c_str());
	}
	return XValue((UInt)val);
}
template <> XValue XValue::fromString<XValue::SInt>(std::string const& str)
{
	bool success = false;
	XValue::SInt val = parseAs<XValue::SInt>(str, 0, &success);
	if(!success){
		TARTE_EXCEPTION(Exception, "Failed to convert %s to SInt.", str.c_str());
	}
	return XValue((SInt)val);
}
template <> XValue XValue::fromString<XValue::Float>(std::string const& str)
{
	bool success = false;
	XValue::Float val = parseAs<XValue::Float>(str, &success);
	if(!success){
		TARTE_EXCEPTION(Exception, "Failed to convert %s to Double.", str.c_str());
	}
	return XValue((Float)val);
}
template <> XValue XValue::fromString<XValue::Bool>(std::string const& str)
{
	bool success = false;
	XValue::Bool val = parseAs<bool>(str, &success);
	if(!success){
		TARTE_EXCEPTION(Exception, "Failed to convert %s to bool", str.c_str());
	}
	return XValue((Bool)val);
}
template <> XValue XValue::fromString<XValue::Binary>(std::string const& str)
{
	return XValue( base64::decode(str) );
}

}
