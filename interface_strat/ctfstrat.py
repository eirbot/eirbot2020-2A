import pygame
from pygame.locals import *
import numpy as np
import os
from pathlib import Path
import pypath as pp # pypath-fpa
import math

BACKGROUND_COLOR = (33, 33, 33)
BOARD_RATIO_X_TO_Y = 2/3

DEFAULT_LIST_OBSTACLE = [pp.Rectangle(pp.Coordinates(60, 20), pp.Size(2, 40)),
                         pp.Rectangle(pp.Coordinates(150, 40), pp.Size(10, 40)),
                         pp.Rectangle(pp.Coordinates(80, 80), pp.Size(160, 2))]


class Robot(object):
    def __init__(self, width_mm=30, lenght_mm=30):
        self.width = width_mm
        self.lenght = lenght_mm

    def display(self):
        pass


class BoardApp(object):
    def __init__(self):
        self.fenetre = pygame.display.set_mode(
            (1920, 1080), RESIZABLE | DOUBLEBUF)

        self.real_x, self.real_y = (300, 200) # En cm
        self.board = pp.Field(self.real_x * 10, self.real_x * 10, 200)
        self.add_list_obstacles(DEFAULT_LIST_OBSTACLE)
        self.astar = pp.Castar()
        board_path = Path("ressources/board_1920_1280.png").absolute()
        file = open(board_path, 'r')
        self.raw_board = pygame.image.load(file).convert()
        self.board_size_x = 0
        self.board_size_y = 0
        self.board_pos_x = 0
        self.board_pos_y = 0
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
        board = pygame.transform.scale(
            self.raw_board, (self.board_size_x, self.board_size_y))
        self.fenetre.fill(BACKGROUND_COLOR)
        self.fenetre.blit(board, (self.board_pos_x, self.board_pos_y))
        self.display_obstacles()


    def add_list_obstacles(self, obstacle):
        if isinstance(obstacle, pp.Rectangle):
            self.board.add_obstacle(board)
            return True
        elif isinstance(obstacle, list):
            if len(obstacle) != 0:
                if isinstance(obstacle[0], pp.Rectangle):
                    for obs in obstacle:
                        self.board.add_obstacle(obs)
                    return True
        print("No valid arguemnts :", obstacle)
        return False

    def display_obstacles(self):
        for obstacle in self.board.get_list_of_obstacle():
            self.print_obstacle(obstacle)

    def print_obstacle(self, obstacle):
        x = round(self.board_pos_x + obstacle.pos.x *
                  self.board_size_x/self.real_x)
        y = round(self.board_pos_y + obstacle.pos.y *
                  self.board_size_y/self.real_y)
        
        resized_width = round(obstacle.dim.width*self.board_size_x/self.real_x)
        resized_height = round(obstacle.dim.height*self.board_size_y/self.real_y)
        pygame.draw.rect(self.fenetre, (20, 20, 20), (x-math.floor(resized_width/2)+1,
                                                         y-math.floor(resized_height/2)+1, resized_width, resized_height), 0)

    def print_node(self, node):
        x = round(self.board_pos_x + node.x*self.board_size_x/self.real_x)
        y = round(self.board_pos_y + node.y*self.board_size_y/self.real_y)
        pygame.draw.rect(self.fenetre, (255, 100, 255), (x-3, y-3, 5, 5), 0)

    def test_pathfinding(self, start, end):
        err , list_nodes = self.astar.find_path(start, end, self.board)
        if err:
            print("Astar error "+ str(err))
            return
        for node in list_nodes:
            self.print_node(node)


class StratApp(object):
    def __init__(self):
        pygame.init()
        self.fenetre = pygame.display.set_mode(
            (1920, 1080), RESIZABLE | DOUBLEBUF)
        self.fenetre.fill(BACKGROUND_COLOR)
        self.running = 1
        self.board = BoardApp()

    def run(self):
        while self.running:
            for event in pygame.event.get():  # On parcours la liste de tous les événements reçus
                if event.type == QUIT or event.type == K_ESCAPE:  # Si un de ces événements est de type QUIT
                    self.running = 0  # On arrête la boucle
                if event.type == VIDEORESIZE:
                    self.board.display(event.size)
                if event.type == KEYDOWN:
                    if event.key == pygame.K_p:
                        print("Calculating")
                        self.board.test_pathfinding(pp.Coordinates(1, 1), pp.Coordinates(250, 150))
            pygame.display.flip()


if __name__ == "__main__":
    app = StratApp()
    app.run()
