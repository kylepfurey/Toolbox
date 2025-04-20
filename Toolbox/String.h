// .h
// Basic String Type
// by Kyle Furey

#pragma once
#include <iostream>
#include "Vector.h"

// Whether uppercase characters are greater than lowercase characters.
#define UPPERCASE_GREATER ('A' > 'a')

// The difference from lowercase characters to uppercase characters.
#define CASE_DIFFERENCE ('A' - 'a')

// A character that represents the end of a string.
#define END_OF_STRING 0

// Declares a new std::to_string implementation with the given parameter.
#define DECLARE_TO_STRING(Parameter) namespace std { static std::string to_string(Parameter); }\
static std::string std::to_string(Parameter)

// Declares a new std::to_string implementation with the given parameter and templated types.
#define DECLARE_TEMPLATED_TO_STRING(Templates, Parameter) namespace std { template<Templates> static std::string to_string(Parameter); }\
template<Templates> static std::string std::to_string(Parameter)

/** A collection of useful template types in C++. */
namespace Toolbox {

	// CHARACTERS

	/** Returns whether the given character is lowercase. */
	template<typename CharacterType = char>
	static bool IsLowercase(const CharacterType Character) {
		return Character >= 'a' && Character <= 'z';
	}

	/** Returns whether the given character is uppercase. */
	template<typename CharacterType = char>
	static bool IsUppercase(const CharacterType Character) {
		return Character >= 'A' && Character <= 'Z';
	}

	/** Returns whether the given character is an alphabetical letter. */
	template<typename CharacterType = char>
	static bool IsAlphabetical(const CharacterType Character) {
		return IsLowercase(Character) || IsUppercase(Character);
	}

	/** Returns whether the given character is a whitespace character. */
	template<typename CharacterType = char>
	static bool IsWhitespace(const CharacterType Character) {
		switch (Character) {
		case ' ':
			return true;
		case '\n':
			return true;
		case '\t':
			return true;
		case '\r':
			return true;
		case '\v':
			return true;
		case '\f':
			return true;
		default:
			return false;
		}
	}

	/** Returns the lowercase version of the given character. */
	template<typename CharacterType = char>
	static CharacterType ToLowercase(const CharacterType Character) {
		if (!IsUppercase(Character)) {
			return Character;
		}
		return Character - CASE_DIFFERENCE;
	}

	/** Returns the uppercase version of the given character. */
	template<typename CharacterType = char>
	static CharacterType ToUppercase(const CharacterType Character) {
		if (!IsLowercase(Character)) {
			return Character;
		}
		return Character + CASE_DIFFERENCE;
	}


	// C STRING

	/** Represents a raw pointer to a constant sequence of characters. */
	using CString = const char[];

	/** Calculates the length of a C string pointer. */
	template<typename CharacterType = char>
	static size_t CStringLength(const CharacterType* String) {
		if (String == nullptr) {
			return 0;
		}
		size_t Index = 0;
		while (String[Index] != END_OF_STRING) {
			++Index;
		}
		return Index;
	}


	// BASIC STRING

	/** Represents a mutable sequence of data that can be iterated on and combined with other strings. */
	template<typename CharacterType = char>
	class BasicString final {
	public:

		// CHARACTER

		/** An alias for one of this string's "characters". */
		using Char = CharacterType;

	private:

		// STRING

		/** An alias for this string. */
		using String = BasicString;


		// DATA

		/** The underlying vector of characters in this string. */
		Vector<Char> string;

	public:

		// CONSTRUCTORS AND DESTRUCTOR

		/** Default constructor. */
		BasicString() : string(1, static_cast<Char>(END_OF_STRING)) {
		}

		/** Character constructor. */
		BasicString(const Char Character) : string({ Character == END_OF_STRING ? '0' : Character, END_OF_STRING }) {
		}

		/** C string constructor. */
		BasicString(const Char* String) : string(CStringLength<Char>(String), String) {
			Validate();
		}

		/** Array constructor. */
		BasicString(const size_t Length, const Char* Array) : string(Length, Array) {
			Validate();
		}

