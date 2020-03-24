import numpy as np
import time
class Board():
    POINT = 0
    
    def __init__(self, width_mm=3000, height_mm=2000):
        self.width = width_mm
        self.height = height_mm
        self.surface = np.zeros((width_mm, height_mm), dtype=np.uint8)

    def add_obstacle(self, x, y):
        self.surface[y, x] = 1
    
    def size(self):
        return (self.width, self.height)

    def __getitem__(self, key):
        if isinstance(key, tuple):
            x, y = key
        if isinstance(key, int):
            x = key
            y = 0
        if x < 0 or x > self.width or y < 0 or y > self.height:
            raise ValueError("Out of range!")
        return self.surface[x, y]
    
class Node():
    """A node class for A* Pathfinding"""

    def __init__(self, parent=None, position=None):
        self.parent = parent
        self.position = position

        self.g = 0
        self.h = 0
        self.f = 0

    def __eq__(self, other):
        return self.position == other.position


def astar(maze, start, end):
    """Returns a list of tuples as a path from the given start to the given end in the given maze"""

    # Create start and end node
    start_node = Node(None, start)
    start_node.g = start_node.h = start_node.f = 0
    end_node = Node(None, end)
    end_node.g = end_node.h = end_node.f = 0
    len_board_x, len_board_y = maze.size()
    neighbours = [(0, -1), (0, 1), (-1, 0), (1, 0), (-1, -1), (-1, 1), (1, -1), (1, 1)] # Adjacent squares
    # Initialize both open and closed list
    possible_path = []
    closed_list = []

    # Add the start node
    possible_path.append(start_node)

    # Loop until you find the end
    while len(possible_path) > 0:
        current_node = possible_path[0]
        current_index = 0
        for index, item in enumerate(possible_path):
            if item.f < current_node.f:
                current_node = item
                current_index = index

        # Pop current off open list, add to closed list
        possible_path.pop(current_index)
        closed_list.append(current_node)

        # Found the goal
        if current_node == end_node:
            path = []
            current = current_node
            while current is not None:
                path.append(current.position)
                current = current.parent
            return path[::-1] # Return reversed path

        # Generate children
        children = []
        for new_position in neighbours: # Adjacent squares

            # Get node position
            node_position = (current_node.position[0] + new_position[0], current_node.position[1] + new_position[1])

            # Make sure within range

            if node_position[0] > (len_board_x - 1) or node_position[0] < 0 or node_position[1] > (len_board_y -1) or node_position[1] < 0:
                continue

            # Make sure walkable terrain
            if maze[node_position[0], node_position[1]] != 0:
                continue

            # Create new node
            new_node = Node(current_node, node_position)

            # Append
            children.append(new_node)

        # Loop through children
        for child in children:

            # Child is on the closed list
            for closed_child in closed_list:
                if child == closed_child:
                    continue

            # Create the f, g, and h values
            child.g = current_node.g + 1
            child.h = ((child.position[0] - end_node.position[0]) ** 2) + ((child.position[1] - end_node.position[1]) ** 2)
            child.f = child.g + child.h

            # Child is already in the open list
            for open_node in possible_path:
                if child == open_node and child.g > open_node.g:
                    continue

            # Add the child to the open list
            possible_path.append(child)
    return None


def main():

    maze = Board()

    start = (0, 0)
    end = (100, 600)
    p_time = time.time()
    path = astar(maze, start, end)
    print(time.time()-p_time)


if __name__ == '__main__':
    main()