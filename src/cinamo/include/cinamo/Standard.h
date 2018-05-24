/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#pragma once

#include <cstdio>
#include "Handler.h"
#include "functional/Either.h"

namespace cinamo {
class File final : public HandlerBody<File> {
private:
	FILE* f_;
public:
	File(FILE* f_):f_(f_) {
	}
	bool onFree(){
		fclose(f_);
		this->f_ = nullptr;
		return false;
	}
public:
	template <typename T>
	Either<std::string, std::vector<T> > read(size_t cnt) {
		std::vector<T> vec;
		vec.resize(cnt);
		return cnt == fread(vec.data(), sizeof(T), cnt, f_) ?
				Right<std::string, std::vector<T> >(vec) :
				Left<std::string, std::vector<T> >("failed to exec fread");
	}
	Either<std::string, Handler<File> > seek(off64_t const& off, int seek_op) {
		return fseeko64(f_, off, seek_op) == 0 ?
				Right<std::string, Handler<File> >(this->self()):
				Left<std::string, Handler<File> >("Failed to exec feeko64");
	}
	Either<std::string, Handler<File> > seekFromBegin(off64_t const& off) {
		return this->seek(off, SEEK_SET);
	}
public:
	static Either<std::string, Handler<File> > open(std::string const& fname, std::string const& mode){
		FILE* f = fopen(fname.c_str(), mode.c_str());
		return f ?
				Right<std::string, Handler<File> >(Handler<File>(new File(f))) :
				Left<std::string, Handler<File> >("Failed to exec fopen");
	}
	static Either<std::string, Handler<File> > openInRB(std::string const& fname){
		return open(fname, "rb");
	}
	static Either<std::string, Handler<File> > openInR(std::string const& fname){
		return open(fname, "r");
	}
	static Either<std::string, Handler<File> > openInWB(std::string const& fname){
		return open(fname, "wb");
	}
	static Either<std::string, Handler<File> > openInW(std::string const& fname){
		return open(fname, "w");
	}
};
}
