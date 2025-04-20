// .h
// Nullable Type
// by Kyle Furey

#pragma once
#include <stdexcept>
#include <utility>

/** A collection of useful template types in C++. */
namespace Toolbox {

	// NULLABLE

	/** Represents any value that can have a null state. */
	template<typename Type>
	class Nullable {

		// DATA

		/** Whether this nullable type stores valid data. */
		bool valid;

		/** The underlying data this nullable type stores. */
		Type data;

	public:

		// CONSTRUCTORS

		/** Default constructor. */
		Nullable() : valid(false), data() {
		}

		/** Data constructor. */
		Nullable(const Type& Data) : valid(true), data(Data) {
		}

		/** Argument constructor. */
		template<typename ... ArgumentTypes>
		Nullable(ArgumentTypes&&... Arguments) : valid(true), data(std::forward<ArgumentTypes>(Arguments)...) {
		}

		/** Null constructor. */
		Nullable(std::nullptr_t) : valid(false), data() {
		}


		// OPERATORS

		/** Assignment operator. */
		Nullable& operator=(const Type& Data) {
			valid = true;
			data = Data;
			return *this;
		}

		/** Null assignment operator. */
		Nullable& operator=(std::nullptr_t) {
			valid = false;
			return *this;
		}

		/** Type operator. */
		explicit operator Type& () {
			return Get();
		}

		/** Constant type operator. */
		explicit operator const Type& () const {
			return Get();
		}

		/** Not null check operator. */
		explicit operator bool() const {
			return valid;
		}

		/** Null check operator. */
		bool operator!() const {
			return !valid;
		}


		// GETTERS

		/** Returns whether the data is not null. */
		bool IsValid() const {
			return valid;
		}

		/** Returns whether the data is null. */
		bool IsNull() const {
			return !valid;
		}

		/** Returns a constant reference to the data or throws if it is null. */
		Type& Get() {
			if (!valid) {
				throw std::runtime_error("ERROR: Nullable type was accessed when null!");
			}
			return data;
		}

		/** Returns a constant reference to the data or throws if it is null. */
		const Type& Get() const {
			if (!valid) {
				throw std::runtime_error("ERROR: Nullable type was accessed when null!");
			}
			return data;
		}

		/** Returns whether the data is valid and sets the Out pointer to the underlying data if it is. */
		bool TryGet(Type*& Out) {
			if (!valid) {
				return false;
			}
			Out = &data;
			return true;
		}

		/** Returns whether the data is valid and sets the constant Out pointer to the underlying data if it is. */
		bool TryGet(const Type*& Out) const {
			if (!valid) {
				return false;
			}
			Out = &data;
			return true;
		}


		// SETTERS

		/** Sets the data. */
		void Set(const Type& Data) {
			valid = true;
			data = Data;
		}

		/** Sets the data to null. */
		void Set(std::nullptr_t) {
			valid = false;
		}

		/** Releases the data and nullifes this object. */
		Type Release() {
			if (!valid) {
				throw std::runtime_error("ERROR: Nullable type was accessed when null!");
			}
			valid = false;
			return data;
		}
	};
}