		/** Vector constructor. */
		BasicString(const Vector<Char>& Vector) : string(Vector) {
			Validate();
		}

		/** Standard string constructor. */
		BasicString(const std::basic_string<Char>& String) : string(String.length(), String.c_str()) {
			Validate();
		}

		/** Initializer list constructor. */
		BasicString(const std::initializer_list<Char>& List) : string(List) {
			Validate();
		}

		/** Copy constructor. */
		BasicString(const String& Copied) : string(Copied.string) {
			Validate();
		}

		/** Move constructor. */
		BasicString(String&& Moved) noexcept : string(Moved.string) {
			Moved.string.Clear();
			Moved.string.PushBack(END_OF_STRING);
			Validate();
		}

		/** Null constructor. */
		BasicString(std::nullptr_t) : string(1, static_cast<Char>(END_OF_STRING)) {
		}


		// OPERATORS

		/** Character assignment operator. */
		String& operator=(const Char Character) {
			string = { Character == END_OF_STRING ? '0' : Character, END_OF_STRING };
			return *this;
		}

		/** C string assignment operator. */
		String& operator=(const Char* String) {
			string = Vector<Char>(CStringLength<Char>(String), String);
			Validate();
			return *this;
		}

		/** Vector assignment operator. */
		String& operator=(const Vector<Char>& Vector) {
			string = Vector;
			Validate();
			return *this;
		}

		/** Standard string assignment operator. */
		String& operator=(const std::basic_string<Char>& String) {
			string = Vector<Char>(String.length(), String.c_str());
			Validate();
			return *this;
		}

		/** Copy assignment operator. */
		String& operator=(const String& Copied) {
			if (this == &Copied) {
				return *this;
			}
			string = Copied.string;
			Validate();
			return *this;
		}

		/** Move assignment operator. */
		String& operator=(String&& Moved) noexcept {
			if (this == &Moved) {
				return *this;
			}
			string = Moved.string;
			Moved.string.Clear();
			Moved.string.PushBack(END_OF_STRING);
			Validate();
			return *this;
		}

		/** Null assignment operator. */
		String& operator=(std::nullptr_t) {
			string = Vector<Char>(1, END_OF_STRING);
			return *this;
		}

		/** String equality operator. */
		bool operator==(const String& Other) const {
			if (Length() != Other.Size()) {
				return false;
			}
			for (size_t Index = 0; Index < Other.Length(); ++Index) {
				if (string[Index] != Other.string[Index]) {
					return false;
				}
			}
			return true;
		}

		/** String inequality operator. */
		bool operator!=(const String& Other) const {
			return !(*this == Other);
		}

		/** Character equality operator. */
		bool operator==(const Char Character) const {
			if (Length() != 1) {
				return false;
			}
			return string[0] == Character;
		}

		/** Character inequality operator. */
		bool operator!=(const Char Character) const {
			return !(*this == Character);
		}

		/** C string equality operator. */
		bool operator==(const Char* String) const {
			if (String == nullptr) {
				return IsEmpty();
			}
			size_t Size = CStringLength(String);
			if (Length() != Size) {
				return false;
			}
			for (size_t Index = 0; Index < Size; ++Index) {
				if (string[Index] != String[Index]) {
					return false;
				}
			}
			return true;
		}

		/** C string inequality operator. */
		bool operator!=(const Char* String) const {
			return !(*this == String);
		}

		/** Vector equality operator. */
		bool operator==(const Vector<Char>& Vector) const {
			if (Length() != Vector.Size()) {
				return false;
			}
			for (size_t Index = 0; Index < Vector.Size(); ++Index) {
				if (string[Index] != Vector[Index]) {
					return false;
				}
			}
			return true;
		}

		/** Vector inequality operator. */
		bool operator!=(const Vector<Char>& Vector) const {
			return !(*this == Vector);
		}

