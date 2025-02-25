# Algorithm Implementations Repository

This repository contains various algorithm implementations and performance evaluations for a range of topics in computer science. Each project showcases different techniques, such as tree traversals, graph algorithms, sorting methods, dynamic ordered statistics, and more. All algorithms are implemented with a focus on clarity, efficiency, and performance analysis.

## Projects

### 1. **Breadth-First Search (BFS) Implementation**
   - **Description:** Implemented the BFS algorithm on a graph, allowing traversal starting from a specific source node. The algorithm explores nodes level by level.
   - **Graph Representation:** Uses an adjacency list to represent the graph, where each node maintains a list of neighbors.
   - **Queue Mechanism:** BFS is implemented with a queue to ensure level-wise exploration of nodes.
   - **Node Properties:** Each node tracks its distance from the source, its parent in the BFS tree, and its exploration state (color).
   - **Operations Counting:** An operation counter tracks the steps taken during BFS, useful for performance analysis.
   - **Path Calculation:** Computes the shortest path between two nodes and stores it in reverse order for accurate path presentation.
   - **Performance Testing:** The BFS implementation is tested on various graph sizes and edge counts, providing performance analysis for scalability.

### 2. **Binary Tree Traversal**
   - **Description:** Implemented both recursive and iterative methods for binary tree traversal.
   - **Recursive Traversal:** Explores the left-most branch and then moves to the right one.
   - **Iterative Traversal:** Temporarily alters the tree structure by linking the rightmost node of the left subtree to its predecessor. The traversal continues once the link is removed.
   - **Complexity:** Both methods have a time complexity of O(n).
   - **Performance Analysis:** Randomly built trees for more accurate results and compared the time performance between both methods.

### 3. **DFS Algorithm**
   - **Description:** Implements Depth-First Search (DFS) along with Topological Sort and Tarjan’s Algorithm.
   - **DFS:** Starts at an initial node and explores as deeply as possible, marking nodes as visited.
   - **Topological Sort:** Linear ordering of nodes in a directed acyclic graph based on dependencies.
   - **Tarjan’s Algorithm:** Identifies strongly connected components in a directed graph.
   - **Data Structures:** Uses discovery and finish times, as well as node colors (white, grey, black).

### 4. **Direct Sorting Methods**
   - **Description:** Implements three types of direct sorting algorithms: Insertion Sort, Selection Sort, and Bubble Sort.
   - **Insertion Sort:** Divides the array into sorted and unsorted parts and places each element in its appropriate position in the sorted part.
   - **Selection Sort:** Repeatedly selects the smallest element and moves it to the sorted part of the array.
   - **Bubble Sort:** Efficiently sorts by comparing adjacent elements and swapping them if necessary.
   - **Performance Analysis:** Analyzed Best, Average, and Worst case scenarios with time complexity comparisons for each method.

### 5. **Disjoint Sets**
   - **Description:** Implements operations for disjoint sets, including `Make_Set`, `Find_Set`, and `Union`.
   - **Union-Find:** Uses rank-based union to merge two sets and path compression to optimize find operations.
   - **Kruskal's Algorithm:** Uses union-find to find the minimum spanning tree of a graph by sorting edges and adding them without forming cycles.
   - **Performance Analysis:** Random edges and adjacency matrix used to ensure performance accuracy.

### 6. **Dynamic Ordered Statistics**
   - **Description:** Implements operations on augmented binary search trees to maintain dynamic ordered statistics.
   - **Operations:** Build Tree, OS_Select (find the ith smallest element), OS_Delete (delete element based on its rank).
   - **Complexity:** `BuildTree` is O(n), and both `OS_Select` and `OS_Delete` are O(h) where h is the height of the tree.
   - **Performance Analysis:** Performed experiments with various tree sizes to measure the efficiency of these operations.

### 7. **Hashtables**
   - **Description:** Implements basic hash table operations: `INSERT`, `SEARCH`, and `DELETE` with quadratic hashing.
   - **Collision Handling:** Handles collisions by recalculating hash values using a counter.
   - **Performance Evaluation:** Measures the number of recalculations for `SEARCH` and `DELETE` operations and analyzes the filling factor impact (ranging from 0.80 to 0.99).

### 8. **Heap Building Methods**
   - **Description:** Implements two methods of building a heap: `bottom-up` and `top-down`, and a heap sort algorithm.
   - **Bottom-Up:** Uses the `sink` operation to ensure heap properties are maintained while building the heap.
   - **Top-Down:** Uses the `swim` operation to maintain heap properties as elements are added.
   - **Heap Sort:** Builds a heap and then repeatedly extracts the maximum element and places it at the end of the array.
   - **Performance Analysis:** Compares the efficiency of both methods in Best and Worst case scenarios.

### 9. **Hybrid QuickSort**
   - **Description:** Implements a hybrid sorting algorithm that combines QuickSort and InsertionSort.
   - **QuickSort:** Partitions the array around a pivot element and recursively sorts the subarrays.
   - **InsertionSort:** Used for smaller subarrays (based on an experimentally determined threshold) to improve efficiency.
   - **Performance Analysis:** Observes and measures the performance based on the chosen threshold for switching between the two algorithms.

### 10. **Merging k Lists**
   - **Description:** Merges k sorted lists into one sorted list using a min-heap for efficient merging.
   - **Approach:** Uses a heap to keep track of the smallest element from each list and extract it in order.
   - **Performance Analysis:** Measures the complexity based on the total number of elements (n) and the number of lists (k), with time complexity of O(n log k).

### 11. **Multi-way Trees**
   - **Description:** Implements three different methods for representing multi-way trees and transitioning between them.
   - **Representations:** 
     - Simple array-based representation.
     - Node structure with children and number of children.
     - Binary tree representation with pointers for children and siblings.
   - **Pretty Print:** Recursively prints the tree structure for each representation.

### 12. **QuickSort vs HeapSort**
   - **Description:** Compares QuickSort and HeapSort algorithms in terms of performance and efficiency.
   - **HeapSort:** Builds a heap using `heapify` and `build_heap`, then sorts the array by repeatedly extracting the maximum element.
   - **QuickSort:** Recursively sorts the array by partitioning around a pivot.
   - **Performance Comparison:** Compares the time and number of operations for both algorithms.

### 13. **Selection Sort: Iterative vs Recursive**
   - **Description:** In this program, I've implemented the Selection Sort algorithm in two ways: iteratively and recursively.
   - **Iterative Version:** In the iterative version, a loop is used to find the minimum element in the unsorted part of the array and place it at the end of the sorted section.
   - **Recursive Version:** The recursive version operates similarly, but instead of using a loop, it calls the sorting function recursively to select and place the minimum element in the sorted section.
   - **Performance Comparison:** I performed experiments to compare the performance of the two approaches (iterative and recursive) under best, average and worst conditions. 

---

## Performance Analysis

For most of the algorithms, extensive performance analysis has been conducted, comparing the efficiency across various cases such as Best, Average, and Worst case scenarios. Graphs and data plots are included to visualize the performance under different conditions.

---
