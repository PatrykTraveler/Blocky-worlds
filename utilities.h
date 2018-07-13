#pragma once
template<class T> inline constexpr T operator| (T a, T b) { return (T)((int)a | (int)b); }
template<class T> inline T& operator|= (T& a, T b) { return (T&)((int&)a |= (int)b); }
template <class T> inline T min(T a, T b) { return a < b ? a : b; }
template <class T> inline T max(T a, T b) { return a > b ? a : b; }
