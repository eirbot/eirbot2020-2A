import pygame
from pygame.locals import *
from pathlib import Path
import pypath as pp  # pypath-fpa
import math

BACKGROUND_COLOR = (33, 33, 33)
FPS = 144

DEFAULT_LIST_OBSTACLE = [pp.Rectangle(pp.Coordinates(60, 20), pp.Size(2, 40)),
                         pp.Rectangle(pp.Coordinates(
                             150, 40), pp.Size(10, 40)),
                         pp.Rectangle(pp.Coordinates(80, 80), pp.Size(160, 2))]



class Frame(object):
    COUNT = 0
    def __init__(self, parent_window, position=(0, 0), size=(10, 10), dynamic_size=False, dynamic_fill_y=False, dynamic_pos=False, hovering=False, background_color=None, margin=0):
        self.id = Frame.COUNT
        Frame.COUNT += 1
        self.parent_window = parent_window
        self.parent_window_size = pygame.display.get_surface().get_size()
        self.parent_window_position = (0,0)
        self.percent_position = position
        self.percent_size = size
        self.position = position
        self.size = size
        self.dynamic_size = dynamic_size
        self.dynamic_fill_y = dynamic_fill_y
        self.dynamic_pos = dynamic_pos
        self.hovering = hovering
        self.background_color = background_color
        self.margin = margin



    def update_position(self):
        self.position = (int(self.percent_position[0]*self.parent_window_size[0]/100)+self.parent_window_position[0],
            int(self.percent_position[1]*self.parent_window_size[1]/100)+self.parent_window_position[1])
    
    def update_size(self):
        self.size = (int(self.percent_size[0]/100 *self.parent_window_size[0]),
            int(self.percent_size[1]/100 *self.parent_window_size[1]))

    def update_size_and_position(self, frame_list):
        self.update_position()
        if self.dynamic_size == True:
            if isinstance(frame_list, list):
                self.compute_dynamic_size(frame_list)
            else:
                raise AttributeError(frame_list)
        else:
            self.update_position()

    def compute_dynamic_size(self, frame_list):
        self.size = (10,10)
        max_x = self.parent_window_size[0]
        max_y = self.parent_window_size[1]

        if self.dynamic_fill_y:
            for frame in frame_list:
                if frame.get_id() != self.id:
                    frame_pos = frame.get_position()    
                    frame_size = frame.get_size()                                              
                    if frame_pos[1] < max_y and frame_pos[1] + frame_size[1] > self.position[1] and self.is_frame_facing(frame, "y"):
                        max_y = frame_pos[1]-1
            self.size = (max((max_x - self.position[0] - self.margin, 0)), max((max_y - self.position[1]- self.margin, 0)))
            for frame in frame_list:
                if frame.get_id() != self.id:
                    frame_pos = frame.get_position()  
                    frame_size = frame.get_size()                     
                    if frame_pos[0] < max_x and frame_pos[0] + frame_size[0] > self.position[0] and self.is_frame_facing(frame, "x"):
                        max_x = frame_pos[0]-1
            self.size = (max((max_x - self.position[0] - self.margin, 0)), max((max_y - self.position[1]- self.margin, 0)))
        else:
            for frame in frame_list:
                if frame.get_id() != self.id:
                    frame_pos = frame.get_position()
                    frame_size = frame.get_size()                       
                    if frame_pos[0] < max_x and frame_pos[0] + frame_size[0] > self.position[0]  and self.is_frame_facing(frame, "x"):
                        max_x = frame_pos[0]-1
                    self.size = (max((max_x - self.position[0] - self.margin, 0)), max((max_y - self.position[1]- self.margin, 0)))
            for frame in frame_list:
                if frame.get_id() != self.id:
                    frame_pos = frame.get_position()    
                    frame_size = frame.get_size()        
                    if frame_pos[1] < max_y and frame_pos[1] + frame_size[1] > self.position[1] and self.is_frame_facing(frame, "y"):
                        max_y = frame_pos[1]-1
            self.size = (max((max_x - self.position[0] - self.margin, 0)), max((max_y - self.position[1]- self.margin, 0)))

    
    def is_frame_facing(self, frame, direction = "x"):
        frame_position = frame.get_position()
        frame_size = frame.get_size()
        if direction == "x":
            if self.position[1] >= frame_position[1] and self.position[1] <= frame_position[1] + frame_size[1]:
                return True
            if self.position[1] + self.size[1] >= frame_position[1] and self.position[1] + self.size[1] <= frame_position[1] + frame_size[1]:
                return True
            if self.position[1] < frame_position[1] and self.position[1] + self.size[1] > frame_position[1] + frame_size[1]:
                return True
            return False
        elif direction == "y":
            if self.position[0] >= frame_position[0] and self.position[0] <= frame_position[0] + frame_size[0]:
                return True
            if self.position[0] + self.size[0] >= frame_position[0] and self.position[0] + self.size[0] <= frame_position[0] + frame_size[0]:
                return True
            if self.position[0] < frame_position[0] and self.position[0] + self.size[0] > frame_position[0] + frame_size[0]:
                return True
            return False
        else:
            raise AttributeError(direction)


    def display(self, frame_list):
        self.update_size_and_position(frame_list)
        if self.background_color is not None:
            pygame.draw.rect(self.parent_window, self.background_color, (*self.position, *self.size), 0)
        pygame.draw.rect(self.parent_window, (0, 0, 0), (*self.position, *self.size), 2) # borders
        # pygame.draw.rect(self.parent_window, (0, 255, 0), (*self.position, 4,4), 0) # debug purpose
        self.extra_display()

    def extra_display(self):
        pass

    def get_id(self):
        return self.id
    
    def update_parent_window_infos(self, size, position=(0,0)):
        self.parent_window_size = size
        self.parent_window_position = position
    
    def pos_screen_to_frame(self, pos):
        x = (pos[0] - self.position[0])
        y = (pos[1] - self.position[1])
        if x < 0 or y < 0 or x >= self.size[0] or y >= self.size[1]:
            return None
        return (x, y)

    def pos_frame_to_screen(self, pos):
        x = (pos[0] + self.position[0])
        y = (pos[1] + self.position[1])
        return (x, y)
    
    def get_position(self):
        return self.position

    def get_size(self):
        return self.size
    


