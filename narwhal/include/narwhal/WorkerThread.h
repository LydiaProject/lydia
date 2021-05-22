#include <condition_variable>
#include <deque>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>

namespace narwhal {

	/**
	 * A simple, easy to understand & work with worker thread.
	 *
	 * \tparam BaseData 	The *base* data type you are intending to use.
	 *		 	Other data types intended to be processed should
	 *			inherit from BaseData and be checked for when 
	 *			processing.
	 */
	template <class BaseData>
	struct WorkerThread : public std::enable_shared_from_this<WorkerThread<BaseData>> {
		/**
		 * Constructor. Takes a lambda or std::bind() result.
		 * 
		 * \param[in] p Processor function object.
		 */
		template <class Processor>
		inline WorkerThread(Processor&& p)
			: processor(std::move(p)) {
		}

		~WorkerThread() {
			Stop();

			// Join the thread if it hadn't already exited or something
			if(thread.joinable())
				thread.join();
		}

		/**
		 * Start running the worker thread.
		 * Can only be called if it's not running.
		 */
		void Run() {
			if(!should_run.load()) {
				should_run.store(true);
				thread = std::thread(&WorkerThread::ThreadEntry, this->shared_from_this());
			}
		}

		/**
		 * Notifies the worker thread that it should stop.
		 *
		 * Unprocessed data will be dealt with once destructed.
		 */
		void Stop() {
			if(should_run.load())
				should_run.store(false);
		}

		/**
		 * Push data to the worker thread.
		 * Notifies the worker thread.
		 *
		 * \param[in] data Data to push into.
	`	 */
		void PushData(std::shared_ptr<BaseData>&& data) {
			std::unique_lock<std::mutex> lk(mutex);
			data_queue.push_back(data);
			cond.notify_one();
		}

	   private:
		/**
		 * Thread entry point.
		 */
		void ThreadEntry() {
			while(true) {
				if(!should_run.load())
					break;

				auto self = this->shared_from_this();

				while(data_queue.empty()) {
					std::unique_lock<std::mutex> lock(mutex);
					cond.wait(lock, [&self] {
						// Continue waiting if we somehow got here when data is empty.
						return !self->data_queue.empty();
					});
				}

				std::shared_ptr<BaseData> data = data_queue.front();
				data_queue.pop_front();

				if(!processor(data))
					Stop();
			}
		}

		/**
		 * The magic thread. 
		 * OK, in all seriousness this thread runs the worker.
		 */
		std::thread thread;

		/**
		 * A atomic flag dictating if the thread
		 * should be running. 
		 *
		 * It is true if the above thread should be running, 
		 * and false if it should not be.
		 */
		std::atomic_bool should_run;

		/**
		 * The type erased processor function object.
		 */
		std::function<bool(std::shared_ptr<BaseData>&&)> processor;

		// TODO: We might need more mutexes?

		std::mutex mutex;

		std::condition_variable cond;

		/**
		 * Data queue.
		 */
		std::deque<std::shared_ptr<BaseData>> data_queue;
	};

} // namespace narwhal
