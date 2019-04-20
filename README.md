<h1 align="center">
  <br />
  🎠
  <br />
  Merry Go Round
  <sup>
    <br />
    <br />
  </sup>
</h1>

<div align="center">
  <img src="https://img.shields.io/github/last-commit/async3619/merry-go-round.svg?style=flat-square" alt="MIT License" />
  <img src="https://img.shields.io/github/package-json/v/async3619/merry-go-round.svg?style=flat-square" alt="Version" />
  <img src="https://img.shields.io/github/license/async3619/merry-go-round.svg?style=flat-square" alt="MIT License" />
  <br />
  <sup>Node.js native module for handling media files</sup>
  <br />
  <br />
</div>

## Introduction

<img align="right" src="https://raw.githubusercontent.com/async3619/merry-go-round/master/docs/merry-go-round.gif" />

This is a node.js native module to handle information of media files. as you already know, most of media files out there have their
own tag system such as ID3, APE and so on. this module is developed for this purpose! 

We already got like a ton of amazing javascript modules for same purpose. like <a href="https://github.com/borewit/music-metadata">music-metadata</a>, <a href="https://github.com/Zazama/node-id3">node-id3</a>. I thought it'll be just reinventing the wheel but their modules are having a tiny issues that I was quite disappointed about. (and there was educational purpose too) so I started to develop this module and I gladly hope you can develop your own great applications with this module.

This is what I've disappointed about already existing modules:

- Poor performance: This is pretty ridiculous to hope a node.js module run fast even if it written in pure javascript but I think we should improve it as can as possible. so I chose **native** module which uses C/C++ codes to run faster. but there is some trade-off like `native module will be platform-dependent` but I'm developing this module as cross-platform compliant. so don't worry about it.

- Lack of typings: Yes, typings. recently I've seen statistics that show that many of developers are using typescript as their primary development language. therefore, it has became important to provide typing information to improve their developing experience while using this module.

- Not maintained anymore: It's not easy to find alternative modules since most of tagging module of node.js out there are not maintained. so I've started to develop this one. as alternative of that modules.


## Prerequisite

This project uses `CMake` instead of `node-gyp`  to build codes so you have to install:

- CMake 3.14+

The code base of this project is C++17 compliant, so you need to have newer C++ compiler than other native addons.

- g++ 8+

or if you're on Windows...

- Microsoft Visual Studio 2017+ (see [windows-build-tools](https://www.npmjs.com/package/windows-build-tools))


## Installation

currently, I have not published this module on NPM so you have to clone this project directly. and I really want to suggest you to use [Yarn](https://github.com/yarnpkg/yarn) instead of npm:

```bash
$ git clone https://github.com/async3619/merry-go-round
$ cd merry-go-round
$ yarn
```

or as a dependency:

```bash
$ yarn add https://github.com/async3619/merry-go-round
```
