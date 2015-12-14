/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "Task.hpp"

namespace chisa {
namespace tk {

static std::string const TAG("TaskHandler");

TaskHandler::TaskHandler(Logger& log)
:log(log)
{

}

void TaskHandler::send( std::function<bool()> const& lambda ){
	if(this->log.t()){
		this->log.t(TAG, "Lambda std::function<bool()> registered.");
	}
	this->lambas_.push_back(std::function<bool(float)>( [lambda](float n)->bool{ return lambda(); } ));
}
void TaskHandler::send( std::function<void()> const& lambda ){
	if(this->log.t()){
		this->log.t(TAG, "Lambda std::function<void()> registered.");
	}
	this->lambas_.push_back(std::function<bool(float)>( [lambda](float n)->bool{ lambda();return false; } ));
}
void TaskHandler::send( std::function<bool(float)> const& lambda ){
	if(this->log.t()){
		this->log.t(TAG, "Lambda std::function<bool(float)> registered.");
	}
	this->lambas_.push_back(lambda);
}

void TaskHandler::run(const float delta_ms)
{
	for( auto it = lambas_.begin(); it != lambas_.end(); ) {
		std::function<bool(float)> const& f = *it;
		bool const result = f(delta_ms);
		if(!result) {
			it = this->lambas_.erase(it);
		}else{
			++it;
		}
	}
}

}}
