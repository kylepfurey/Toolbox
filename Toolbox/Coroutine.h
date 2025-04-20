// .h
// Async Coroutine Interfaces and Types
// by Kyle Furey

#pragma once
#include <chrono>
#include <thread>
#include <atomic>

// Check for whether coroutines are supported.
#pragma region INCLUDE_COROUTINE
#if defined(__cpp_lib_coroutine) && (defined(__cpp_impl_coroutine) || defined(__cpp_coroutines))
#include <coroutine>
namespace coroutine = std;
// Whether coroutines are compiled.
#define COROUTINES_COMPILED 1
#elif defined(__cpp_coroutines) || defined(__cpp_impl_coroutine)
#include <experimental/coroutine>
namespace coroutine = std::experimental;
// Whether coroutines are compiled.
#define COROUTINES_COMPILED 1
#else
#pragma message("WARNING: Coroutines are not supported in this version of C++!")
// Whether coroutines are compiled.
#define COROUTINES_COMPILED 0
#endif
#pragma endregion
#include "Nullable.h"

#if COROUTINES_COMPILED

// A keyword used to co_yield a void returning async function.
#define co_pause co_yield nullptr

/** A collection of useful template types in C++. */
namespace Toolbox {

	// PROMISE TYPES

	/** An interface that allows a function to be paused and be resumed later. */
	template<typename GeneratorType, typename ReturnType>
	struct PromiseType {

		// DATA

		/** The value this promise type is currently storing for return. */
		Nullable<ReturnType> Value;


		// CONSTRUCTOR AND DESTRUCTOR

		/** Default constructor. */
		PromiseType() : Value(nullptr) {
		}

		/** Virtual destructor. */
		virtual ~PromiseType() = default;


		// INTERFACE

		/** Returns the parent coroutine. */
		virtual GeneratorType get_return_object() {
			return { coroutine::coroutine_handle<GeneratorType::promise_type>::from_promise(*this) };
		}

		/** Called when the coroutine first starts. */
		virtual coroutine::suspend_never initial_suspend() {
			return {};
		}

		/** Called when the coroutine is about to end. */
		virtual coroutine::suspend_always final_suspend() noexcept {
			return {};
		}

		/** Called when the coroutine yields a new value. */
		virtual coroutine::suspend_always yield_value(const ReturnType& Value) {
			this->Value.Set(Value);
			return {};
		}

		/** Called when the coroutine returns its value. */
		virtual void return_value(const ReturnType& Value) {
			this->Value.Set(Value);
		}

		/** Called when the coroutine throws an exception. */
		virtual void unhandled_exception() {
			std::terminate();
		}
	};

	/** An interface that allows a function to be paused and be resumed later. */
	template<typename GeneratorType>
	struct PromiseType<GeneratorType, void> {

		// DESTRUCTOR

		/** Virtual destructor. */
		virtual ~PromiseType() = default;


		// INTERFACE

		/** Returns the parent coroutine. */
		virtual GeneratorType get_return_object() {
			return { coroutine::coroutine_handle<GeneratorType::promise_type>::from_promise(*this) };
		}

		/** Called when the coroutine first starts. */
		virtual coroutine::suspend_never initial_suspend() {
			return {};
		}

		/** Called when the coroutine is about to end. */
		virtual coroutine::suspend_always final_suspend() noexcept {
			return {};
		}

		/** Called when the coroutine yields. */
		virtual coroutine::suspend_always yield_value(std::nullptr_t) {
			return {};
		}

		/** Called when the coroutine returns. */
		virtual void return_void() {
		}

		/** Called when the coroutine throws an exception. */
		virtual void unhandled_exception() {
			std::terminate();
		}
	};


	// ASYNC TASK

	/* An interface that allows a coroutine to be suspended until the task is completed when awaited. */
	template<typename ReturnType, typename Predicate = bool>
	struct Task {

		// DATA

		/** The value representing the result of this task. */
		ReturnType Value;

		/** A flag representing whether the task is complete and therefore when the coroutine should continue. */
		std::atomic<Predicate> Complete;


		// CONSTRUCTORS