class Container(Frame):
    def __init__(self, parent_window, position=(0,0), size=(200,100), dynamic_size=False, dynamic_fill_y=False, dynamic_pos=False, hovering=False, background_color=(255,255,255)):
        super().__init__(parent_window, position=position, size=size, dynamic_size=dynamic_size, dynamic_pos=dynamic_pos, hovering=hovering, background_color=background_color)
        self.list_frames = []

    def add_frame(self, frame):
        self.list_frames.append(frame)
    
    def extra_display(self):
        for frame in self.list_frames:
            frame.update_parent_window_infos(self.size, self.position)
        for frame in self.list_frames:
            frame.display(self.list_frames)
    
    def update_parent_window_infos(self, size, position=(0,0)):
        super().update_parent_window_infos(size, position)
        for frame in self.list_frames:
            frame.update_parent_window_infos(self.size, self.position)
    
    def delete_frame(self, frame):
        self.list_frames.remove(frame)

class TextFrame(Frame):
    def __init__(self, parent_window, position=(0,0), size=(100,70), dynamic_size=False, dynamic_fill_y=False, dynamic_pos=False, hovering=False, background_color=None, text_color = (255, 255, 255)):
        super().__init__(parent_window, position=position, size=size, dynamic_size=dynamic_size, dynamic_pos=dynamic_pos, hovering=hovering, background_color=background_color)
        self.text_color = text_color
        self.text = ""
    
    def print_text(self, text):
        myfont = pygame.font.SysFont("freemono", 14)
        label = myfont.render(text, 1, self.text_color)
        self.parent_window.blit(label, self.pos_frame_to_screen((10, 10)))

class ModeFrame(TextFrame):
    def __init__(self, parent_window, position=(0,0), size=(100,70), dynamic_size=False, dynamic_fill_y=False, dynamic_pos=False, hovering=False, background_color=BACKGROUND_COLOR, text_color = (255, 255, 255)):
        super().__init__(parent_window, position=position, size=size, dynamic_size=dynamic_size, dynamic_pos=dynamic_pos, hovering=hovering, background_color=background_color, text_color=text_color)
        self.current_mode = StratApp.NORMAL
    
    def extra_display(self):
        if self.current_mode == StratApp.NORMAL:
            self.print_text("NORMAL")
        elif self.current_mode == StratApp.EDIT_PATH:
            self.print_text("EDIT PATH")
        elif self.current_mode == StratApp.RENDER_PATH:
            self.print_text("RENDER PATH")
        elif self.current_mode == StratApp.TEST_PATHFINDING:
            self.print_text("TEST PATHFINDING")

    def set_current_mode(self, current_mode):
        self.current_mode = current_mode


