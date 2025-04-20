// .h
// Linked List Type
// by Kyle Furey

#pragma once
#include <string>
#include <random>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include "Sorting.h"

/** A collection of useful template types in C++. */
namespace Toolbox {

	// LINKED LIST

	/** Represents a doubly linked list of the given type. */
	template<typename Type>
	class List final {

		// NODE

		/** An individual doubly linked node in a linked list. */
		struct Node final {

			// DATA

			/** A pointer to the previous node. */
			Node* previous;

			/** The underlying data of this node. */
			Type* data;

			/** A pointer to the next node. */
			Node* next;


			// CONSTRUCTORS AND DESTRUCTOR

			/** Default constructor. */
			Node(const Type& Value = Type(), Node* Previous = nullptr, Node* Next = nullptr) : previous(Previous), data(new Type(Value)), next(Next) {
			}

			/** Null constructor. */
			Node(std::nullptr_t) : previous(nullptr), data(nullptr), next(nullptr) {
			}


			// OPERATORS

			/** Null assignment operator. */
			Node& operator=(std::nullptr_t) {
				previous = nullptr;
				data = nullptr;
				next = nullptr;
				return *this;
			}

			/** Returns a reference to the underlying element of this node. */
			Type& operator*() {
				return *data;
			}

			/** Returns a constant reference to the underlying element of this node. */
			const Type& operator*() const {
				return *data;
			}

			/** Returns a dereferenced pointer to the underlying element of this node. */
			Type* operator->() {
				return data;
			}

			/** Returns a dereferenced constant pointer to the underlying element of this node. */
			const Type* operator->() const {
				return data;
			}

			/** Increments this node to the next element. */
			Node& operator++() {
				if (next == nullptr) {
					*this = nullptr;
					return *this;
				}
				*this = *next;
				return *this;
			}

			/** Increments this node to the next node and returns a copy of the previous node. */
			Node& operator++(int) {
				++(*this);
				return *previous;
			}

			/** Returns a copy of this node after incrementing the given count. */
			Node& operator+(const ptrdiff_t Count) {
				if (Count == 0) {
					return *this;
				}
				Node Copy = *this;
				Copy += Count - 1;
				return *Copy.next;
			}

