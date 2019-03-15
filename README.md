About CTL-Vector
==============

CTL-Vector is an implementation of STL-compatible vector container.
It has own iterator and allocator, but you are free to change that.

Written in Native-С++

Installation
------------

Download project as '.zip' archive and extract it to whatever you like directory or use `git`:
```sh
$ git clone https://github.com/abelidze/ctl-vector.git
```

Running
-------

> **TESTS**

```sh
make test && make clean && ./bin/vector
```

> **COVERAGE**

```sh
make cov && make clean && ./bin/vector
```

> **BENCHMARKS**

```sh
make bench && make clean && ./bin/vector
```

* Tested with MinGW x64 and its `make` utility, but you can use any working C / C++ compiler.

Contact
-------

Developers on Telegram:

[![https://telegram.me/Abelidze](https://img.shields.io/badge/%E2%9E%A4Telegram-@Abelidze-DD2200.svg?style=flat-square&&colorA=2D233B)](https://telegram.me/Abelidze)

License
-------
CTL-Vector is open-sourced software licensed under the [MIT License](http://opensource.org/licenses/MIT).

#### Disclaimer

This program is provided "AS IS" without warranty of any kind.