#pragma once

#include <type_traits>

#define SP_CAST(t) static_cast<##t&>(this)

// ※静的ポリモーフィズムを実装するときは、派生元のクラスでSP_CASTが使われた関数を必ず実装する