		/** Default constructor. */
		Task() : Complete(false), Value() {
		}

		/** Flag constructor. */
		Task(std::atomic<Predicate>*& OutFlag, ReturnType*& OutValue) : Complete(false), Value() {
			OutFlag = &Complete;
			OutValue = &Value;
		}

		/** Delete copy constructor. */
		Task(const Task&) = delete;

		/** Delete move constructor. */
		Task(Task&&) noexcept = delete;


		// OPERATORS

		/** Delete copy assignment operator. */
		Task& operator=(const Task&) = delete;

		/** Delete move assignment operator. */
		Task& operator=(Task&&) noexcept = delete;


		// INTERFACE

		/** Returns whether the task should not start. */
		virtual bool await_ready() {
			return false;
		}

		/** Called when the task is awaited, blocking execution until it completes. */
		virtual void await_suspend(coroutine::coroutine_handle<> Coroutine) {
			if (!Coroutine) {
				return;
			}
			std::thread([this, Coroutine] {
				while (!Complete.load()) {
					std::this_thread::yield();
				}
				if (Coroutine) {
					Coroutine.resume();
				}
				}).detach();
		}

		/** Called when the task is completed and returns its value. */
		virtual ReturnType await_resume() {
			return Value;
		}
	};

	/* An interface that allows a coroutine to be suspended until the task is completed when awaited. */
	template<typename Predicate>
	struct Task<void, Predicate> {

		// DATA

		/** A flag representing whether the task is complete and therefore when the coroutine should continue. */
		std::atomic<Predicate> Complete;


		// CONSTRUCTORS

		/** Default constructor. */
		Task() : Complete(false) {
		}

		/** Flag constructor. */
		Task(std::atomic<Predicate>*& OutFlag) : Complete(false) {
			OutFlag = &Complete;
		}

		/** Delete copy constructor. */
		Task(const Task&) = delete;

		/** Delete move constructor. */
		Task(Task&&) noexcept = delete;


		// OPERATORS

		/** Delete copy assignment operator. */
		Task& operator=(const Task&) = delete;

		/** Delete move assignment operator. */
		Task& operator=(Task&&) noexcept = delete;


		// INTERFACE

		/** Returns whether the task should not start. */
		virtual bool await_ready() {
			return false;
		}

		/** Called when the task is awaited, blocking execution until it completes. */
		virtual void await_suspend(coroutine::coroutine_handle<> Coroutine) {
			if (!Coroutine) {
				return;
			}
			std::thread([this, Coroutine] {
				while (!Complete.load()) {
					std::this_thread::yield();
				}
				if (Coroutine) {
					Coroutine.resume();
				}
				}).detach();
		}

		/** Called when the task is completed. */
		virtual void await_resume() {
		}
	};


	// DELAY

	/** A task that delays the current coroutine by the given number of milliseconds when awaited. */
	struct Delay final : public Task<void> {
	private:

		// DATA

		/** The number of milliseconds delayed. */
		size_t milliseconds;

	public:

		// CONSTRUCTOR

		/** Default constructor. */
		Delay(const size_t Milliseconds) : milliseconds(Milliseconds) {
		}


		// TASK

		/** Called when the task is awaited, blocking execution until it completes. */
		void await_suspend(coroutine::coroutine_handle<> Coroutine) override final {
			if (!Coroutine) {
				return;
			}
			std::thread([this, Coroutine] {
				std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
				if (Coroutine) {
					Coroutine.resume();
				}
				}).detach();
		}
	};


	// ASYNC GENERATOR

	/** Allows a function to be paused and resumed later when set as the return type. */
	template<typename Type>
	class Async final {
	public:

		// COROUTINE TYPES

		/** This async generator's promise type. */
		using promise_type = PromiseType<Async, Type>;

		/** The handle used to control this generator. */
		using handle_type = coroutine::coroutine_handle<promise_type>;

	private:

		// DATA

		/** A handle to the underlying coroutine. */
		handle_type coroutine;

	public:

		// CONSTRUCTOR AND DESTRUCTOR

