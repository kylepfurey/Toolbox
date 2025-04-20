// .h
// Iterator Collection Type
// by Kyle Furey

#pragma once
#include <stdexcept>

/** A collection of useful template types in C++. */
namespace Toolbox {

	// COLLECTION

	/** Represents a temporary collection comprised of two iterators that enable for(auto& foo : bar) iteration. */
	template<typename Iterator>
	class Collection final {

		// DATA

		/** The total number of elements in this collection. */
		size_t size;

		/** An iterator representing the first element. */
		Iterator first;

		/** An iterator representing the element after the last element. */
		Iterator last;

	public:

		// CONSTRUCTORS

		/** Default constructor. */
		Collection(const Iterator& Begin, const Iterator& End) : size(0), first(Begin), last(End) {
			Iterator First = first;
			while (First != last) {
				++First;
				++size;
			}
		}

		/** Collection constructor. */
		template<typename CollectionType>
		Collection(CollectionType& Collection) : size(0), first(Collection.begin()), last(Collection.end()) {
			Iterator First = first;
			while (First != last) {
				++First;
				++size;
			}
		}

		/** Constant collection constructor. */
		template<typename CollectionType>
		Collection(const CollectionType& Collection) : size(0), first(Collection.begin()), last(Collection.end()) {
			Iterator First = first;
			while (First != last) {
				++First;
				++size;
			}
		}


		// OPERATORS

		/** Returns a reference to the data at the given index. */
		auto& operator[](size_t Index) {
			if (Index >= size) {
				throw std::runtime_error(
					std::string("ERROR: Index ") +
					std::to_string(Index) +
					" is out of bounds of the collection of size " +
					std::to_string(size) + ".");
			}
			Iterator Current = first;
			while (Index > 0) {
				++Current;
				--Index;
			}
			return *Current;
		}

		/** Returns a constant reference to the data at the given index. */
		const auto& operator[](size_t Index) const {
			if (Index >= size) {
				throw std::runtime_error(
					std::string("ERROR: Index ") +
					std::to_string(Index) +
					" is out of bounds of the collection of size " +
					std::to_string(size) + ".");
			}
			Iterator Current = first;
			while (Index > 0) {
				++Current;
				--Index;
			}
			return *Current;
		}


		// ITERATORS

		/** Returns an iterator to the first element in this collection. */
		Iterator begin() {
			return first;
		}

		/** Returns a constant iterator to the first element in this collection. */
		Iterator begin() const {
			return first;
		}

		/** Returns an iterator to the element after the last element in this collection. */
		Iterator end() {
			return last;
		}

		/** Returns a constant iterator to the element after the last element in this collection. */
		Iterator end() const {
			return last;
		}

		/** Returns a constant iterator to the first element in this collection. */
		Iterator cbegin() const {
			return begin();
		}

		/** Returns a constant iterator to the element after the last element in this collection. */
		Iterator cend() const {
			return end();
		}


		// SIZE

		/** Returns the size of this collection. */
		size_t Size() const {
			return size;
		}
	};
}