		/** Standard string equality operator. */
		bool operator==(const std::basic_string<Char>& String) const {
			if (Length() != String.length()) {
				return false;
			}
			for (size_t Index = 0; Index < String.length(); ++Index) {
				if (string[Index] != String[Index]) {
					return false;
				}
			}
			return true;
		}

		/** Standard string inequality operator. */
		bool operator!=(const std::basic_string<Char>& String) const {
			return !(*this == String);
		}

		/** Returns a reference to the character at the given index. */
		Char& operator[](const size_t Index) {
			if (!IsValidIndex(Index)) {
				throw std::runtime_error(
					std::basic_string<Char>("ERROR: Index ") +
					std::to_string(Index) +
					" is out of bounds of the string of length " +
					std::to_string(Length()) + ".");
			}
			return string[Index];
		}

		/** Returns a copy of the character at the given index. */
		Char operator[](const size_t Index) const {
			if (!IsValidIndex(Index)) {
				throw std::runtime_error(
					std::basic_string<Char>("ERROR: Index ") +
					std::to_string(Index) +
					" is out of bounds of the string of length " +
					std::to_string(Length()) + ".");
			}
			return string[Index];
		}

		/** Returns a copy of this string with the given character appended. */
		String operator+(const Char Character) const {
			String String = *this;
			String += Character;
			return String;
		}

		/** Appends the given character to this string. */
		String& operator+=(const Char Character) {
			return Append(Character);
		}

		/** Returns a copy of this string with the given string appended. */
		String operator+(const String& Other) const {
			String String = *this;
			String += Other;
			return String;
		}

		/** Appends the given string to this string. */
		String& operator+=(const String& Other) {
			return Append(Other);
		}

		/** C string operator. */
		explicit operator Char* () {
			return CString();
		}

		/** Constant C string operator. */
		explicit operator const Char* () const {
			return CString();
		}

		/** Constant vector operator. */
		explicit operator const Vector<Char>() const {
			return string;
		}

		/** Constant standard string operator. */
		explicit operator std::basic_string<Char>() const {
			return ToString();
		}


		// ITERATORS

		/** Returns a pointer to the first character in the string. */
		Char* begin() {
			return string.begin();
		}

		/** Returns a constant pointer to the first character in the string. */
		const Char* begin() const {
			return string.begin();
		}

		/** Returns a pointer to the character after the last character in the string. */
		Char* end() {
			return string.end() - 1;
		}

		/** Returns a constant pointer to the character after the last character in the string. */
		const Char* end() const {
			return string.end() - 1;
		}


		// GETTERS

		/** Returns the current number of characters in the string. */
		size_t Length() const {
			return string.Size() - 1;
		}

		/** Returns the current maximum number of characters in the string. */
		size_t Capacity() const {
			return string.Capacity();
		}

		/** Returns a new string starting at the index of this string with the given number of characters. */
		String Substring(size_t Index) const {
			return Substring(Index, Length() - Index);
		}

		/** Returns a new string starting at the index of this string with the given number of characters. */
		String Substring(size_t Index, size_t Count) const {
			if (!IsValidIndex(Index)) {
				throw std::runtime_error(
					std::basic_string<Char>("ERROR: Index ") +
					std::to_string(Index) +
					" is out of bounds of the string of length " +
					std::to_string(Length()) + ".");
			}
			if (Count > Length() - Index) {
				throw std::runtime_error(
					std::basic_string<Char>("ERROR: Count ") +
					std::to_string(Count) +
					" at index " +
					std::to_string(Index) +
					" is greater than the string of length " +
					std::to_string(Length()) + ".");
			}
			Vector<Char> Substring(Count + 1);
			Count += Index;
			size_t Current = 0;
			for (; Index < Count; ++Index) {
				Substring[Current] = string[Index];
				++Current;
			}
			Substring.Back() = END_OF_STRING;
			return String(Substring);
		}

		/** Returns whether the given index is a valid index in the string. */
		bool IsValidIndex(const size_t Index) const {
			return Index < Length();
		}