			/** Returns a reference to this node after incrementing the given count. */
			Node& operator+=(ptrdiff_t Count) {
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

			/** Decrements this node to the previous element. */
			Node& operator--() {
				if (previous == nullptr) {
					*this = nullptr;
					return *this;
				}
				*this = *previous;
				return *this;
			}

			/** Decrements this node to the previous node and returns a copy of the next node. */
			Node& operator--(int) {
				--(*this);
				return *next;
			}

			/** Returns a copy of this node after decrementing the given count. */
			Node& operator-(const ptrdiff_t Count) {
				if (Count == 0) {
					return *this;
				}
				Node Copy = *this;
				Copy -= Count + 1;
				return *Copy.previous;
			}

			/** Returns a reference to this node after decrementing the given count. */
			Node& operator-=(ptrdiff_t Count) {
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

			/** Returns a reference to this node's element after incrementing the given count. */
			Type& operator[](size_t Index) {
				return *(*this + Index);
			}

			/** Returns a constant reference to this node's element after incrementing the given count. */
			const Type& operator[](size_t Index) const {
				return *(*this + Index);
			}

			/** Returns whether the given nodes are equal. */
			bool operator==(const Node& Other) const {
				return data == Other.data;
			}

			/** Returns whether this node has a valid next pointer. */
			bool operator!=(const Node& Other) const {
				return !(*this == Other);
			}

			/** Returns whether this node is valid. */
			explicit operator bool() const {
				return data != nullptr;
			}

			/** Returns whether this node is not valid. */
			bool operator!() const {
				if (*this) {
					return false;
				}
				return true;
			}
		};


		// DATA

		/** The current size of the list. */
		size_t size;

		/** A pointer to the first node of the list. */
		Node* head;

		/** A pointer to the last node of the list. */
		Node* tail;

		/** A sentinel node used to indicate the end of a list. */
		Node NIL;


		// NODE

		/** Traverses the list to reach the node at the given index using the shortest path. */
		Node& Traverse(const size_t Index) {
			if (!IsValidIndex(Index)) {
				throw std::runtime_error(
					std::string("ERROR: Index ") +
					std::to_string(Index) +
					" is out of bounds of the list of size " +
					std::to_string(size) + ".");
			}
			if (Index > size / 2) {
				return *tail - (size - Index - 1);
			}
			else {
				return *head + Index;
			}
		}

		/** Traverses the list to reach the node at the given index using the shortest path (constant). */
		const Node& Traverse(const size_t Index) const {
			if (!IsValidIndex(Index)) {
				throw std::runtime_error(
					std::string("ERROR: Index ") +
					std::to_string(Index) +
					" is out of bounds of the list of size " +
					std::to_string(size) + ".");
			}
			if (Index > size / 2) {
				return *tail - (size - Index - 1);
			}
			else {
				return *head + Index;
			}
		}

	public:

		// CONSTRUCTORS AND DESTRUCTOR

		/** Default constructor. */
		List() : size(0), head(nullptr), tail(nullptr), NIL(nullptr) {
		}

		/** Fill constructor. */
		List(size_t Size, const Type& Value = Type()) : size(0), head(nullptr), tail(nullptr), NIL(nullptr) {
			for (size_t Index = 0; Index < Size; ++Index) {
				PushBack(Value);
			}
		}

		/** Array constructor. */
		List(const size_t Size, const Type* Array) : size(0), head(nullptr), tail(nullptr), NIL(nullptr) {
			for (size_t Index = 0; Index < Size; ++Index) {
				PushBack(Array[Index]);
			}
		}

		/** Initializer list constructor. */
		List(const std::initializer_list<Type>& List) : size(0), head(nullptr), tail(nullptr), NIL(nullptr) {
			for (size_t Index = 0; Index < List.size(); ++Index) {
				PushBack(List.begin()[Index]);
			}
		}

		/** Copy constructor. */
		List(const List<Type>& Copied) : size(0), head(nullptr), tail(nullptr), NIL(nullptr) {
			for (auto& Element : Copied) {
				PushBack(Element);
			}
		}

		/** Move constructor. */
		List(List<Type>&& Moved) noexcept : size(Moved.size), head(Moved.head), tail(Moved.tail), NIL(nullptr) {
			Moved.size = 0;
			Moved.head = nullptr;
			Moved.tail = nullptr;
		}

		/** Destructor. */
		~List() {
			Clear();
		}


		// OPERATORS

		/** Copy assignment operator. */
		List<Type>& operator=(const List<Type>& Copied) {
			if (this == &Copied) {
				return *this;
			}
			Clear();
			for (auto& Element : Copied) {
				PushBack(Element);
			}
			return *this;
		}

		/** Move assignment operator. */
		List<Type>& operator=(List<Type>&& Moved) noexcept {
			if (this == &Moved) {
				return *this;
			}
			Clear();
			size = Moved.size;
			head = Moved.head;
			tail = Moved.tail;
			Moved.size = 0;
			Moved.head = nullptr;
			Moved.tail = nullptr;
			return *this;
		}

		/** Returns a reference to the data at the given index. */
		Type& operator[](size_t Index) {
			if (!IsValidIndex(Index)) {
				throw std::runtime_error(
					std::string("ERROR: Index ") +
					std::to_string(Index) +
					" is out of bounds of the list of size " +
					std::to_string(size) + ".");
			}
			return *Traverse(Index);
		}

		/** Returns a constant reference to the data at the given index. */
		const Type& operator[](size_t Index) const {
			if (!IsValidIndex(Index)) {
				throw std::runtime_error(
					std::string("ERROR: Index ") +
					std::to_string(Index) +
					" is out of bounds of the list of size " +
					std::to_string(size) + ".");
			}
			return *Traverse(Index);
		}


		// ITERATORS

		/** Returns a reference to the first node in the list. */
		Node& begin() {
			if (head == nullptr) {
				return NIL;
			}
			return *head;
		}

		/** Returns a constant reference to the first node in the list. */
		const Node& begin() const {
			if (head == nullptr) {
				return NIL;
			}
			return *head;
		}

		/** Returns a reference to the NIL node. */
		Node& end() {
			return NIL;
		}

		/** Returns a constant reference to the NIL node. */
		const Node& end() const {
			return NIL;
		}


		// GETTERS

		/** Returns the number of elements in the list. */
		size_t Size() const {
			return size;
		}

		/** Returns whether the given index is a valid index in the list. */
		bool IsValidIndex(const size_t Index) const {
			return Index < size;
		}

		/** Returns a reference to the data at the given index. */
		Type& Get(const size_t Index) {
			return (*this)[Index];
		}

		/** Returns a constant reference to the data at the given index. */
		const Type& Get(const size_t Index) const {
			return (*this)[Index];
		}

		/** Returns a reference to the data at the front of the list. */
		Type& Front() {
			return (*this)[0];
		}

		/** Returns a constant reference to the data at the front of the list. */
		const Type& Front() const {
			return (*this)[0];
		}

		/** Returns a reference to the data at the back of the list. */
		Type& Back() {
			return (*this)[size - 1];
		}

		/** Returns a constant reference to the data at the back of the list. */
		const Type& Back() const {
			return (*this)[size - 1];
		}

		/** Returns the index of the first matching value in the list, or -1 if no match is found. */
		ptrdiff_t Find(const Type& Value) const {
			ptrdiff_t Index = 0;
			for (auto& Element : *this) {
				if (Value == Element) {
					return Index;
				}
				++Index;
			}
			return -1;
		}

		/** Returns the index of the last matching value in the list, or -1 if no match is found. */
		ptrdiff_t FindLast(const Type& Value) const {
			ptrdiff_t Index = size - 1;
			Node* Node = tail;
			while (Node != nullptr) {
				if (Value == **Node) {
					return Index;
				}
				--Index;
				Node = Node->previous;
			}
			return -1;
		}

		/** Returns whether the given value is present in the list. */
		bool Contains(const Type& Value) const {
			return Find(Value) != -1;
		}

		/** Returns the total number of elements that match the given value in the list. */
		size_t Total(const Type& Value) const {
			size_t Total = 0;
			for (auto& Element : *this) {
				if (Value == Element) {
					++Total;
				}
			}
			return Total;
		}

		/** Returns whether the list is sorted using Type's > operator. */
		bool IsSorted() const {
			return Sorting::IsSorted<List, Type>(size, *this);
		}

		/** Returns whether the list is empty. */
		bool IsEmpty() const {
			return size == 0;
		}


		// SETTERS

		/** Sets the data at the given index with a copy of the given value. */
		void Set(const size_t Index, const Type& Value) {
			(*this)[Index] = Value;
		}

		/** Swaps the given elements at the given indicies. */
		void Swap(const size_t Left, const size_t Right) {
			if (!IsValidIndex(Left)) {
				throw std::runtime_error(
					std::string("ERROR: Index ") +
					std::to_string(Left) +
					" is out of bounds of the list of size " +
					std::to_string(size) + ".");
			}
			if (!IsValidIndex(Right)) {
				throw std::runtime_error(
					std::string("ERROR: Index ") +
					std::to_string(Right) +
					" is out of bounds of the list of size " +
					std::to_string(size) + ".");
			}
			Node* LeftNode = &Traverse(Left);
			Node* RightNode = &Traverse(Right);
			Type Swapped = **LeftNode;
			**LeftNode = **RightNode;
			**RightNode = Swapped;
		}

		/** Fills the list with a copy of the given value. */
		void Fill(const Type& Value) {
			for (auto& Element : *this) {
				Element = Value;
			}
		}

		/** Bubble sorts the list using Type's > operator. */
		void BubbleSort() {
			Sorting::BubbleSort<List, Type>(size, *this);
		}

		/** Merge sorts the list using Type's > operator. */
		void MergeSort() {
			Sorting::MergeSort<List, Type>(size, *this);
		}

		/** Quick sorts the list using Type's > operator. */
		void QuickSort() {
			Sorting::QuickSort<List, Type>(size, *this);
		}

		/** Reverses the list. */
		void Reverse() {
			for (size_t Index = 0; Index < size / 2; ++Index) {
				Swap(Index, size - Index - 1);
			}
		}

		/** Shuffles the list. */
		void Shuffle() {
			for (size_t Index = 0; Index < size; ++Index) {
				const size_t Random = std::rand() % size;
				Swap(Index, Random);
			}
		}


		// EXPANSION

		/** Deallocates the list. */
		void Clear() {
			Node* Current = head;
			while (Current != nullptr) {
				Node* Next = Current->next;
				delete Current->data;
				Current->data = nullptr;
				delete Current;
				Current = Next;
			}
			Current = nullptr;
			size = 0;
			head = nullptr;
			tail = nullptr;
		}

		/** Inserts a copy of the given data at the given index in the list. */
		Type& Insert(const size_t Index, const Type& Value) {
			if (Index > size) {
				throw std::runtime_error(
					std::string("ERROR: Index ") +
					std::to_string(Index) +
					" is out of bounds of the list of size " +
					std::to_string(size) + ".");
			}
			if (Index == size) {
				if (IsEmpty()) {
					head = new Node(Value);
					tail = head;
					++size;
					return **head;
				}
				tail->next = new Node(Value, tail);
				tail = tail->next;
				++size;
				return **tail;
			}
			Node* Next = &Traverse(Index);
			Node* New = new Node(Value, Next->previous, Next);
			if (Next->previous != nullptr) {
				Next->previous->next = New;
			}
			else {
				head = New;
			}
			Next->previous = New;
			++size;
			return *New->data;
		}

		/** Pushes a copy of the given data to the front of the list. */
		Type& PushFront(const Type& Value) {
			return Insert(0, Value);
		}

		/** Pushes a copy of the given data to the back of the list. */
		Type& PushBack(const Type& Value) {
			return Insert(size, Value);
		}

		/** Inserts a new element with the given arguments at the given index in the list. */
		template<typename... ArgumentTypes>
		Type& Emplace(const size_t Index, ArgumentTypes&&... Arguments) {
			if (Index > size) {
				throw std::runtime_error(
					std::string("ERROR: Index ") +
					std::to_string(Index) +
					" is out of bounds of the list of size " +
					std::to_string(size) + ".");
			}
			if (Index == size) {
				if (IsEmpty()) {
					head = new Node(Type(std::forward<ArgumentTypes>(Arguments)...));
					tail = head;
					++size;
					return **head;
				}
				tail->next = new Node(Type(std::forward<ArgumentTypes>(Arguments)...), tail);
				tail = tail->next;
				++size;
				return **tail;
			}
			Node* Next = &Traverse(Index);
			Node* New = new Node(Type(std::forward<ArgumentTypes>(Arguments)...), Next->previous, Next);
			if (Next->previous != nullptr) {
				Next->previous->next = New;
			}
			else {
				head = New;
			}
			Next->previous = New;
			++size;
			return *New->data;
		}

		/** Pushes a new element with the given arguments to the front of the list. */
		template<typename... ArgumentTypes>
		Type& EmplaceFront(ArgumentTypes&&... Arguments) {
			return Emplace(0, Arguments...);
		}

		/** Pushes a new element with the given arguments to the back of the list. */
		template<typename... ArgumentTypes>
		Type& EmplaceBack(ArgumentTypes&&... Arguments) {
			return Emplace(size, Arguments...);
		}

		/** Removes the element at the given index. */
		void Erase(const size_t Index) {
			if (!IsValidIndex(Index)) {
				throw std::runtime_error(
					std::string("ERROR: Index ") +
					std::to_string(Index) +
					" is out of bounds of the list of size " +
					std::to_string(size) + ".");
			}
			if (Index == 0) {
				if (size == 1) {
					delete head->data;
					head->data = nullptr;
					delete head;
					head = nullptr;
					tail = nullptr;
				}
				else {
					head = head->next;
					delete head->previous->data;
					head->previous->data = nullptr;
					delete head->previous;
					head->previous = nullptr;
				}
			}
			else if (Index == size - 1) {
				tail = tail->previous;
				delete tail->next->data;
				tail->next->data = nullptr;
				delete tail->next;
				tail->next = nullptr;
			}
			else {
				Node* Next = Traverse(Index).next;
				Node* Previous = Next->previous->previous;
				delete Next->previous->data;
				Next->previous->data = nullptr;
				delete Next->previous;
				Next->previous = Previous;
				if (Previous != nullptr) {
					Previous->next = Next;
				}
			}
			--size;
		}

		/** Removes the element at the front of the list. */
		void PopFront() {
			Erase(0);
		}

		/** Removes the element at the back of the list. */
		void PopBack() {
			Erase(size - 1);
		}


		// TO STRING

		/** Returns the list as a string. */
		std::string ToString() const {
			std::string String;
			for (auto& Element : *this) {
				String += "( " + std::to_string(Element) + " ) -> ";
			}
			return String += "NULL";
		}
	};
}
