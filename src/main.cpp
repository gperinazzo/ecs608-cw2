#include "matrix.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

template <typename T>
void printMatrix(Matrix<T> mat)
{
	for (int i = 0; i < mat.x; ++i)
	{
		int j;
		for (j = 0; j < mat.y - 1; ++j)
			std::cout << (int)mat[i][j] << " ";
		std::cout << (int)mat[i][j] << "\n";
	}
	std::cout << std::flush;
}

template <typename T>
void transitiveClosure(Matrix<T> & matrix)
{
	std::stack<int> queue;

	for (int i = 0; i < matrix.x; ++i)
	{
		std::vector<bool> expanded(matrix.x, false);
		queue.push(i);
		while (!queue.empty())
		{
			int cur = queue.top();
			queue.pop();
			if (!expanded[cur])
			{
				expanded[cur] = true;
				for (int j = 0; j < matrix.y; j++)
				{
					if (matrix[cur][j] == 1)
					{
						matrix[i][j] = 1;
						queue.push(j);
					}
				}
			}

		}
	}
}

template <typename T>
bool DFS(Matrix<T> & mat, int init, int target)
{
	std::stack<int> queue;
	std::vector<bool> expanded(mat.x, false);

	queue.push(init);
	while(!queue.empty())
	{
		int cur = queue.top();
		queue.pop();
		if (cur == target)
			return true;
		expanded[cur] = true;
		for (int i = 0; i < mat.y; i++)
		{
			if (mat[cur][i] == 1 && !expanded[i])
				queue.push(i);
		}
	}
	return false;
}


int main(int argc, char* argv[])
{
	int num_events = 12;
	int num_threads = 4;
	int num_connections = 3;

	Matrix<bool> mat(num_events, num_events, false);
	
	std::vector<bool> connected(num_events, false);
	std::vector<bool> hasSuccessor(num_events, false);
	std::vector<int> sucessor(num_events, true);

	std::vector<int> lastEvent(num_threads, -1);
	std::vector<int> eventThread(num_events, 0);
	int totalConnected = 0;

	int totalLess = 0;
	int totalUnknown = 0;


	std::srand (time(NULL));

	/*unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator (seed);

	std::uniform_int_distribution<int> randEvent(0, num_events);
	std::uniform_int_distribution<int> randThread(0, num_threads);
	*/

	// Distribute each event to a thread
	for (int i = 0; i < num_events; ++i)
	{
		// Get a random thread number
		int thread = std::rand() % num_threads;
		// Check the last event in the thread
		if (lastEvent[thread] != -1)
		{
			// Set our last event to be connected to the current one
			mat[lastEvent[thread]][i] = true;
		}
		lastEvent[thread] = i;
		eventThread[i] = thread;
	}

	for (int i = 0; i < num_connections && totalConnected < num_events - 1; ++i)
	{
		int sender, reciever;
		sender = std::rand() % num_events;
		while(connected[sender])
			sender = (sender + 1) % num_events;

		std::vector<bool> explored(num_events, false);
		for (int j = 0; j < num_events; ++j)
			if (eventThread[j] == eventThread[sender] || connected[j]) explored[j] = true;

		reciever = std::rand() % num_events;
		for (int j = reciever; j < reciever + num_events; ++j)
		{
			int cur = j % num_events;
			if (!explored[cur])
			{
				explored[cur] = true;
				if (!DFS(mat, cur, sender))
				{
					mat[sender][cur] = true;
					connected[sender] = true;
					connected[cur] = true;
					totalConnected += 2;
					break;
				}
			}
		}
	}

	transitiveClosure(mat);

	for (int i = 0; i < num_events; ++i)
		for (int j = i + 1; j < num_events; ++j)
			if (mat[i][j] || mat[j][i]) totalLess++;
			else totalUnknown++;

	printMatrix(mat);
	std::cout << "\n" << totalLess << " " << totalUnknown;
}