class BoardApp(Frame):
    def __init__(self, parent_window):
        Frame.__init__(self, parent_window)
        self.real_size = (300, 200)  # En cm
        self.board = pp.Field(self.real_size[0] * 10, self.real_size[1] * 10, 200)
        self.add_list_obstacles(DEFAULT_LIST_OBSTACLE)
        self.astar = pp.Castar()
        board_path = Path("ressources/board_1920_1280.png").absolute()
        file = open(board_path, 'r')
        self.raw_board = pygame.image.load(file).convert()


    def update_size_and_position(self, frame_list):
        window_x, window_y = self.parent_window_size
        board_ratio_x_to_y = 2/3
        board_screen_ratio = 0.7
        self.position = (int((window_x-self.size[0])/2), int((window_y-self.size[1])/2))
        self.size = (int(window_x * board_screen_ratio), int(window_x * board_screen_ratio * board_ratio_x_to_y))
        
    
    def extra_display(self):

        board = pygame.transform.scale(
            self.raw_board, self.size)
        self.parent_window.fill(BACKGROUND_COLOR)
        self.parent_window.blit(board, self.position)
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
        x, y = self.pos_board_to_screen((obstacle.pos.x, obstacle.pos.y))
        resized_width, resized_height = self.size_board_to_screen((obstacle.dim.width, obstacle.dim.height))
        pygame.draw.rect(self.parent_window, (100, 20, 20), (x-math.floor(resized_width/2)+1,
                                                       y-math.floor(resized_height/2)+1, resized_width, resized_height), 0)

    def print_nodes(self, nodes):
        for node in nodes:
            self.print_node(node)

    def print_node(self, node):
        x, y = self.pos_board_to_screen((node.x, node.y))
        node_size = self.size_board_to_screen(1)
        pygame.draw.rect(self.parent_window, (255, 100, 255), (x-3, y-3, node_size, node_size), 0)

    def print_waypoints(self, waypoints):
        for wp in waypoints:
            self.print_waypoint(wp)

    def print_waypoint(self, wp):
        x, y = self.pos_board_to_screen(wp)
        pygame.draw.circle(self.parent_window, (19, 191, 97), (x, y),
                           self.size_board_to_screen(4), 0)

    def pathfinding(self, start, end):
        err, list_nodes = self.astar.find_path(start, end, self.board)
        if err:
            print("Astar error " + str(err))
            return None
        return list_nodes

    def pos_board_to_frame(self, pos):
        return (round(pos[0]*self.size[0]/self.real_size[0]),
            round(pos[1]*self.size[1]/self.real_size[1]))

    def pos_frame_to_board(self, pos):
        return (round(pos[0]*self.real_size[0]/self.size[0]),
            round(pos[1]*self.real_size[1]/self.size[1]))
    
    def pos_screen_to_board(self, pos):
        rt = self.pos_screen_to_frame(pos)
        if rt is None:
            return None
        return self.pos_frame_to_board(rt)
    
    def pos_board_to_screen(self, pos):
        return self.pos_frame_to_screen(self.pos_board_to_frame(pos))
    
    def size_board_to_screen(self, size):
        if isinstance(size, tuple):
            return (round(size[0]*self.size[0]/self.real_size[0]),
                round(size[1]*self.size[1]/self.real_size[1]))
        return round(size*self.size[0]/self.real_size[0])

    def size_screen_to_board(self, size):
        return (round(size[0]*self.real_size[0]/self.size[0]),
            round(size[1]*self.real_size[1]/self.size[1]))

