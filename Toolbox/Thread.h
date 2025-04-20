// .h
// Thread Type
// by Kyle Furey

#pragma once
#include <utility>
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>

// A thread exit code indicating a thread did not successfully complete its execution.
#define THREAD_INCOMPLETE -1

// Shorthand for declaring a thread lambda that stores its owning thread as "ThisThread" and returns an exit code within its body.
#define THREAD_LAMBDA(Captures) [Captures](const Toolbox::Thread& ThisThread) -> int

// Shorthand for checking whether a thread was cancelled, and exits with THREAD_INCOMPLETE if it was.
#define CHECK_THREAD if (ThisThread.IsCancelled()) return THREAD_INCOMPLETE;

/** A collection of useful template types in C++. */
namespace Toolbox {

	// MUTEX

	/** A shared object used to synchronize operations between multiple threads. */
	using Mutex = std::mutex;


	// ATOMIC

	/** A wrapper for a value whose read and write ability is mediated for thread safety. */
	template<typename Type>
	using Atomic = std::atomic<Type>;


	// THREAD

	/** A handle for a new asynchronous thread of execution. */
	class Thread final {
	public:

		// THREAD ID

		/** A unique comparable ID to represent a thread. */
		using ThreadID = std::thread::id;

	private:

		// DATA

		/** The exit code of this thread. */
		Atomic<int> code;

		/** Whether this thread has started its execution. */
		Atomic<bool> started;

		/** Whether this thread was requested to cancel its execution. */
		Atomic<bool> cancelled;

		/** Whether this thread has successfully completed its execution. */
		Atomic<bool> complete;

		/** The underlying thread object. */
		std::thread thread;

	public:

		// CONSTRUCTORS AND DESTRUCTOR

		/** Default constructor. */
		Thread() : code(THREAD_INCOMPLETE), started(false), cancelled(false), complete(false), thread() {
		}

		/** Async thread constructor. */
		template<typename Lambda>
		Thread(const Lambda& Execution) : code(THREAD_INCOMPLETE), started(false), cancelled(false), complete(false), thread([this, Execution]() {
			while (!started.load()) { std::this_thread::yield(); }
			if (cancelled.load()) { return; }
			code.store(Execution(*this));
			complete.store(!cancelled.load());
			}) {
		}

		/** Delete copy constructor. */
		Thread(const Thread&) = delete;

		/** Delete move constructor. */
		Thread(Thread&&) noexcept = delete;

		/** Destructor. */
		~Thread() {
			if (thread.joinable()) {
				started.store(true);
				cancelled.store(true);
				thread.join();
			}
		}


		// OPERATORS

		/** Delete copy assignment operator. */
		Thread& operator=(const Thread&) = delete;

		/** Delete move assignment operator. */
		Thread& operator=(Thread&&) noexcept = delete;


		// THREADING

		/** Starts this thread asynchronously and returns whether it was successfully started. */
		bool Run() {
			if (!started.load()) {
				started.store(true);
				thread.detach();
				return true;
			}
			return false;
		}

		/** Attempts to cancel this thread and returns whether the cancellation request was successful. */
		bool Cancel() {
			if (cancelled.load() || complete.load()) {
				return false;
			}
			cancelled.store(true);
			return true;
		}

		/**
		 * Waits until this thread completes or cancels its execution before continuing the current thread.<br/>
		 * Returns whether the thread was successfully joined.
		 */
		bool Join() {
			if (cancelled.load() || complete.load()) {
				return false;
			}
			if (!started.load()) {
				started.store(true);
				thread.join();
				return true;
			}
			while (!cancelled.load() && !complete.load()) {
				std::this_thread::yield();
			}
			return true;
		}

		/** Returns this thread's ID. */
		ThreadID ID() const {
			return thread.get_id();
		}

		/** Returns the exit code of this thread. */
		int ExitCode() const {
			return code.load();
		}

		/** Returns whether this thread has started its execution. */
		bool IsStarted() const {
			return started.load();
		}

		/** Returns whether this thread was requested to cancel its execution. */
		bool IsCancelled() const {
			return cancelled.load();
		}

		/** Returns whether this thread has successfully completed its execution. */
		bool IsComplete() const {
			return !cancelled.load() && complete.load();
		}

		/** Returns an estimate of this maximum number of concurrent threads. */
		static size_t MaxThreads() {
			return std::thread::hardware_concurrency();
		}

		/** Yields the current thread so other threads may continue. */
		static void Yield() {
			std::this_thread::yield();
		}

		/** Sleeps the current thread for the given number of milliseconds. */
		static void Sleep(const size_t Milliseconds) {
			std::this_thread::sleep_for(std::chrono::milliseconds(Milliseconds));
		}

		/** Returns the ID of the current thread. */
		static ThreadID CurrentID() {
			return std::this_thread::get_id();
		}
	};
}
