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

#include <tarte/XArchiver.h>
#include <tarte/String.h>

namespace tarte {

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

XArchiver& XArchiver::binary(char* const& bin, std::size_t const& len)
{
	if(decode_now_){
		if(!bin){
			TARTE_EXCEPTION(Exception, "[BUG] Oops. Binary Area is not allocated.");
		}
		XBinary const& b(array_->get<XBinary>(this->array_index_++));
		if(len != b.size()){
			TARTE_EXCEPTION(Exception, "[BUG] Binary size does not match. requested: %d archived: %d", len, b.size());
		}
		std::copy(b.begin(), b.end(), bin);
	}else{
		array_->append( XValue(bin, len) );
	}
	return *this;
}

}
