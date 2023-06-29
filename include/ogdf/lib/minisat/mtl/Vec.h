/*******************************************************************************************[Vec.h]
Copyright (c) 2003-2007, Niklas Een, Niklas Sorensson
Copyright (c) 2007-2010, Niklas Sorensson

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************/

#pragma once

#include <ogdf/lib/minisat/mtl/IntTypes.h>
#include <ogdf/lib/minisat/mtl/XAlloc.h>

#include <new>
#include <utility>

#include <assert.h>

namespace Minisat {
namespace Internal {

//=================================================================================================
// Automatically resizable arrays

template<class T>
class vec {
	T* data;
	int sz;
	int cap;

	// Don't allow copying (error prone):
	vec<T>& operator=(vec<T>& other) {
		assert(0);
		return *this;
	}

	vec(vec<T>& other) { assert(0); }

	// Helpers for calculating next capacity:
	static inline int imax(int x, int y) {
		int mask = (y - x) >> (sizeof(int) * 8 - 1);
		return (x & mask) + (y & (~mask));
	}

	//static inline void nextCap(int& cap){ cap += ((cap >> 1) + 2) & ~1; }
	static inline void nextCap(int& cap) { cap += ((cap >> 1) + 2) & ~1; }

	void copyFrom(const vec<T>& other) {
		capacity(other.sz);
		for (int i = 0; i < other.sz; ++i) {
			new (&data[i]) T(other.data[i]);
		}
		sz = other.sz;
	}

public:
	// Constructors:
	vec() : data(nullptr), sz(0), cap(0) { }

	vec(const vec<T>& other) : data(nullptr), sz(0), cap(0) { copyFrom(other); }

	explicit vec(int size) : data(nullptr), sz(0), cap(0) { growTo(size); }

	vec(int size, const T& pad) : data(nullptr), sz(0), cap(0) { growTo(size, pad); }

	~vec() { clear(true); }

	// Pointer to first element:
	operator T*(void) { return data; }

	// Assignment operator:
	vec<T>& operator=(const vec<T>& other) {
		if (this != &other) {
			clear();
			copyFrom(other);
		}
		return *this;
	}

	// Size operations:
	int size(void) const { return sz; }

	void shrink(int nelems) {
		assert(nelems <= sz);
		for (int i = 0; i < nelems; i++) {
			sz--, data[sz].~T();
		}
	}

	void shrink_(int nelems) {
		assert(nelems <= sz);
		sz -= nelems;
	}

	int capacity(void) const { return cap; }

	void capacity(int min_cap);
	void growTo(int size);
	void growTo(int size, const T& pad);
	void clear(bool dealloc = false);

	// Stack interface:
	void push(void) {
		if (sz == cap) {
			capacity(sz + 1);
		}
		new (&data[sz]) T();
		sz++;
	}

	void push(const T& elem) {
		if (sz == cap) {
			capacity(sz + 1);
		}
		data[sz++] = elem;
	}

	void push_(const T& elem) {
		assert(sz < cap);
		data[sz++] = elem;
	}

	void pop(void) {
		assert(sz > 0);
		sz--, data[sz].~T();
	}

	// NOTE: it seems possible that overflow can happen in the 'sz+1' expression of 'push()', but
	// in fact it can not since it requires that 'cap' is equal to INT_MAX. This in turn can not
	// happen given the way capacities are calculated (below). Essentially, all capacities are
	// even, but INT_MAX is odd.

	const T& last(void) const { return data[sz - 1]; }

	T& last(void) { return data[sz - 1]; }

	// Vector interface:
	const T& operator[](int index) const { return data[index]; }

	T& operator[](int index) { return data[index]; }

	// Duplicatation (preferred instead):
	void copyTo(vec<T>& copy) const {
		copy.clear();
		copy.growTo(sz);
		for (int i = 0; i < sz; i++) {
			copy[i] = data[i];
		}
	}

	void moveTo(vec<T>& dest) {
		dest.clear(true);
		dest.data = data;
		dest.sz = sz;
		dest.cap = cap;
		data = nullptr;
		sz = 0;
		cap = 0;
	}
};

template<class T>
void vec<T>::capacity(int min_cap) {
	if (cap >= min_cap) {
		return;
	}
	int add = imax((min_cap - cap + 1) & ~1, ((cap >> 1) + 2) & ~1); // NOTE: grow by approximately 3/2

	T* new_data = static_cast<T*>(::operator new((cap + add) * sizeof(T)));
	if (new_data == nullptr) {
		throw OutOfMemoryException();
	}

	for (int i = 0; i < cap; ++i) {
		new (&new_data[i]) T(std::move(data[i])); // Move-construct elements to the new memory block
	}

	for (int i = 0; i < cap; ++i) {
		data[i].~T();
	}

	::operator delete(data);
	data = new_data;
	cap += add;
}

template<class T>
void vec<T>::growTo(int size, const T& pad) {
	if (sz >= size) {
		return;
	}
	capacity(size);
	for (int i = sz; i < size; i++) {
		data[i] = pad;
	}
	sz = size;
}

template<class T>
void vec<T>::growTo(int size) {
	if (sz >= size) {
		return;
	}
	capacity(size);
	for (int i = sz; i < size; i++) {
		new (&data[i]) T;
	}
	sz = size;
}

template<class T>
void vec<T>::clear(bool dealloc) {
	if (data != nullptr) {
		for (int i = 0; i < sz; i++) {
			data[i].~T();
		}
		sz = 0;
		if (dealloc) {
			free(data), data = nullptr, cap = 0;
		}
	}
}

//=================================================================================================
} // namespace Internal
} // namespace Minisat
