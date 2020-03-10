import numpy as np

class Board():
    POINT = 0
    
    def __init__(self, width_mm=3000, height_mm=2000):
        self.width = width_mm
        self.height = height_mm
        self.surface = np.zeros((width_mm, height_mm), dtype=np.uint8)

    def add_obstacle(self, x, y):
        self.surface[y, x] = 1

    def __getitem__(self, key):
        if isinstance(key, tuple):
            x, y = key
        if isinstance(key, int):
            x = key
            y = 0
        if x < 0 or x > self.width or y < 0 or y > self.height:
            raise ValueError("Out of range!")
        return self.surface[y, x]
    

