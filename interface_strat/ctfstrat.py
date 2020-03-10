import pygame
from pygame.locals import *
import numpy as np
import os
from pathlib import Path

class StratApp(object):
    def __init__(self):
        pygame.init()
        self.fenetre = pygame.display.set_mode((1920, 1080), RESIZABLE)
        self.running = 1
        board_path = Path("ressources/board_1920_1280.png").absolute()
        file = open(board_path, 'r')
        self.raw_board = pygame.image.load(file).convert()
        window_x, window_y = pygame.display.get_surface().get_size()
        board_size_x = int(window_x * 0.80)
        board_size_y = int(window_y * 0.80)
        board_pos_x = (window_x-board_size_x)/2
        board_pos_y = (window_y-board_size_y)/2
        self.board = pygame.transform.scale(self.raw_board, (board_size_x, board_size_y))

        self.fenetre.blit(self.board, (board_pos_x, board_pos_y))

    
    def run(self):
        while self.running:
            for event in pygame.event.get():   #On parcours la liste de tous les événements reçus
                if event.type == QUIT or event.type == K_ESCAPE:     #Si un de ces événements est de type QUIT
                    continuer = 0      #On arrête la boucle
            pygame.display.flip()

if __name__ == "__main__":
    app = StratApp()
    app.run()