		/** Default constructor. */
		Async(const handle_type& Coroutine = handle_type()) : coroutine(Coroutine) {
		}

		/** Delete copy constructor. */
		Async(const Async&) = delete;

		/** Move constructor. */
		Async(Async&& Moved) noexcept : coroutine(Moved.coroutine) {
			Moved.coroutine = {};
		}

		/** Destructor. */
		~Async() {
			if (coroutine) {
				coroutine.destroy();
			}
		}


		// OPERATORS

		/** Delete copy assignment operator. */
		Async& operator=(const Async&) = delete;

		/** Move assignment operator. */
		Async& operator=(Async&& Moved) noexcept {
			if (this == &Moved) {
				return *this;
			}
			coroutine = Moved.coroutine;
			Moved.coroutine = {};
			return *this;
		}

		/** Returns whether the coroutine is not complete. */
		explicit operator bool() const {
			return !IsComplete();
		}

		/** Returns whether the coroutine is complete. */
		bool operator!() const {
			return IsComplete();
		}


		// COROUTINE

		/** Continues the coroutine and returns if the coroutine successfully resumed. */
		bool Resume() {
			if (!coroutine) {
				return false;
			}
			if (coroutine.done()) {
				return false;
			}
			coroutine.resume();
			return true;
		}

		/** Returns the async generator's current value and sets it to null. */
		Type Get() {
			return coroutine.promise().Value.Release();
		}

		/** Continues the coroutine and returns a reference to this async generator's new value when it is ready. */
		Type Next() {
			if (!Resume()) {
				throw std::runtime_error("ERROR: Attempted to fetch the next value of a completed coroutine!");
			}
			while (!IsValueReady()) {
				std::this_thread::yield();
			}
			return Get();
		}

		/** Returns whether the coroutine successfully completed. */
		bool IsComplete() const {
			if (!coroutine) {
				return true;
			}
			return coroutine.done();
		}

		/** Returns whether the async generator's current value is ready. */
		bool IsValueReady() const {
			if (!coroutine) {
				return false;
			}
			return coroutine.promise().Value.IsValid();
		}
	};

	/** Allows a function to be paused and resumed later when set as the return type. */
	template<>
	class Async<void> final {
	public:

		// COROUTINE TYPES

		/** This async generator's promise type. */
		using promise_type = PromiseType<Async, void>;

		/** The handle used to control this generator. */
		using handle_type = coroutine::coroutine_handle<promise_type>;

	private:

		// DATA

		/** A handle to the underlying coroutine. */
		handle_type coroutine;

	public:

		// CONSTRUCTOR AND DESTRUCTOR

		/** Default constructor. */
		Async(const handle_type& Coroutine = handle_type()) : coroutine(Coroutine) {
		}

		/** Delete copy constructor. */
		Async(const Async&) = delete;

		/** Move constructor. */
		Async(Async&& Moved) noexcept : coroutine(Moved.coroutine) {
			Moved.coroutine = {};
		}

		/** Destructor. */
		~Async() {
			if (coroutine) {
				coroutine.destroy();
			}
		}


		// OPERATORS

		/** Delete copy assignment operator. */
		Async& operator=(const Async&) = delete;

		/** Move assignment operator. */
		Async& operator=(Async&& Moved) noexcept {
			if (this == &Moved) {
				return *this;
			}
			coroutine = Moved.coroutine;
			Moved.coroutine = {};
			return *this;
		}

		/** Returns whether the coroutine is not complete. */
		explicit operator bool() const {
			return !IsComplete();
		}

		/** Returns whether the coroutine is complete. */
		bool operator!() const {
			return IsComplete();
		}


		// COROUTINE

		/** Continues the coroutine and returns if the coroutine successfully resumed. */
		bool Resume() {
			if (!coroutine) {
				return false;
			}
			if (coroutine.done()) {
				return false;
			}
			coroutine.resume();
			return true;
		}

		/** Returns whether the coroutine successfully completed. */
		bool IsComplete() const {
			if (!coroutine) {
				return true;
			}
			return coroutine.done();
		}
	};
}

#endif	// COROUTINES_COMPILED