		/** Returns a reference to the character at the given index. */
		Char& Get(const size_t Index) {
			if (!IsValidIndex(Index)) {
				throw std::runtime_error(
					std::basic_string<Char>("ERROR: Index ") +
					std::to_string(Index) +
					" is out of bounds of the string of length " +
					std::to_string(Length()) + ".");
			}
			return string[Index];
		}

		/** Returns a copy of the character at the given index. */
		Char Get(const size_t Index) const {
			if (!IsValidIndex(Index)) {
				throw std::runtime_error(
					std::basic_string<Char>("ERROR: Index ") +
					std::to_string(Index) +
					" is out of bounds of the string of length " +
					std::to_string(Length()) + ".");
			}
			return string[Index];
		}

		/** Returns a reference to the character at the front of the string. */
		Char& Front() {
			if (IsEmpty()) {
				throw std::runtime_error("ERROR: The string is empty!");
			}
			return string[0];
		}

		/** Returns a copy of the character at the front of the string. */
		Char Front() const {
			if (IsEmpty()) {
				throw std::runtime_error("ERROR: The string is empty!");
			}
			return string[0];
		}

		/** Returns a reference to the character at the back of the string. */
		Char& Back() {
			if (IsEmpty()) {
				throw std::runtime_error("ERROR: The string is empty!");
			}
			return string[Length() - 1];
		}

		/** Returns a copy of the character at the back of the string. */
		Char Back() const {
			if (IsEmpty()) {
				throw std::runtime_error("ERROR: The string is empty!");
			}
			return string[Length() - 1];
		}

		/** Returns the index of the first matching character in the string, or -1 if no match is found. */
		ptrdiff_t Find(const Char Character) const {
			return string.Find(Character);
		}

		/** Returns the index of the first character of the first matching string in the string, or -1 if no match is found. */
		ptrdiff_t Find(const String& String) const {
			if (Length() < String.Length()) {
				return -1;
			}
			for (size_t Index = 0; Index < Length() - String.Length() + 1; ++Index) {
				if (Matches(Index, String)) {
					return Index;
				}
			}
			return -1;
		}

		/** Returns the index of the last matching character in the string, or -1 if no match is found. */
		ptrdiff_t FindLast(const Char Character) const {
			return string.FindLast(Character);
		}

		/** Returns the index of the first character of the last matching string in the string, or -1 if no match is found. */
		ptrdiff_t FindLast(const String& String) const {
			if (IsEmpty() || String.IsEmpty() || Length() < String.Length()) {
				return -1;
			}
			for (ptrdiff_t Index = Length() - String.Length(); Index >= 0; --Index) {
				if (Matches(Index, String)) {
					return Index;
				}
			}
			return -1;
		}

		/** Returns whether the given character is present in the string. */
		bool Contains(const Char Character) const {
			return string.Contains(Character);
		}

		/** Returns whether the given substring is present in the string. */
		bool Contains(const String& String) const {
			return Find(String) != -1;
		}

		/** Returns the total number of characters that match the given character in the string. */
		size_t Total(const Char Character) const {
			return string.Total(Character);
		}

		/** Returns the total number of strings that match the given string in the string. */
		size_t Total(const String& String) const {
			if (Length() < String.Length()) {
				return 0;
			}
			size_t Total = 0;
			for (size_t Index = 0; Index < Length() - String.Length() + 1; ++Index) {
				if (Matches(Index, String)) {
					++Total;
					Index += String.Length() - 1;
				}
			}
			return Total;
		}

		/** Returns whether the string is empty. */
		bool IsEmpty() const {
			return Length() == 0;
		}

		/** Returns whether this string begins with the given character. */
		bool StartsWith(const Char Character) const {
			if (IsEmpty() || Front() != Character) {
				return false;
			}
			return true;
		}

		/** Returns whether this string begins with the given string. */
		bool StartsWith(const String& String) const {
			return Find(String) == 0;
		}

		/** Returns whether this string ends with the given character. */
		bool EndsWith(const Char Character) const {
			if (IsEmpty() || Back() != Character) {
				return false;
			}
			return true;
		}

