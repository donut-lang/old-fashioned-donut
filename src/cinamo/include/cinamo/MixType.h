/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2013, PSI
 */

#pragma once
#include <memory>
namespace cinamo {

/**
 * LかつRであるという型の制約を表す
 */
template <typename L, typename R>
class AND final {
private:
	std::shared_ptr<L> spiritL_;
	std::shared_ptr<R> spiritR_;
public:
	template <typename Origin>
	AND(Origin* orig) {
		std::shared_ptr<Origin> x(orig);
		this->spiritL_ = x;
		this->spiritR_ = x;
	};
	template <typename Origin>
	AND(std::shared_ptr<Origin> orig):spiritL_(orig), spiritR_(orig){}

public:
	std::shared_ptr<L> asLeft(){
		return this->spiritL_;
	}
	std::shared_ptr<R> asRight(){
		return this->spiritR_;
	}
};

}
