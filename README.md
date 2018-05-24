# Donut

現在このリポジトリには以下が同居しております。

## Donut - a programming language for time leapers

 - src/donutに配置
 - http://donut-lang.org/

"時間遡行者のための"プログラミング言語です。C++ + Antlr3 + ICU(for unicode)

## Chisa - a GUI toolkit for time leapers.

 - src/chisaに配置
 - https://donut-lang.org/chisa

ど～なっつをバックエンドに使ったGUIツールキット。OpenGL + C++製。AndroidとWin32にも対応してたけど、今は謎。

## バイナリから目覚めるぼくらのファミリーコンピュータ！

 - universe/workshop/**** 以下
 - https://7io.org/famicom

以上を利用して作られた「分からなかった所まで遡れる」情報教材です。

### How to build

```bash
echo 'Bake donut as you like: '

cmake -G 'Unix Makefiles'
cmake -G 'Ninja'
cmake -G 'Xcode'
cmake -G 'Eclipse CDT4 - Ninja'
cmake -G 'Eclipse CDT4 - Unix Makefiles'
```

### License
Donut and Chisa are dual licensed under GPL v3 (or any later) or 2-clause BSD license. You can chose one (or both) of them.


# Powerd by

IPAの[未踏IT人材発掘・育成事業](http://www.ipa.go.jp/jinzai/mitou/)に採択された[「CPUの理解を容易にするシステムと解説サイトの構築」](http://www.ipa.go.jp/jinzai/mitou/2012/2012_1/gaiyou/h-1.html)の一部分として開発され、支援を受けていました。