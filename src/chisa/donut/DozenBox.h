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

#pragma once
#include "../util/ClassUtil.h"
#include "../Handler.h"
#include "Donut.h"

namespace chisa {
namespace donut {

/**
 * ドーナツを複数実行するためにまとめ上げるクラス。
 */
class DozenBox : public HandlerBody<DozenBox> {
	DEFINE_MEMBER_REF(public, logging::Logger, log);
private: /* ドーナツの実体 */
	Handler<Donut> mainDonut_; //メイン
	std::map<std::string, Handler<Donut> > combos_; //サブとしてのコンボ・メニュー
public: /* 生成・破棄 */
	DozenBox(logging::Logger& log);
	virtual ~DozenBox() noexcept = default;
	inline bool onFree() const noexcept{ return false; };
private: /* メインドーナツの実行 */
	Handler<Object> execute( const std::string& src );
	Handler<Heap> heap() const noexcept;
public: /* コンボでの実行 */
	Handler<Object> startCombo(std::string const& comboname, std::string const& src);
	Handler<Object> continueCombo(std::string const& comboname, Handler<Object> const& obj);
};

}}
