#pragma once

#include <type_traits>

#define SP_CAST(T) static_cast<T&>(this)

// ※静的ポリモーフィズムを実装するときは、派生元のクラスでSP_CASTが使われた関数を必ず実装する
