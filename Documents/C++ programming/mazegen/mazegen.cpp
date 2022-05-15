#include <iostream>
#include <fstream>
#include <stack>
#include <list>
#include <vector>
#include <stdio.h>
#include <string>

#define cell_size 100
#define map_size 2*100+1

class mazeCell
{
	public:
		int x;
		int y;
		mazeCell(int xPos = -1, int yPos = -1)
		{
			x = xPos;
			y = yPos;
		}
};


bool isEven(int x){
	return x % 2 == 0 ? true : false;
}

bool sameCell(mazeCell cell1, mazeCell cell2)
{
	if(cell1.x == cell2.x && cell1.y == cell2.y)
	{
		return true;
	}
	return false;
}

bool stackContainsCell(mazeCell cell, std::stack<mazeCell> stack)
{
	std::stack<mazeCell> stack_to_check = stack;
	while(stack.size() > 0)
	{
		if(sameCell(stack.top(), cell))
		{
			return true;
		}
		stack.pop();
	}
	return false;
}

void removeLoop(mazeCell cell_to_return_to, std::stack<mazeCell>& stack, std::vector<mazeCell>& unvisited_cells)
{
	while(!sameCell(stack.top(), cell_to_return_to))
	{
		unvisited_cells.insert(unvisited_cells.begin(), stack.top());
		stack.pop();
	}
}

int cellCoordToMazeCoord(int coord)
{
	return (2*coord) + 1;
}

void eraseCell(std::vector<mazeCell>& vector, mazeCell cell)
{
	for(int i = 0; i < vector.size(); i++)
	{
		if(sameCell(cell, vector[i]))
		{
			vector.erase(std::next(vector.begin(), i));
			return;
		}
	}
}

void renderMap(int map[map_size][map_size], int number)
{
	std::ofstream maze;
	maze.open("./mazes/maze" + std::to_string(number) + ".pbm");
	maze << "P1\n\n";
	maze << map_size << " " << map_size << "\n\n";
	for(int x = 0; x < map_size; x++)
	{
		for(int y = 0; y < map_size; y++)
		{
			if(map[x][y] == 1){
				maze << 0;
			} else 
			{
				maze << 1;
			}
		}
		maze << "\n";
	}
	maze.close();
}

int main()
{
	for(int n = 0; n < 1000; n++)
	{
		int map[map_size][map_size] = {0};
		std::vector<mazeCell> unvisited_cells;
		std::stack<mazeCell> visited_cells;
		std::stack<mazeCell> cellsPartOfMaze;

		for(int x = 0; x < map_size; x++){
			for(int y = 0; y < map_size; y++){
				if(!isEven(x) && !isEven(y))
				{
					mazeCell cell_to_add = mazeCell((x - 1)/2,(y-1)/2);
					unvisited_cells.push_back(cell_to_add);
				}
			}
		}

		mazeCell start = mazeCell(0,0);
		mazeCell goal = start;

		while(sameCell(goal, start))
		{
			goal = mazeCell(rand() % cell_size,rand() % cell_size);
		}

		mazeCell current = start;
		visited_cells.push(current);
		eraseCell(unvisited_cells, current);

		while(!sameCell(goal, current))
		{
			mazeCell next;
			while((next.x < 0 || next.x > cell_size - 1) || (next.y < 0 || next.y > cell_size - 1) || sameCell(next,visited_cells.top()))
			{
				next = current;
				int randomDir = rand() % 4;
				if(randomDir == 0)
				{
					next.x += 1;
				} else if(randomDir == 1)
				{
					next.y += 1;
				} else if(randomDir == 2)
				{
					next.x -= 1;
				} else if(randomDir == 3)
				{
					next.y -= 1;
				}
			}
			if(stackContainsCell(next, visited_cells))
			{
				removeLoop(next, visited_cells, unvisited_cells);
			} else
			{
				visited_cells.push(current);
				eraseCell(unvisited_cells, current);

			}
			current = next;
		}
		visited_cells.push(current);
		eraseCell(unvisited_cells, current);

		while(visited_cells.size() > 0)
		{
			map[cellCoordToMazeCoord(visited_cells.top().x)][cellCoordToMazeCoord(visited_cells.top().y)] = 1;
			cellsPartOfMaze.push(visited_cells.top());
			current = visited_cells.top();
			cellsPartOfMaze.push(current);
			visited_cells.pop();
			if(!visited_cells.size() == 0)
			{
				int xOffset = visited_cells.top().x - current.x;
				int yOffset = visited_cells.top().y - current.y;
				map[cellCoordToMazeCoord(current.x) + xOffset][cellCoordToMazeCoord(current.y) + yOffset] = 1;
			}
		}

		while(unvisited_cells.size() > 0)
		{
			current = unvisited_cells[0];
			visited_cells.push(current);
			eraseCell(unvisited_cells, current);

			while(!stackContainsCell(current, cellsPartOfMaze))
			{
				mazeCell next;
				while((next.x < 0 || next.x > cell_size - 1) || (next.y < 0 || next.y > cell_size - 1) || sameCell(next,visited_cells.top()))
				{
					next = current;
					int randomDir = rand() % 4;
					if(randomDir == 0)
					{
						next.x += 1;
					} else if(randomDir == 1)
					{
						next.y += 1;
					} else if(randomDir == 2)
					{
						next.x -= 1;
					} else if(randomDir == 3)
					{
						next.y -= 1;
					}
				}
				if(stackContainsCell(next, visited_cells))
				{
					removeLoop(next, visited_cells, unvisited_cells);
				} else
				{
					visited_cells.push(current);
					eraseCell(unvisited_cells, current);

				}
				current = next;
			}
			visited_cells.push(current);
			eraseCell(unvisited_cells, current);

			while(visited_cells.size() > 0)
			{
				map[cellCoordToMazeCoord(visited_cells.top().x)][cellCoordToMazeCoord(visited_cells.top().y)] = 1;
				cellsPartOfMaze.push(visited_cells.top());
				current = visited_cells.top();
				cellsPartOfMaze.push(current);
				visited_cells.pop();
				if(!visited_cells.size() == 0)
				{
					int xOffset = visited_cells.top().x - current.x;
					int yOffset = visited_cells.top().y - current.y;
					map[cellCoordToMazeCoord(current.x) + xOffset][cellCoordToMazeCoord(current.y) + yOffset] = 1;
				}
			}
		}

		renderMap(map, n);
	}

	return 0;
}
