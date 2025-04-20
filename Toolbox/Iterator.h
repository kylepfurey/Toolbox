// .h
// Iterator Interfaces
// by Kyle Furey

#pragma once

/** A collection of useful template types in C++. */
namespace Toolbox {

	// ITERATORS

	/**
	 * A wrapper around a single element in a collection with operators that enable forward traversal.<br/>
	 * This is an interface that iterators can inherit from to guarantee iteration with that type when implemented.<br/>
	 * Only forward iteration is supported with this iterator. Backwards iteration is supported through Iterator2.
	 */
	template<typename DerivedIterator, typename Type>
	struct Iterator {

		// DESTRUCTOR

		/** Virtual destructor. */
		virtual ~Iterator() = default;


		// INTERFACE

		/** Returns a reference to the underlying element of this iterator. */
		virtual Type& operator*() = 0;

		/** Returns a constant reference to the underlying element of this iterator. */
		virtual const Type& operator*() const = 0;

		/** Increments this iterator to the next element. */
		virtual DerivedIterator& operator++() = 0;

		/** Returns whether the given iterators are equal. */
		virtual bool operator==(const Iterator& Other) const {
			return &(*(*this)) == &(*Other);
		}

		/** Returns whether this iterator is valid. */
		virtual explicit operator bool() const {
			return true;
		}


		// ITERATOR

		/** Returns a dereferenced pointer to the underlying element of this iterator. */
		Type* operator->() {
			return &(*(*this));
		}

		/** Returns a dereferenced constant pointer to the underlying element of this iterator. */
		const Type* operator->() const {
			return &(*(*this));
		}

		/** Increments this iterator to the next iterator and returns a copy of the previous iterator. */
		DerivedIterator operator++(int) {
			DerivedIterator Copy = *this;
			++(*this);
			return Copy;
		}

		/** Returns whether the given iterators are not equal. */
		bool operator!=(const Iterator& Other) const {
			return !(*this == Other);
		}

		/** Returns whether this iterator is not valid. */
		bool operator!() const {
			if (*this) {
				return false;
			}
			return true;
		}
	};

	/**
	 * A wrapper around a single element in a collection with operators that enable forward and backward traversal.<br/>
	 * This is an interface that iterators can inherit from to guarantee iteration with that type when implemented.<br/>
	 * A pointer is an excellent example of a two-way iterator.
	 */
	template<typename DerivedIterator, typename Type>
	struct Iterator2 : public Iterator<DerivedIterator, Type> {

		// INTERFACE

		/** Decrements this iterator to the previous element. */
		virtual DerivedIterator& operator--() = 0;


		// ITERATOR

		/** Decrements this iterator to the previous iterator and returns a copy of the next iterator. */
		DerivedIterator operator--(int) {
			DerivedIterator Copy = *this;
			--(*this);
			return Copy;
		}

		/** Returns a copy of this iterator after incrementing the given count. */
		DerivedIterator operator+(const ptrdiff_t Count) const {
			DerivedIterator Copy = *this;
			Copy += Count;
			return Copy;
		}

		/** Returns a reference to this iterator after incrementing the given count. */
		DerivedIterator& operator+=(ptrdiff_t Count) {
			while (Count > 0) {
				++(*this);
				--Count;
			}
			while (Count < 0) {
				--(*this);
				++Count;
			}
			return *this;
		}

		/** Returns a copy of this iterator after decrementing the given count. */
		DerivedIterator operator-(const ptrdiff_t Count) const {
			DerivedIterator Copy = *this;
			Copy -= Count;
			return Copy;
		}

		/** Returns a reference to this iterator after decrementing the given count. */
		DerivedIterator& operator-=(ptrdiff_t Count) {
			while (Count > 0) {
				--(*this);
				--Count;
			}
			while (Count < 0) {
				++(*this);
				++Count;
			}
			return *this;
		}

		/** Returns a reference to this iterator's element after incrementing the given count. */
		Type& operator[](size_t Index) {
			return *(*this + Index);
		}

		/** Returns a constant reference to this iterator's element after incrementing the given count. */
		const Type& operator[](size_t Index) const {
			return *(*this + Index);
		}
	};


	// ITERABLE

	/** An interface that allows an object to be iterated on with for(auto& foo : bar) when implemented. */
	template<typename Iterator, typename ConstantIterator = const Iterator>
	class Iterable {

		// INTERFACE

		/** Returns an iterator to the first element in this collection. */
		virtual Iterator begin() = 0;

		/** Returns a constant iterator to the first element in this collection. */
		virtual ConstantIterator begin() const = 0;

		/** Returns an iterator to the element after the last element in this collection. */
		virtual Iterator end() = 0;

		/** Returns a constant iterator to the element after the last element in this collection. */
		virtual ConstantIterator end() const = 0;


		// ITERATOR

		/** Returns a constant iterator to the first element in this collection. */
		ConstantIterator cbegin() const {
			return begin();
		}

		/** Returns a constant iterator to the element after the last element in this collection. */
		ConstantIterator cend() const {
			return end();
		}
	};
}