		/** Returns whether this string ends with the given string. */
		bool EndsWith(const String& String) const {
			ptrdiff_t Index = Find(String);
			return Index != -1 && static_cast<size_t>(Index == Length()) - String.Length();
		}

		/** Returns whether the characters at and after the given index match the given string. */
		bool Matches(size_t Index, const String& String) const {
			if (!IsValidIndex(Index)) {
				throw std::runtime_error(
					std::basic_string<Char>("ERROR: Index ") +
					std::to_string(Index) +
					" is out of bounds of the string of length " +
					std::to_string(Length()) + ".");
			}
			size_t Count = Index + String.Length();
			if (Count > Length()) {
				return false;
			}
			size_t Current = 0;
			for (; Index < Count; ++Index) {
				if (string[Index] != String[Current]) {
					return false;
				}
				++Current;
			}
			return true;
		}


		// SETTERS

		/** Sets the character at the given index with a copy of the given character. */
		String& Set(const size_t Index, const Char Character) {
			if (!IsValidIndex(Index)) {
				throw std::runtime_error(
					std::basic_string<Char>("ERROR: Index ") +
					std::to_string(Index) +
					" is out of bounds of the string of length " +
					std::to_string(Length()) + ".");
			}
			string[Index] = Character == END_OF_STRING ? '0' : Character;
			return *this;
		}

		/** Swaps the given characters at the given indicies. */
		String& Swap(const size_t Left, const size_t Right) {
			if (!IsValidIndex(Left)) {
				throw std::runtime_error(
					std::basic_string<Char>("ERROR: Index ") +
					std::to_string(Left) +
					" is out of bounds of the string of length " +
					std::to_string(Length()) + ".");
			}
			if (!IsValidIndex(Right)) {
				throw std::runtime_error(
					std::basic_string<Char>("ERROR: Index ") +
					std::to_string(Right) +
					" is out of bounds of the string of length " +
					std::to_string(Length()) + ".");
			}
			string.Swap(Left, Right);
			return *this;
		}

		/** Fills the string with a copy of the given character. */
		String& Fill(const Char Character) {
			string.Fill(Character == END_OF_STRING ? '0' : Character);
			if (string.IsEmpty()) {
				string.PushBack(END_OF_STRING);
			}
			else {
				string[Length()] = END_OF_STRING;
			}
			return *this;
		}

		/** Fills the string with a copy of the given character for the given count. */
		String& Fill(Char Character, const size_t Count) {
			if (Count > Length()) {
				throw std::runtime_error(
					std::basic_string<Char>("ERROR: Count ") +
					std::to_string(Count) +
					" is greater than the string of length " +
					std::to_string(Length()) + ".");
			}
			Character = Character == END_OF_STRING ? '0' : Character;
			for (size_t Index = 0; Index < Count; ++Index) {
				string[Index] = Character;
			}
			if (string.IsEmpty()) {
				string.PushBack(END_OF_STRING);
			}
			else {
				string[Length()] = END_OF_STRING;
			}
			return *this;
		}

		/** Fills the string with a copy of the given character starting from the given index for the given count. */
		String& Fill(Char Character, const size_t Index, size_t Count) {
			if (!IsValidIndex(Index)) {
				throw std::runtime_error(
					std::basic_string<Char>("ERROR: Index ") +
					std::to_string(Index) +
					" is out of bounds of the string of length " +
					std::to_string(Length()) + ".");
			}
			if (Count > Length() - Index) {
				throw std::runtime_error(
					std::basic_string<Char>("ERROR: Count ") +
					std::to_string(Count) +
					" at index " +
					std::to_string(Index) +
					" is greater than the string of length " +
					std::to_string(Length()) + ".");
			}
			Count += Index;
			Character = Character == END_OF_STRING ? '0' : Character;
			for (size_t Current = Index; Current < Count; ++Current) {
				string[Current] = Character;
			}
			if (string.IsEmpty()) {
				string.PushBack(END_OF_STRING);
			}
			else {
				string[Length()] = END_OF_STRING;
			}
			return *this;
		}

