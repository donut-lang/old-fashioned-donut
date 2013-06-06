/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <cinamo/ClassUtil.h>
#include <cinamo/VectorMap.h>
#include <cinamo/Handler.h>
#include "Donut.h"

namespace donut {
using namespace cinamo;

/**
 * ドーナツを複数実行するためにまとめ上げるクラス。
 */
class DozenBox : public HandlerBody<DozenBox> {
	DEFINE_MEMBER_REF(public, Logger, log);
private: /* ドーナツの実体 */
	Handler<Donut> mainDonut_; //メイン
	VectorMap<std::string, Handler<Donut> > combos_; //サブとしてのコンボ・メニュー
public: /* 生成・破棄 */
	DozenBox(Logger& log);
	virtual ~DozenBox() noexcept = default;
	inline bool onFree() const noexcept{ return false; };
private: /* メインドーナツの実行 */
	Handler<Object> execute( std::string const& src );
	Handler<Heap> heap() const noexcept;
public: /* コンボでの実行 */
	Handler<Object> startCombo(std::string const& comboname, std::string const& src);
	Handler<Object> continueCombo(std::string const& comboname, Handler<Object> const& obj);
};

}
