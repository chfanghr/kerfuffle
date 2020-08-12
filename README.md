# Kerfuffle: High-Performance Hierarchical FSM

![Windows](https://github.com/chfanghr/kerfuffle/workflows/Windows/badge.svg)
![Linux](https://github.com/chfanghr/kerfuffle/workflows/Linux/badge.svg)

Header-only heriarchical FSM framework in C++11, 
with fully statically-defined structure 
(no dynamic allocations), built with variadic templates.

## Features
* Written in widely-supported modern(ish) C++11
* Header-only
* Convenient, minimal boilerplate
* Fully static, no dynamic allocations
* Uses inline-friendly compile-time pylymorphism, no virtual methods were harmed
* Type-safe transitions
* Hierarchical, with a selection of composite(sub-machine) and orthogonal regions
* Gamedev-friendly, supports explicit `State::update()`
* Also supports traditional event-based workflow with `State::react()`
* Scaleable, supports robust state re-use via state injections
* Built-in logging support
* Built-in serialization support, with activity and transition history support
* Permissive [MIT License](LICENSE)