class StratApp(object):
    NORMAL = 0
    EDIT_PATH = 1
    TEST_PATHFINDING = 2
    RENDER_PATH = 3

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

        self.mode_frame = ModeFrame(self.fenetre, position=(0,0), dynamic_size=True, background_color=(255, 204, 0), text_color = (0,0,0))
        self.info_frame = ModeFrame(self.fenetre, position=(50,0), dynamic_size=True, background_color=(77, 166, 255), text_color = (0,0,0))
        self.top_container = Container(self.fenetre,position=(20,2), dynamic_size=True,background_color=(42, 36, 36))
        self.top_container.add_frame(self.mode_frame)
        self.top_container.add_frame(self.info_frame)
        self.right_container = Container(self.fenetre,position=(90,2), dynamic_size=True, dynamic_fill_y=True, background_color=(42, 36, 36))
        self.frame_list = [self.board, self.top_container, self.right_container]

    def run(self):
        clock = pygame.time.Clock()
        while self.running:
            clock.tick(FPS)
            for event in pygame.event.get():  # On parcours la liste de tous les événements reçus
                self.event_handler(event)
            self.display()
            pygame.display.flip()

    def event_handler(self, event):
        if event.type == QUIT:  # Si un de ces événements est de type QUIT
            self.running = 0  # On arrête la boucle
        elif event.type == VIDEORESIZE:
            for frame in self.frame_list:
                frame.update_parent_window_infos(event.size)
        elif event.type == KEYDOWN:
            if event.key == K_ESCAPE or event.key == K_n:
                self.mode = StratApp.NORMAL
            if event.key == pygame.K_p:
                self.mode = StratApp.TEST_PATHFINDING
            if event.key == pygame.K_e:
                self.mode = StratApp.EDIT_PATH
            if event.key == pygame.K_r:
                self.mode = StratApp.RENDER_PATH
                self.render_path()
            self.mode_frame.set_current_mode(self.mode)
        elif event.type == MOUSEBUTTONDOWN:
            if event.button == 1:
                if self.mode == StratApp.EDIT_PATH:
                    self.construct_path(event.pos)
                elif self.mode == StratApp.TEST_PATHFINDING:
                    ret = self.push_waypoint_pathfinding(event.pos)
                    if ret: self.run_pathfinding()

            elif event.button == 3:
                if self.mode == StratApp.EDIT_PATH:
                    self.revert_path()

    def display(self):
        self.update_frames()
        if self.mode in [StratApp.EDIT_PATH, StratApp.RENDER_PATH]:
            self.board.print_waypoints(self.constructed_path)
        if self.mode in [StratApp.TEST_PATHFINDING, StratApp.RENDER_PATH]:
            self.board.print_nodes(self.path_pathfinding)
    
    def update_frames(self):
        for frame in self.frame_list:
            frame.display(self.frame_list)

    def construct_path(self, mouse_pos):
        converted_pos = self.board.pos_screen_to_board(mouse_pos)
        if converted_pos is not None:
            self.constructed_path.append(converted_pos)

    def revert_path(self):
        if len(self.constructed_path) != 0:
            self.constructed_path.pop()

    def push_waypoint_pathfinding(self, wp):
        converted_pos = self.board.pos_screen_to_board(wp)
        if converted_pos is not None:
            if len(self.waypoints_pathfinding) >= 2:
                self.waypoints_pathfinding.pop(0)
            self.waypoints_pathfinding.append(converted_pos)
            return True
        return False

    def run_pathfinding(self):
        if len(self.waypoints_pathfinding) >= 2:
            wp0 = self.waypoints_pathfinding[0]
            wp1 = self.waypoints_pathfinding[1]
            returned_value = self.board.pathfinding(pp.Coordinates(
                wp0[0], wp0[1]), pp.Coordinates(wp1[0], wp1[1]))
            if returned_value is not None:
                self.path_pathfinding = returned_value
                print("Astar from", wp0, " to ", wp1)
    
    def render_path(self):
        self.path_pathfinding = []
        if len(self.constructed_path) < 2 : return
        path = self.constructed_path.copy()
        wp0 = path.pop(0)
        wp1 = path.pop(0)
        returned_value = self.board.pathfinding(pp.Coordinates(
            wp0[0], wp0[1]), pp.Coordinates(wp1[0], wp1[1]))
        if returned_value is not None:
            self.path_pathfinding.extend(returned_value)
        for node in path: 
            wp0 = wp1
            wp1 = node
            returned_value = self.board.pathfinding(pp.Coordinates(
                wp0[0], wp0[1]), pp.Coordinates(wp1[0], wp1[1]))
            if returned_value is not None:
                self.path_pathfinding.extend(returned_value)


class Robot(object):
    def __init__(self, width_mm=30, lenght_mm=30):
        self.width = width_mm
        self.lenght = lenght_mm

    def display(self):
        pass




if __name__ == "__main__":
    app = StratApp()
    app.run()
