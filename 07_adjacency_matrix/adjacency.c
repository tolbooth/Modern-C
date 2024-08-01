#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "lin-alg.h"

const size_t total_nodes = 5;
const size_t capacity = 10;

typedef struct QueueStruct {
	int arr[capacity];
	int rear_index;
} Queue;

void enqueue(Queue* queue, int new_node);

int dequeue(Queue* queue);

int unvisited(int visited[]);

int min_index(int arr[], int len, int visited[]);

void printAdjMatrix(int Adj[total_nodes][total_nodes + 1]);

int breadth_first_search(int adjacency_matrix[total_nodes][total_nodes], int target_node);

void generateAdjacencyMatrix(int adjacency_matrix[total_nodes][total_nodes], size_t total_edges, int graph[total_nodes][2]);

void generateWeightedAdjacencyMatrix(int adjacency_matrix[total_nodes][total_nodes], size_t total_edges, int graph[total_nodes][3]);

void shortestPath(int weighted_adjacency_matrix[total_nodes][total_nodes], int start_node, int target_node);

int main(void) {
	int graph_with_node[][2] = {{0,2}, {0,4}, {4,3}, {1,3}, {1,4}};
	int graph_without_node[][2] = {{0,2}, {0,4}, {4,3}, {2,3}, {2,4}};
	int weighted_graph[][3] = {{0,2,1}, {0,4,3}, {4,3,4}, {1,3,2}, {1,4,1}};
	size_t total_edges = 5;

	int adjacency_matrix_true[total_nodes][total_nodes] = { 0 };
	int adjacency_matrix_false[total_nodes][total_nodes] = { 0 };
	int weighted_adjacency_matrix[total_nodes][total_nodes] = { 0 };

	generateAdjacencyMatrix(adjacency_matrix_true, total_edges, graph_with_node);
	generateAdjacencyMatrix(adjacency_matrix_false, total_edges, graph_without_node);
	generateWeightedAdjacencyMatrix(weighted_adjacency_matrix, total_edges, weighted_graph);

	int target_node = 1;
	printAdjMatrix(adjacency_matrix_true);
	printf("Node %d in good graph: %s.\n", target_node, breadth_first_search(adjacency_matrix_true, target_node) ? "true" : "false");
	printf("---------\n");
	printAdjMatrix(adjacency_matrix_false);
	printf("Node %d in bad graph: %s.\n", target_node, breadth_first_search(adjacency_matrix_false, target_node) ? "true" : "false");
	printf("---------\n");
	printAdjMatrix(weighted_adjacency_matrix);
	printf("The shorted path from node %d to %d is: ", 0, 3);
	shortestPath(weighted_adjacency_matrix, 0, 3);

	return EXIT_SUCCESS;
}

int breadth_first_search(int adjacency_matrix[total_nodes][total_nodes], int target_node) {
	int visited[total_nodes] = { 0 };
	int current_node;

	Queue queue = { { 0 }, 0 };
	enqueue(&queue, 0);

	while(queue.rear_index != 0) {
		current_node = dequeue(&queue);
		for (size_t i = 0; i < total_nodes; i++) {
			if (visited[i] || i == current_node) {
				visited[i] = 1;
				continue;
			} else if (adjacency_matrix[current_node][i]) {
				if (i == target_node) {
					return 1;
				}
				enqueue(&queue, i);
				visited[i] = 1;
			}
		}
	}
	return 0;
}

void shortestPath(int weighted_adjacency_matrix[total_nodes][total_nodes], int start_node, int target_node) {
	int visited[total_nodes] = { 0 };
	int distance_from_start[total_nodes] = { 0 };
	int previous_node[total_nodes] = { 0 };

	for (size_t i = 0; i < total_nodes; ++i) {
		if (i == start_node)
			continue;
		distance_from_start[i] = INT_MAX;
	}

	int current_node = -1;
	while(unvisited(visited)) {
		current_node = min_index(distance_from_start, total_nodes, visited);
		for (size_t i = 0; i < total_nodes; ++i) {
			int neighbour_distance = weighted_adjacency_matrix[current_node][i];
			if (!visited[i] && neighbour_distance != INT_MAX) {
				int new_distance = distance_from_start[current_node] + neighbour_distance;
				if (new_distance < distance_from_start[i]) {
					distance_from_start[i] = new_distance;
					previous_node[i] = current_node;
				}
			}
		}
		visited[current_node] = 1;
	}

	while(target_node != start_node) {
		printf("%d ", target_node);
		target_node = previous_node[target_node];
	}
	printf("%d.\n", start_node);
}

int min_index(int arr[], int len, int visited[]) {
	int current_min_idx = 0;
	while(visited[current_min_idx]) {
		++current_min_idx;
	}
	for(size_t i = 0; i < len; ++i) {
		if (arr[i] <= arr[current_min_idx] && !visited[i]) {
			current_min_idx = i;
		}
	}
	return current_min_idx;
}

int unvisited(int visited[]) {
	for (size_t i = 0; i < total_nodes; ++i) {
		if (visited[i] == 0)
			return 1;
	}
	return 0;
}

void generateAdjacencyMatrix(int adjacency_matrix[total_nodes][total_nodes], size_t total_edges, int graph[total_nodes][2]) {
	for(size_t i = 0; i < total_nodes; ++i) {
		adjacency_matrix[i][i] = 1;
	}

	for (size_t i = 0; i < total_edges; ++i) {
		adjacency_matrix[graph[i][0]][graph[i][1]] = 1;
		adjacency_matrix[graph[i][1]][graph[i][0]] = 1;
	}
}

void generateWeightedAdjacencyMatrix(int adjacency_matrix[total_nodes][total_nodes], size_t total_edges, int graph[total_nodes][3]) {
	for(size_t i = 0; i < total_nodes; ++i) {
		for (size_t j = 0; j < total_nodes; ++j) {
			adjacency_matrix[i][j] = INT_MAX;
		}
	}

	for (size_t i = 0; i < total_edges; ++i) {
		adjacency_matrix[graph[i][0]][graph[i][1]] = graph[i][2];
		adjacency_matrix[graph[i][1]][graph[i][0]] = graph[i][2];
	}
}

void printAdjMatrix(int Adj[total_nodes][total_nodes]) {
    for (int i = 0; i < total_nodes; i++) {
        for (int j = 0; j < total_nodes; j++) {
            printf("%d ", Adj[i][j] == INT_MAX ? 0 : Adj[i][j]);
        }
        printf("\n");
    }
}

void enqueue(Queue* queue, int new_node) {
	if (queue->rear_index == capacity) {
		return;
	} else {
		queue->arr[queue->rear_index] = new_node;
		++(queue->rear_index);
	}
}

int dequeue(Queue* queue) {
	if (queue->rear_index == 0) {
		return -1;
	} else {
		int temp = queue->arr[0];
		for (size_t i = 0; i < queue->rear_index; ++i) {
			queue->arr[i] = queue->arr[i+1];
		}
		--queue->rear_index;
		return temp;
	}
}