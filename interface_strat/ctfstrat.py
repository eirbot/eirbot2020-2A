import pygame
from pygame.locals import *
from pathlib import Path
import pypath as pp  # pypath-fpa
import math

BACKGROUND_COLOR = (33, 33, 33)
BOARD_RATIO_X_TO_Y = 2/3

DEFAULT_LIST_OBSTACLE = [pp.Rectangle(pp.Coordinates(60, 20), pp.Size(2, 40)),
                         pp.Rectangle(pp.Coordinates(
                             150, 40), pp.Size(10, 40)),
                         pp.Rectangle(pp.Coordinates(80, 80), pp.Size(160, 2))]



class Frame(object):
    COUNT = 0
    def __init__(self, parent_window, position=(0, 0), size=(100, 70), dynamic_size=False, dynamic_pos=False, hovering=False, background_color=(255,255,255)):
        self.id = Frame.COUNT
        Frame.COUNT += 1
        self.parent_window = parent_window
        self.parent_window_size = pygame.display.get_surface().get_size()
        self.position = position
        self.size = size
        self.dynamic_size = dynamic_size
        self.dynamic_pos = dynamic_pos
        self.hovering = hovering
        self.background_color = background_color

    def update_size_and_position(self, frame_list):
        for frame in frame_list:
            if frame.get_id() != self.id:
                pass

    def display(self, frame_list):
        self.update_size_and_position(frame_list)
        pygame.draw.rect(self.parent_window, self.background_color, (*self.position, *self.size), 2)

    def get_id(self):
        return self.id
    
    def update_parent_window_size(self, size):
        self.parent_window_size = size
    





class Robot(object):
    def __init__(self, width_mm=30, lenght_mm=30):
        self.width = width_mm
        self.lenght = lenght_mm

    def display(self):
        pass


class BoardApp(Frame):
    def __init__(self, parent_window):
        Frame.__init__(self, parent_window)
        self.real_x, self.real_y = (300, 200)  # En cm
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

    def display(self, frame_list, w_size=None):
        super().display(frame_list)
        window_x, window_y = self.parent_window_size
        self.board_size_x = int(window_x * 0.80)
        self.board_size_y = int(self.board_size_x * BOARD_RATIO_X_TO_Y)
        self.board_pos_x = int((window_x-self.board_size_x)/2)
        self.board_pos_y = int((window_y-self.board_size_y)/2)
        board = pygame.transform.scale(
            self.raw_board, (self.board_size_x, self.board_size_y))
        self.parent_window.fill(BACKGROUND_COLOR)
        self.parent_window.blit(board, (self.board_pos_x, self.board_pos_y))
        self.display_obstacles()

    def add_list_obstacles(self, obstacle):
        if isinstance(obstacle, pp.Rectangle):
            self.board.add_obstacle(obstacle)
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
        resized_height = round(obstacle.dim.height *
                               self.board_size_y/self.real_y)
        pygame.draw.rect(self.parent_window, (100, 20, 20), (x-math.floor(resized_width/2)+1,
                                                       y-math.floor(resized_height/2)+1, resized_width, resized_height), 0)

    def print_nodes(self, nodes):
        for node in nodes:
            self.print_node(node)

    def print_node(self, node):
        x = round(self.board_pos_x + node.x*self.board_size_x/self.real_x)
        y = round(self.board_pos_y + node.y*self.board_size_y/self.real_y)
        pygame.draw.rect(self.parent_window, (255, 100, 255), (x-3, y-3, 5, 5), 0)

    def print_waypoints(self, waypoints):
        for wp in waypoints:
            self.print_waypoint(wp)

    def print_waypoint(self, wp):
        x = round(self.board_pos_x + wp[0]*self.board_size_x/self.real_x)
        y = round(self.board_pos_y + wp[1]*self.board_size_y/self.real_y)
        pygame.draw.circle(self.parent_window, (19, 191, 97), (x, y),
                           round(4 * self.board_size_x/self.real_x), 0)

    def pathfinding(self, start, end):
        err, list_nodes = self.astar.find_path(start, end, self.board)
        if err:
            print("Astar error " + str(err))
            return
        return list_nodes

    def convert_screen_pos_to_board_pos(self, point):
        x = round((point[0] - self.board_pos_x)*self.real_x/self.board_size_x)
        y = round((point[1] - self.board_pos_y)*self.real_y/self.board_size_y)
        if x < 0 or y < 0 or x >= self.real_x or y >= self.real_y:
            return None
        return (x, y)


