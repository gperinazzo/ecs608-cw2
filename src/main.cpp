/*
 * Copyright (c) 2015, Alexandre Medeiros and Guilherme Perinazzo
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "matrix.h"
#include <iostream>
#include <stack>
#include <vector>
#include <cstdlib>
#include <ctime>

// Calculate the transitive closure of a adjacency matrix
// T must be comparable to 1
template <typename T>
void transitiveClosure(Matrix<T> & matrix)
{
	std::stack<int> queue;

	for (int i = 0; i < matrix.x; ++i)
	{
		// Run a DFS algorithm to find all possible connected nodes
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

// Basic iterative depth first search on a adjacency 
// matrix, returns true if there is a path from init 
// to target, return false otherwise
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

	if (argc < 4)
	{
		std::cout << "Usage: cw2 <number of events> <number of threads> <number of connections>" << std::endl;
		return 0;
	}

	int num_events = atoi(argv[1]);
	int num_threads = atoi(argv[2]);
	int num_connections = atoi(argv[3]);

	Matrix<bool> mat(num_events, num_events, false);
	
	std::vector<bool> connected(num_events, false);

	std::vector<int> lastEvent(num_threads, -1);
	std::vector<int> eventThread(num_events, 0);
	int totalConnected = 0;

	int totalLess = 0;
	int totalUnknown = 0;


	std::srand (time(NULL));

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

	// Generate the connections
	for (int i = 0; i < num_connections && totalConnected < num_events - 1; ++i)
	{
		int sender, reciever;

		// Find a random event to start the connection
		sender = std::rand() % num_events;

		// We don't want the starting event to be already engaged in a connection
		while(connected[sender])
			sender = (sender + 1) % num_events;

		// Mark all events that can't receive the connection
		// An event can't recieve a connection if it's already engaged in another connection,
		// or if it's in the same thread as the starting event
		std::vector<bool> explored(num_events, false);
		for (int j = 0; j < num_events; ++j)
			if (eventThread[j] == eventThread[sender] || connected[j]) explored[j] = true;

		// Find a random event to recieve the connection
		reciever = std::rand() % num_events;

		// The events are checked linearly starting at the reciever event,
		// so that we make any possible connection with the starting event
		// This was choosen over a random selection every time to speedup
		// the process when the number of connected events approach the total
		// number of events
		for (int j = reciever; j < reciever + num_events; ++j)
		{
			int cur = j % num_events;
			if (!explored[cur])
			{
				explored[cur] = true;
				
				// To avoid going back in time, we treat the events as a directed graph, where
				// an adjacent node is an event that happens after. Our matrix is the adjacency matrix
				// of the graph.
				// To check if we're going back in time, we need to check if a edge between nodes
				// (x, y) will generate a cycle in the graph. To do that, before adding the edge
				// to the graph, we check if there's a path from y that leads back to x. If there
				// is no such path, the edge can be safely added
				if (!DFS(mat, cur, sender))
				{
					// Add the edge to the adjacency matrix
					mat[sender][cur] = true;

					// Count the number of connected nodes as an alternative stop condition
					totalConnected += 2;
					connected[sender] = true;
					connected[cur] = true;
					break;
				}
			}
		}
	}

	// Find the transitive closure of our adjacency matrix
	transitiveClosure(mat);

	// Compute the number os < and || relations
	for (int i = 0; i < num_events; ++i)
		for (int j = i + 1; j < num_events; ++j)
			if ((mat[i][j] || mat[j][i]) && i != j) totalLess++;
			else if (i != j) totalUnknown++;

	// Print the results
	std::cout << totalLess << " " << totalUnknown << std::endl;
}