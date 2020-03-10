import pygame
from pygame.locals import *
import numpy as np
import os
from pathlib import Path
from pathfinding import Board


BACKGROUND_COLOR = (33,33,33)
class StratApp(object):
    def __init__(self):
        pygame.init()
        self.fenetre = pygame.display.set_mode((1920, 1080), RESIZABLE | DOUBLEBUF)
        self.running = 1
        board_path = Path("ressources/board_1920_1280.png").absolute()
        file = open(board_path, 'r')
        self.raw_board = pygame.image.load(file).convert()
        self.board = None
        self.update_board()
        self.path_board = Board()

    def update_board(self, w_size=None):
        if w_size is None:
            window_x, window_y = pygame.display.get_surface().get_size()
        else:
            window_x, window_y = w_size
        board_size_x = int(window_x * 0.80)
        board_size_y = int(board_size_x * 9/16)
        board_pos_x = (window_x-board_size_x)/2
        board_pos_y = (window_y-board_size_y)/2
        self.board = pygame.transform.scale(self.raw_board, (board_size_x, board_size_y))
        self.fenetre.fill(BACKGROUND_COLOR)
        self.fenetre.blit(self.board, (board_pos_x, board_pos_y))
    
    def run(self):
        while self.running:
            for event in pygame.event.get():   #On parcours la liste de tous les événements reçus
                if event.type == QUIT or event.type == K_ESCAPE:     #Si un de ces événements est de type QUIT
                    self.running = 0      #On arrête la boucle
                if event.type == VIDEORESIZE:
                    self.update_board(event.size)
            pygame.display.flip()

if __name__ == "__main__":
    app = StratApp()
    app.run()
