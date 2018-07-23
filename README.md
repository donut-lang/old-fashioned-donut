# Donut - a programming language for time leapers

 - src/donutに配置
 - http://donut-lang.org/

"時間遡行者のための"プログラミング言語です。C++ + Antlr3 + ICU(for unicode)

# How to build

```bash
echo 'Bake donut as you like: '

cmake -G 'Unix Makefiles'
cmake -G 'Ninja'
cmake -G 'Xcode'
cmake -G 'Eclipse CDT4 - Ninja'
cmake -G 'Eclipse CDT4 - Unix Makefiles'
```

```bash
echo 'build types:'
cmake . -DCMAKE_BUILD_TYPE=Release
cmake . -DCMAKE_BUILD_TYPE=RelWithDebInfo

cmake . -DCMAKE_BUILD_TYPE=Debug
cmake . -DCMAKE_BUILD_TYPE=MinSizeRel
```

```bash
echo 'to clean up:'
bash scripts/clean.sh
```

# Powerd by

IPAの[未踏IT人材発掘・育成事業](http://www.ipa.go.jp/jinzai/mitou/)に採択された[「CPUの理解を容易にするシステムと解説サイトの構築」](http://www.ipa.go.jp/jinzai/mitou/2012/2012_1/gaiyou/h-1.html)の一部分として開発され、支援を受けていました。
