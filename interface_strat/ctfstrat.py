import pygame
from pygame.locals import *
import numpy as np
import os
from pathlib import Path
from pathfinding import Board, Node, astar


BACKGROUND_COLOR = (33, 33, 33)
BOARD_RATIO_X_TO_Y = 2/3


class Robot(object):
    def __init__(self, width_mm=30, lenght_mm=20):
        self.width = width_mm
        self.lenght = lenght_mm
    
    def display(self):
        pass

class BoardApp(object):
    def __init__(self):
        self.fenetre = pygame.display.set_mode((1920, 1080), RESIZABLE | DOUBLEBUF)
        self.board = Board()
        board_path = Path("ressources/board_1920_1280.png").absolute()
        file = open(board_path, 'r')
        self.raw_board = pygame.image.load(file).convert()
        self.board_size_x = 0
        self.board_size_y = 0
        self.board_pos_x = 0
        self.board_pos_y = 0
        self.real_x, self.real_y = self.board.size()
        self.display()
    
    def display(self, w_size=None):
        if w_size is None:
            window_x, window_y = pygame.display.get_surface().get_size()
        else:
            window_x, window_y = w_size
        self.board_size_x = int(window_x * 0.80)
        self.board_size_y = int(self.board_size_x * BOARD_RATIO_X_TO_Y)
        self.board_pos_x = int((window_x-self.board_size_x)/2)
        self.board_pos_y = int((window_y-self.board_size_y)/2)
        board = pygame.transform.scale(self.raw_board, (self.board_size_x, self.board_size_y))
        self.fenetre.fill(BACKGROUND_COLOR)
        self.fenetre.blit(board, (self.board_pos_x, self.board_pos_y))
    
    def display_occuped_zones(self):
        board_x, board_y = self.board.size()
        for x in range(0,board_x - 1):
            for y in range(0,board_y - 1):
                if self.board[x, y]:
                    self.print_occuped_zone(x, y)
    
    def print_occuped_zone(self, x, y):
        x = round(self.board_pos_x + x*self.board_size_x/self.real_x)
        y = round(self.board_pos_y + y*self.board_size_y/self.real_y)
        pygame.draw.rect(self.fenetre, (255, 100, 255), (x-3, y-3, 5, 5), 1)
    
    def test_pathfinding(self, start, end):
        for node in astar(self.board, start, end):
            self.print_occuped_zone(node[0], node[1])


class StratApp(object):
    def __init__(self):
        pygame.init()
        self.fenetre = pygame.display.set_mode((1920, 1080), RESIZABLE | DOUBLEBUF)
        self.fenetre.fill(BACKGROUND_COLOR)
        self.running = 1
        self.board = BoardApp()

    def run(self):
        while self.running:
            for event in pygame.event.get():   #On parcours la liste de tous les événements reçus
                if event.type == QUIT or event.type == K_ESCAPE:     #Si un de ces événements est de type QUIT
                    self.running = 0      #On arrête la boucle
                if event.type == VIDEORESIZE:
                    self.board.display(event.size)
                if event.type == KEYDOWN:
                    if event.key == pygame.K_p:
                        print("Calculating")
                        self.board.test_pathfinding((1, 1), (600, 1200))
            pygame.display.flip()

if __name__ == "__main__":
    app = StratApp()
    app.run()