		/** Reverses the string. */
		String& Reverse() {
			string.PopBack();
			string.Reverse();
			string.PushBack(END_OF_STRING);
			return *this;
		}

		/** Shuffles the string. */
		String& Shuffle() {
			string.PopBack();
			string.Shuffle();
			string.PushBack(END_OF_STRING);
			return *this;
		}

		/** Turns all alphabetical characters in this string lowercase. */
		String& ToLowercase() {
			for (auto& Character : *this) {
				Character = Toolbox::ToLowercase<Char>(Character);
			}
			return *this;
		}

		/** Turns all alphabetical characters in this string uppercase. */
		String& ToUppercase() {
			for (auto& Character : *this) {
				Character = Toolbox::ToUppercase<Char>(Character);
			}
			return *this;
		}

		/** Chops all characters before the given index off of this string and returns them as a new string. */
		String SplitLeft(const size_t Index) {
			if (!IsValidIndex(Index)) {
				throw std::runtime_error(
					std::basic_string<Char>("ERROR: Index ") +
					std::to_string(Index) +
					" is out of bounds of the string of length " +
					std::to_string(Length()) + ".");
			}
			Vector<Char> Substring(Index);
			for (size_t Current = 0; Current < Index; ++Current) {
				Substring[Current] = string[0];
				string.PopFront();
			}
			return String(Substring);
		}

		/** Chops all characters at and after the given index off of this string and returns them as a new string. */
		String SplitRight(const size_t Index) {
			if (!IsValidIndex(Index)) {
				throw std::runtime_error(
					std::basic_string<Char>("ERROR: Index ") +
					std::to_string(Index) +
					" is out of bounds of the string of length " +
					std::to_string(Length()) + ".");
			}
			Vector<Char> Substring(Length() - Index);
			for (size_t Current = Index; Current < Length(); ++Current) {
				Substring[Current - Index] = string[Current];
			}
			string.PopBack();
			for (size_t Current = 0; Current < Index - 1; ++Current) {
				string.PopBack();
			}
			string.PushBack(END_OF_STRING);
			return String(Substring);
		}


		// EXPANSION

		/** Resizes the string to the given length, appending and truncating new and out of bounds characters. */
		String& Resize(const size_t Length, const Char FillCharacter = ' ') {
			string.PopBack();
			string.Resize(Length);
			while (string.Size() < Length) {
				string.PushBack(FillCharacter);
			}
			string.PushBack(END_OF_STRING);
			return *this;
		}

		/** Sets the string's length to zero. */
		String& Clear() {
			string.Clear();
			string.PushBack(END_OF_STRING);
			return *this;
		}

		/** Deallocates the string. */
		String& Reset() {
			string.Resize(1);
			if (string.IsEmpty()) {
				string.PushBack(END_OF_STRING);
			}
			else {
				string[0] = END_OF_STRING;
			}
			return *this;
		}

		/** Inserts a copy of the given character at the given index in the string. */
		String& Insert(const size_t Index, const Char Character) {
			if (Index > Length()) {
				throw std::runtime_error(
					std::basic_string<Char>("ERROR: Index ") +
					std::to_string(Index) +
					" is out of bounds of the string of length " +
					std::to_string(Length()) + ".");
			}
			string.Insert(Index, Character == END_OF_STRING ? '0' : Character);
			return *this;
		}

		/** Inserts a copy of the given string at the given index in the string. */
		String& Insert(const size_t Index, const String& String) {
			if (this == &String) {
				Insert(Index, BasicString<Char>(String));
				return *this;
			}
			if (Index > Length()) {
				throw std::runtime_error(
					std::basic_string<Char>("ERROR: Index ") +
					std::to_string(Index) +
					" is out of bounds of the string of length " +
					std::to_string(Length()) + ".");
			}
			if (Index == 0) {
				return Prepend(String);
			}
			if (Index == Length()) {
				return Append(String);
			}
			Vector<Char> Copy(string.Size() + String.Length());
			for (size_t Current = 0; Current < Index; ++Current) {
				Copy[Current] = string[Current];
			}
			for (size_t Current = 0; Current < String.Length(); ++Current) {
				Copy[Index + Current] = String[Current];
			}
			for (size_t Current = Index + String.Length(); Current < Copy.Capacity(); ++Current) {
				Copy[Current] = string[Current - String.Length()];
			}
			string = std::move(Copy);
			Validate();
			return *this;
		}