class StratApp(object):
    NORMAL = 0
    EDIT_PATH = 1
    TEST_PATHFINDING = 2

    def __init__(self):
        pygame.init()
        self.fenetre = pygame.display.set_mode(
            (1920, 1080), RESIZABLE | DOUBLEBUF)
        self.fenetre.fill(BACKGROUND_COLOR)
        self.running = 1
        self.board = BoardApp(self.fenetre)
        self.last_wsize = pygame.display.get_surface().get_size()

        self.constructed_path = []
        self.mode = StratApp.NORMAL

        self.waypoints_pathfinding = []
        self.path_pathfinding = []

        self.frame_list = [self.board]

    def run(self):
        clock = pygame.time.Clock()
        while self.running:
            clock.tick(144)
            for event in pygame.event.get():  # On parcours la liste de tous les événements reçus
                self.event_handler(event)
            self.display()
            pygame.display.flip()

    def event_handler(self, event):
        if event.type == QUIT:  # Si un de ces événements est de type QUIT
            self.running = 0  # On arrête la boucle
        elif event.type == VIDEORESIZE:
            for frame in self.frame_list:
                frame.update_parent_window_size(event.size)
        elif event.type == KEYDOWN:
            if event.key == K_ESCAPE or event.key == K_n:
                self.mode = StratApp.NORMAL
            if event.key == pygame.K_p:
                self.mode = StratApp.TEST_PATHFINDING
            if event.key == pygame.K_e:
                self.mode = StratApp.EDIT_PATH
        elif event.type == MOUSEBUTTONDOWN:
            if event.button == 1:
                if self.mode == StratApp.EDIT_PATH:
                    self.construct_path(event.pos)
                elif self.mode == StratApp.TEST_PATHFINDING:
                    self.push_waypoint_pathfinding(event.pos)
                    self.run_pathfinding()

            elif event.button == 3:
                if self.mode == StratApp.EDIT_PATH:
                    self.revert_path()

    def display(self):
        self.update_frames()
        if self.mode == StratApp.EDIT_PATH:
            self.board.print_waypoints(self.constructed_path)
        if self.mode == StratApp.TEST_PATHFINDING:
            self.board.print_nodes(self.path_pathfinding)
    
    def update_frames(self):
        for frame in self.frame_list:
            frame.display(self.frame_list)

    def construct_path(self, mouse_pos):
        converted_pos = self.board.convert_screen_pos_to_board_pos(mouse_pos)
        if converted_pos is not None:
            self.constructed_path.append(converted_pos)

    def revert_path(self):
        if len(self.constructed_path) != 0:
            self.constructed_path.pop()

    def push_waypoint_pathfinding(self, wp):
        converted_pos = self.board.convert_screen_pos_to_board_pos(wp)
        if converted_pos is not None:
            if len(self.waypoints_pathfinding) >= 2:
                self.waypoints_pathfinding.pop(0)
            self.waypoints_pathfinding.append(converted_pos)

    def run_pathfinding(self):
        if len(self.waypoints_pathfinding) >= 2:
            wp0 = self.waypoints_pathfinding[0]
            wp1 = self.waypoints_pathfinding[1]
            print("Astar from", wp0, " to ", wp1)
            returned_value = self.board.pathfinding(pp.Coordinates(
                wp0[0], wp0[1]), pp.Coordinates(wp1[0], wp1[1]))
            if returned_value is not None:
                self.path_pathfinding = returned_value



if __name__ == "__main__":
    app = StratApp()
    app.run()
