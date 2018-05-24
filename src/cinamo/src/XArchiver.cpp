/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include <cinamo/XArchiver.h>
#include <cinamo/String.h>

namespace cinamo {

XArchiver::XArchiver(bool decode_now)
:decode_now_(decode_now)
,array_(nullptr)
,array_index_(0)
{

}

XArchiver::XArchiver(Handler<XArray> const& a)
:decode_now_(true)
,array_(a)
,array_index_(0)
{
}


XArchiverIn::XArchiverIn()
:XArchiver(true)
{
	this->array(Handler<XArray>());
	this->index(0);
}

XArchiverIn::XArchiverIn(XValue const& v)
:XArchiver(true)
{
	this->array(v.as<XArray>());
	this->index(0);
}

XArchiverOut::XArchiverOut()
:XArchiver(false)
{
	this->array(Handler<XArray>(new XArray));
	this->index(0);
}

XArchiver& XArchiver::binary(char* const bin, std::size_t const& len)
{
	if(decode_now_){
		if(!bin){
			CINAMO_EXCEPTION(Exception, "[BUG] Oops. Binary Area is not allocated.");
		}
		XBinary const& b(array_->get<XBinary>(this->array_index_++));
		if(len != b.size()){
			CINAMO_EXCEPTION(Exception, "[BUG] Binary size does not match. requested: %d archived: %d", len, b.size());
		}
		std::copy(b.begin(), b.end(), bin);
	}else{
		array_->append( XValue(bin, len) );
	}
	return *this;
}

}