		/** Pushes a copy of the given character to the front of the string. */
		String& Prepend(const Char Character) {
			string.PushFront(Character == END_OF_STRING ? '0' : Character);
			return *this;
		}

		/** Appends the given string at the beginning of this string. */
		String& Prepend(const String& String) {
			if (this == &String) {
				return Prepend(BasicString<Char>(*this));
			}
			for (size_t Index = 0; Index < String.Length(); ++Index) {
				string.Insert(Index, String[Index]);
			}
			return *this;
		}

		/** Pushes a copy of the given character to the back of the string. */
		String& Append(const Char Character) {
			string.PopBack();
			string.PushBack(Character == END_OF_STRING ? '0' : Character);
			string.PushBack(END_OF_STRING);
			return *this;
		}

		/** Appends the given string at the end of this string. */
		String& Append(const String& String) {
			if (this == &String) {
				return Append(BasicString<Char>(*this));
			}
			string.PopBack();
			for (size_t Index = 0; Index < String.Length(); ++Index) {
				string.PushBack(String[Index]);
			}
			string.PushBack(END_OF_STRING);
			return *this;
		}

		/** Removes the character at the given index. */
		String& Erase(const size_t Index) {
			if (!IsValidIndex(Index)) {
				throw std::runtime_error(
					std::basic_string<Char>("ERROR: Index ") +
					std::to_string(Index) +
					" is out of bounds of the string of length " +
					std::to_string(Length()) + ".");
			}
			string.Erase(Index);
			return *this;
		}

		/** Removes the characters at the given index for the given count. */
		String& Erase(const size_t Index, size_t Count) {
			if (!IsValidIndex(Index)) {
				throw std::runtime_error(
					std::basic_string<Char>("ERROR: Index ") +
					std::to_string(Index) +
					" is out of bounds of the string of length " +
					std::to_string(Length()) + ".");
			}
			if (Count > Length() - Index) {
				throw std::runtime_error(
					std::basic_string<Char>("ERROR: Count ") +
					std::to_string(Count) +
					" at index " +
					std::to_string(Index) +
					" is greater than the string of length " +
					std::to_string(Length()) + ".");
			}
			while (Count > 0) {
				string.Erase(Index);
				--Count;
			}
			return *this;
		}

		/** Removes the character at the front of the string. */
		Char PopFront() {
			Char Character = Front();
			Erase(0);
			return Character;
		}

		/** Removes the character at the back of the string. */
		Char PopBack() {
			Char Character = Back();
			Erase(Length() - 1);
			return Character;
		}

		/** Replaces all instances of the given character with the given character. */
		String& Replace(const Char ReplacedCharacter, Char Character) {
			Character = Character == END_OF_STRING ? '0' : Character;
			for (auto& Current : *this) {
				if (Current == ReplacedCharacter) {
					Current = Character;
				}
			}
			return *this;
		}

		/** Replaces all instances of the given character with the given string. */
		String& Replace(const Char ReplacedCharacter, const String& String) {
			if (this == &String) {
				return Replace(ReplacedCharacter, BasicString<Char>(String));
			}
			for (size_t Index = 0; Index < Length(); ++Index) {
				if (string[Index] == ReplacedCharacter) {
					Erase(Index);
					Insert(Index, String);
					Index += String.Length() - 1;
				}
			}
			return *this;
		}

