// .h
// Static Algorithm Functions
// by Kyle Furey

#pragma once
#include "Vector.h"
#include "Sorting.h"

/** A collection of useful template types in C++. */
namespace Toolbox {

	// ALGORITHMS

	/** Useful algorithms for collections. */
	namespace Algorithms {

		// MAP

		/** Applies the given operation on each element in the given collection. */
		template <typename CollectionType, typename Type>
		static CollectionType& Map(CollectionType& Collection, void(*Operation)(Type&)) {
			for (auto& Element : Collection) {
				Operation(Element);
			}
			return Collection;
		}

		/** Applies the given operation on each element in the given collection. */
		template <typename CollectionType, typename Type>
		static CollectionType& Map(size_t Size, CollectionType& Collection, void(*Operation)(Type&)) {
			for (auto& Element : Collection) {
				if (Size == 0) {
					break;
				}
				Operation(Element);
				--Size;
			}
			return Collection;
		}


		// FILTER

		/** Returns a vector of pointers to elements in the given collection that match the given predicate. */
		template <typename CollectionType, typename Type>
		static Vector<Type*> Filter(CollectionType& Collection, bool(*Predicate)(const Type&)) {
			Vector<Type*> NewCollection;
			for (auto& Element : Collection) {
				if (Predicate(Element)) {
					NewCollection.PushBack(&Element);
				}
			}
			return NewCollection;
		}

		/** Returns a vector of pointers to elements in the given collection that match the given predicate. */
		template <typename CollectionType, typename Type>
		static Vector<Type*> Filter(size_t Size, CollectionType& Collection, bool(*Predicate)(const Type&)) {
			Vector<Type*> NewCollection(Size);
			for (auto& Element : Collection) {
				if (Size == 0) {
					break;
				}
				if (Predicate(Element)) {
					NewCollection.PushBack(&Element);
				}
				--Size;
			}
			return NewCollection;
		}

		/** Returns a vector of constant pointers to elements in the given collection that match the given predicate. */
		template <typename CollectionType, typename Type>
		static Vector<const Type*> Filter(const CollectionType& Collection, bool(*Predicate)(const Type&)) {
			Vector<const Type*> NewCollection;
			for (auto& Element : Collection) {
				if (Predicate(Element)) {
					NewCollection.PushBack(&Element);
				}
			}
			return NewCollection;
		}

		/** Returns a vector of constant pointers to elements in the given collection that match the given predicate. */
		template <typename CollectionType, typename Type>
		static Vector<const Type*> Filter(size_t Size, const CollectionType& Collection, bool(*Predicate)(const Type&)) {
			Vector<const Type*> NewCollection(Size);
			for (auto& Element : Collection) {
				if (Size == 0) {
					break;
				}
				if (Predicate(Element)) {
					NewCollection.PushBack(&Element);
				}
				--Size;
			}
			return NewCollection;
		}


		// REDUCE

		/** Returns the result of combining all elements in the given collection with the given accumulator. */
		template <typename CollectionType, typename Type>
		static Type Reduce(const CollectionType& Collection, Type(*Accumulator)(const Type&, const Type&)) {
			Type Result{};
			for (auto& Element : Collection) {
				Result = Accumulator(Result, Element);
			}
			return Result;
		}

		/** Returns the result of combining all elements in the given collection with the given accumulator. */
		template <typename CollectionType, typename Type>
		static Type Reduce(size_t Size, const CollectionType& Collection, Type(*Accumulator)(const Type&, const Type&)) {
			Type Result{};
			for (auto& Element : Collection) {
				if (Size == 0) {
					break;
				}
				Result = Accumulator(Result, Element);
				--Size;
			}
			return Result;
		}

		/** Returns the result of combining all elements in the given collection with the given accumulator. */
		template <typename CollectionType, typename Type>
		static Type Reduce(const CollectionType& Collection, Type Start, Type(*Accumulator)(const Type&, const Type&)) {
			for (auto& Element : Collection) {
				Start = Accumulator(Start, Element);
			}
			return Start;
		}

		/** Returns the result of combining all elements in the given collection with the given accumulator. */
		template <typename CollectionType, typename Type>
		static Type Reduce(size_t Size, const CollectionType& Collection, Type Start, Type(*Accumulator)(const Type&, const Type&)) {
			for (auto& Element : Collection) {
				if (Size == 0) {
					break;
				}
				Start = Accumulator(Start, Element);
				--Size;
			}
			return Start;
		}

		/** Returns the result of combining all elements in the given collection with the given accumulator. */
		template <typename CollectionType, typename Type>
		static Type Reduce(const CollectionType& Collection, void(*Accumulator)(Type&, const Type&)) {
			Type Result{};
			for (auto& Element : Collection) {
				Accumulator(Result, Element);
			}
			return Result;
		}

		/** Returns the result of combining all elements in the given collection with the given accumulator. */
		template <typename CollectionType, typename Type>
		static Type Reduce(size_t Size, const CollectionType& Collection, void(*Accumulator)(Type&, const Type&)) {
			Type Result{};
			for (auto& Element : Collection) {
				if (Size == 0) {
					break;
				}
				Accumulator(Result, Element);
				--Size;
			}
			return Result;
		}

		/** Returns the result of combining all elements in the given collection with the given accumulator. */
		template <typename CollectionType, typename Type>
		static Type Reduce(const CollectionType& Collection, Type Start, void(*Accumulator)(Type&, const Type&)) {
			for (auto& Element : Collection) {
				Accumulator(Start, Element);
			}
			return Start;
		}

		/** Returns the result of combining all elements in the given collection with the given accumulator. */
		template <typename CollectionType, typename Type>
		static Type Reduce(size_t Size, const CollectionType& Collection, Type Start, void(*Accumulator)(Type&, const Type&)) {
			for (auto& Element : Collection) {
				if (Size == 0) {
					break;
				}
				Accumulator(Start, Element);
				--Size;
			}
			return Start;
		}


		// SORT

		/** Quick sorts the given collection with the given comparer. */
		template <typename CollectionType, typename Type>
		static CollectionType& Sort(CollectionType& Collection, bool(*Comparer)(const Type&, const Type&) = Sorting::GreaterThan) {
			Sorting::QuickSort<CollectionType, Type>(Collection.Size(), Collection, Comparer);
			return Collection;
		}

		/** Quick sorts the given collection with the given comparer. */
		template <typename CollectionType, typename Type>
		static CollectionType& Sort(size_t Size, CollectionType& Collection, bool(*Comparer)(const Type&, const Type&) = Sorting::GreaterThan) {
			Sorting::QuickSort<CollectionType, Type>(Size, Collection, Comparer);
			return Collection;
		}


		// MOVE

		/** Moves the From value to To, and returns a reference to To. */
		template<typename Type>
		static Type& Move(Type& From, Type& To) {
			To = static_cast<Type&&>(From);
			return To;
		}


		// SWAP

		/** Swaps the given values. */
		template<typename Type>
		static void Swap(Type& Left, Type& Right) {
			Type Temp = Left;
			Left = Right;
			Right = Temp;
		}


		// COPY

		/** Copies the From value to To, and returns a reference to To. */
		template<typename Type>
		static Type& Copy(const Type& From, Type& To) {
			To = From;
			return To;
		}
	}
}
