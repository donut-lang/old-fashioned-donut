/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <vector>
#include <cinamo/Handler.h>
#include <cinamo/Platform.h>
#include <cinamo/ClassUtil.h>
#include <cinamo/Thread.h>
#include "SoundSpec.h"
#include "Instrument.h"

namespace chisa {
using namespace cinamo;

class Quartet : public HandlerBody<Quartet> {
public:
	struct Player{
		DEFAULT_COPY_AND_ASSIGN(Player);
		Player() = delete;
		Player(Handler<Instrument> const& inst, std::size_t buflen);
		~Player() = default;
		Handler<Instrument> instrument;
		std::vector<unsigned char> buffer;
	};
private:
	std::vector<Player> players_;
	SoundSpec desiredSpec_;
	SoundSpec realSpec_;
	std::atomic<bool> startRequested_;
	std::atomic<bool> stared_;
public:
	Quartet(SoundSpec const& desired);
	virtual ~Quartet() noexcept = default;
	inline bool onFree() const noexcept { return false; };
public:
	inline SoundSpec const& spec() const noexcept { return this->realSpec_; };
	/******************************************************************************************************************
	 * 外側からのインターフェイス
	 ******************************************************************************************************************/
public:
	class Lock {
		STACK_OBJECT(Lock);
		DISABLE_COPY_AND_ASSIGN(Lock);
		Quartet& self_;
	public:
		inline Lock(Handler<Quartet> const& q) noexcept:self_(*q.get()) { self_.lock(); }
		inline Lock(Quartet& q) noexcept:self_(q) { self_.lock(); }
		inline ~Lock() noexcept { self_.unlock(); }
	};
public:
	bool start();
	bool stop();
	void onPlay(unsigned char* stream, int len);
public:
	bool addInstrument(Handler<Instrument> const& inst);
	bool hasInstrument(Handler<Instrument> const& inst);
	bool removeInstrument(Handler<Instrument> const& inst);
	/******************************************************************************************************************
	 * 実装用
	 ******************************************************************************************************************/
protected:
	inline std::vector<Player> const& players() const noexcept { return this->players_; };
	inline std::vector<Player>& players() noexcept { return this->players_; };
	void notifySoundSpec(SoundSpec::DataFormat format, unsigned int channels, unsigned int frequency, unsigned int samples);
	void notifySoundSpec(SoundSpec const& spec);
protected:
	virtual bool startImpl() = 0;
	virtual bool stopImpl() = 0;
	virtual void playImpl(unsigned char* stream, int len) = 0;
	virtual bool lockImpl() noexcept = 0;
	virtual bool unlockImpl() noexcept = 0;
	/******************************************************************************************************************
	 * 内部
	 ******************************************************************************************************************/
private:
	void updateBufferSize();
	bool lock() noexcept;
	bool unlock() noexcept;
	bool startInner();
	bool stopInner();
};


// 実装用
/*
class MyQuartet : public Quartet {
	MyQuartet(SoundSpec const& desired);
	virtual ~MyQuartet() noexcept = default;
private:
	virtual bool startImpl() override final;
	virtual bool stopImpl() override final;
	virtual void playImpl(unsigned char* stream, int len) override final;
	virtual bool lockImpl() noexcept override final;
	virtual bool unlockImpl() noexcept override final;
};
*/

}
