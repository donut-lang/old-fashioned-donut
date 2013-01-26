Chisa
=====
現在このリポジトリには以下の３つが同居しております。

ど～なっつ
----
* src/donutに配置
* http://donut-lang.org/

時を司るスクリプト言語です。C++ + Antlr3 + ICU(for unicode)

ちさ
----
* src/chisaに配置
* http://donut-lang.org/chisa
* http://ledyba.org/famicom

ど～なっつをバックエンドに使ったGUIツールキット。OpenGL + C++製。Androidでも動きます。

タルト
---
* src/tarteに配置
* もちろんC++

ど～なっつとちさの共通ライブラリです。([Saccubus](http://saccubus.sourceforge.jp/)もこれ使えるように出来ないかなぁ。

How to build
====
WindowsでもLinuxでも

    sh bld.sh

WindowsだとboostでC++11の標準ライブラリを代用してるのでインストールしておいて下さい



