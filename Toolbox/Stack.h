// .h
// Stack Type
// by Kyle Furey

#pragma once
#include "List.h"

/** A collection of useful template types in C++. */
namespace Toolbox {

	// STACK

	/** Represents a first-in, last-out, double-ended stack of the given type. */
	template<typename Type>
	class Stack final {

		// DATA

		/** The underlying list managing this stack. */
		List<Type> data;

	public:

		// CONSTRUCTORS AND DESTRUCTOR

		/** Default constructor. */
		Stack() : data() {
		}

		/** Fill constructor. */
		Stack(const size_t Size, const Type& Value = Type()) : data(Size, Value) {
		}

		/** Array constructor. */
		Stack(const size_t Size, const Type* Array) : data(Size, Array) {
		}

		/** Initializer list constructor. */
		Stack(const std::initializer_list<Type>& List) : data(List) {
		}


		// GETTERS

		/** Returns the number of elements in the stack. */
		size_t Size() const {
			return data.Size();
		}

		/** Returns a constant reference to the next element in the stack. */
		const Type& Peek() const {
			if (data.IsEmpty()) {
				throw std::runtime_error("ERROR: The stack is empty!");
			}
			return data.Back();
		}

		/** Returns a constant reference to the last element in the stack. */
		const Type& PeekLast() const {
			if (data.IsEmpty()) {
				throw std::runtime_error("ERROR: The stack is empty!");
			}
			return data.Front();
		}

		/** Returns whether the given value is present in the stack. */
		bool Contains(const Type& Value) const {
			return data.Contains(Value);
		}

		/** Returns the total number of elements that match the given value in the stack. */
		size_t Total(const Type& Value) const {
			return data.Total(Value);
		}

		/** Returns whether the stack is empty. */
		bool IsEmpty() const {
			return data.IsEmpty();
		}


		// SETTERS

		/** Reverses the stack. */
		void Reverse() {
			data.Reverse();
		}


		// EXPANSION

		/** Deallocates the stack. */
		void Clear() {
			data.Clear();
		}

		/** Pushes a copy of the given data to the front of the stack. */
		void Push(const Type& Value) {
			data.PushBack(Value);
		}

		/** Pushes a copy of the given data to the end of the stack. */
		void PushLast(const Type& Value) {
			data.PushFront(Value);
		}

		/** Pushes a new element with the given arguments to the front of the quue. */
		template<typename... ArgumentTypes>
		void Emplace(ArgumentTypes&&... Arguments) {
			data.EmplaceBack(Arguments...);
		}

		/** Pushes a new element with the given arguments to the end of the stack. */
		template<typename... ArgumentTypes>
		void EmplaceLast(ArgumentTypes&&... Arguments) {
			data.EmplaceFront(Arguments...);
		}

		/** Removes and returns the next element in the stack. */
		Type Pop() {
			if (data.IsEmpty()) {
				throw std::runtime_error("ERROR: The stack is empty!");
			}
			Type back = data.Back();
			data.PopBack();
			return back;
		}

		/** Removes and returns the last element in the stack. */
		Type PopLast() {
			if (data.IsEmpty()) {
				throw std::runtime_error("ERROR: The stack is empty!");
			}
			Type front = data.Front();
			data.PopFront();
			return front;
		}


		// TO STRING

		/** Returns the stack as a string. */
		std::string ToString() const {
			std::string String;
			for (ptrdiff_t Index = data.Size() - 1; Index >= 0; --Index) {
				String += "{ " + std::to_string(data[Index]) + " } -> ";
			}
			if (!String.empty()) {
				String.erase(String.length() - 4, 4);
			}
			else {
				return "{ }";
			}
			return String;
		}


		// AS LIST

		/** Returns a reference to this stack's underlying list. */
		List<Type>& AsList() {
			return data;
		}

		/** Returns a constant reference to this stack's underlying list. */
		const List<Type>& AsList() const {
			return data;
		}
	};
}
