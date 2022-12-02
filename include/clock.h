#include <chrono>

using namespace std;

class Timer
{
	public:

		void tick() {
			start = chrono::steady_clock::now();
		}

		int64_t tock() {
			stop = chrono::steady_clock::now();
			return chrono::duration_cast<chrono::microseconds>(stop - start).count();
		}

	private:
		chrono::steady_clock::time_point start;
		chrono::steady_clock::time_point stop;

};