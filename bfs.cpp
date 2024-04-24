#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> createGraph(int size, int edges)
{

    vector<vector<int>> adjList(size);

    for (int i = 0; i < edges; i++)
    {
        cout << "Edge " << i + 1 << ": ";
        int u, v;
        cin >> u >> v;

        adjList[v].push_back(u);
        adjList[u].push_back(v);
    }

    return adjList;
}

vector<int> sequential_BFS(const vector<vector<int>> &adjList, int startIdx)
{

    vector<int> traversal;

    queue<int> frontier;
    vector<bool> visited(adjList.size(), false);

    frontier.push(startIdx);
    visited[startIdx] = true;

    while (!frontier.empty())
    {

        int currentIdx = frontier.front();
        frontier.pop();

        traversal.push_back(currentIdx);

        for (int neighborIdx : adjList[currentIdx])
        {
            if (!visited[neighborIdx])
            {
                visited[neighborIdx] = true;
                frontier.push(neighborIdx);
            }
        }
    }

    return traversal;
}

vector<int> parallel_BFS(const vector<vector<int>> &adjList, int startIdx)
{
    vector<int> traversal;
    queue<int> frontier;
    vector<bool> visited(adjList.size(), false);

    frontier.push(startIdx);
    visited[startIdx] = true;

#pragma omp parallel
    {

        while (true)
        {

            queue<int> localQueue;

#pragma omp critical
            {

                if (!frontier.empty())
                {
                    localQueue.push(frontier.front());
                    frontier.pop();
                }

                if (localQueue.empty())
                {
                    break;
                }

                int currentIdx = localQueue.front();
                localQueue.pop();

                traversal.push_back(currentIdx);

                for (int neighborIdx : adjList[currentIdx])
                {

                    if (!visited[neighborIdx])
                    {
#pragma omp critical
                        {

                            if (!visited[neighborIdx])
                            {
                                visited[neighborIdx] = true;
                                frontier.push(neighborIdx);
                            }
                        }
                    }
                }
            }
        }
    }

    return traversal;
}

int main()
{

    int size;
    cout << "Enter the no of nodes: " << endl;
    cin >> size;

    int edges;
    cout << "Enter the no of edges: " << endl;
    cin >> edges;

    vector<vector<int>> adjList = createGraph(size, edges);

    auto start_seq = chrono::high_resolution_clock::now();
    vector<int> traversal = sequential_BFS(adjList, 0);
    auto end_seq = chrono::high_resolution_clock::now();
    chrono::duration<double> seq_bfs_duration = end_seq - start_seq;

    cout << "BFS sequential time is: " << seq_bfs_duration.count() << endl;

    cout << "The BFS traversal using sequential: " << endl;
    for (int i = 0; i < size; i++)
    {
        cout << traversal[i] << " ";
    }

    cout << endl;

    auto start_par = chrono::high_resolution_clock::now();
    vector<int> traversal_par = parallel_BFS(adjList, 0);
    auto end_par = chrono::high_resolution_clock::now();
    chrono::duration<double> par_bfs_duration = end_par - start_par;

    cout << "BFS parallel time is: " << par_bfs_duration.count() << endl;

    cout << "The BFS traversal using paralel: " << endl;
    for (int i = 0; i < size; i++)
    {
        cout << traversal_par[i] << " ";
    }

    cout << endl;

    double speedup = seq_bfs_duration.count() / par_bfs_duration.count();
    cout << "Speedup: " << speedup << endl;

    return 0;
}