		/** Replaces all instances of the given string with the given character. */
		String& Replace(const String& ReplacedString, Char Character) {
			if (this == &ReplacedString) {
				return Replace(BasicString<Char>(ReplacedString), Character);
			}
			Character = Character == END_OF_STRING ? '0' : Character;
			for (size_t Index = 0; Index < Length(); ++Index) {
				if (Matches(Index, ReplacedString)) {
					Erase(Index, ReplacedString.Length());
					Insert(Index, Character);
				}
			}
			return *this;
		}

		/** Replaces all instances of the given string with the given string. */
		String& Replace(const String& ReplacedString, const String& String) {
			if (this == &ReplacedString) {
				return Replace(BasicString<Char>(ReplacedString), String);
			}
			if (this == &String) {
				return Replace(ReplacedString, BasicString<Char>(String));
			}
			for (size_t Index = 0; Index < Length(); ++Index) {
				if (Matches(Index, ReplacedString)) {
					Erase(Index, ReplacedString.Length());
					Insert(Index, String);
					Index += String.Length() - 1;
				}
			}
			return *this;
		}

		/** Trims the given number of characters from the front of the string and returns them. */
		String TrimFront(const size_t Count) {
			return SplitLeft(Count);
		}

		/** Trims the given number of characters from the back of the string and returns them. */
		String TrimBack(const size_t Count) {
			return SplitRight(Length() - Count);
		}

		/** Removes all whitespace from each end of this string. */
		String& Normalize() {
			size_t Count = 0;
			while (IsWhitespace<Char>(string[Count])) {
				++Count;
			}
			Erase(0, Count);
			if (Length() == 0) {
				return *this;
			}
			Count = Length() - 1;
			string.PopBack();
			while (IsWhitespace<Char>(string[Count])) {
				string.PopBack();
				--Count;
			}
			string.PushBack(END_OF_STRING);
			return *this;
		}


		// C STRING

		/**
		 * Ensures the null terminator character exists at the end of the string.<br/>
		 * This will append it if the null terminator character was not present.<br/>
		 * Returns if the string was valid when the function was called.
		 */
		bool Validate() {
			if (string.IsEmpty() || string.Back() != END_OF_STRING) {
				string.PushBack(END_OF_STRING);
				return false;
			}
			return true;
		}

		/** Returns a pointer to this string as a C string. */
		Char* CString() {
			return string.begin();
		}

		/** Returns a constant pointer to this string as a C string. */
		const Char* CString() const {
			return string.begin();
		}


		// AS VECTOR

		/** Returns a reference to this string's underlying vector. */
		Vector<Char>& AsVector() {
			return string;
		}

		/** Returns a constant reference to this string's underlying vector. */
		const Vector<Char>& AsVector() const {
			return string;
		}


		// TO STRING

		/** Returns a copy of this string as a standard string. */
		std::basic_string<Char> ToString() const {
			return std::basic_string<Char>(string.begin());
		}


		// CONSTANTS

		/** Returns an empty string. */
		static String Empty() {
			return String();
		}
	};


	// STRING TYPE

	/** Represents a sequence of characters that can be iterated on and combined with other strings. */
	using String = BasicString<char>;
}


// INPUT OUTPUT

/** Prints the given string to the given output stream. */
template<typename CharacterType>
std::ostream& operator<<(std::ostream& Output, const Toolbox::BasicString<CharacterType>& String) {
	Output << String.CString();
	return Output;
}

/** Prints the given string to the given input stream. */
template<typename CharacterType>
std::istream& operator>>(std::istream& Input, Toolbox::BasicString<CharacterType>& String) {
	std::string Buffer;
	Input >> Buffer;
	String = Buffer;
	return Input;
}


// TO STRING

/** std::to_string Extension = Converts a standard string into a standard string. */
DECLARE_TEMPLATED_TO_STRING(typename CharacterType, const std::basic_string<CharacterType>& Value) {
	return Value;
}

/** std::to_string Extension = Converts a Toolbox string into a standard string. */
DECLARE_TEMPLATED_TO_STRING(typename CharacterType, const Toolbox::BasicString<CharacterType>& Value) {
	return static_cast<std::basic_string<CharacterType>>(Value);
}
