import pygame
from pygame.locals import *
from pathlib import Path
import pypath as pp  # pypath-fpa
import math
import time

BACKGROUND_COLOR = (33, 33, 33)
FPS = 60

DEFAULT_LIST_OBSTACLE = [pp.Rectangle(pp.Coordinates(60, 20), pp.Size(2, 40)),
                         pp.Rectangle(pp.Coordinates(
                             150, 40), pp.Size(10, 40)),
                         pp.Rectangle(pp.Coordinates(80, 80), pp.Size(160, 2))]



class Frame(object):
    COUNT = 0
    def __init__(self, parent_window, position=(0, 0), size=(100, 100), dynamic_size=False, dynamic_fill_y=False, hovering=False, background_color=None, margin=0):
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
        self.hovering = hovering
        self.background_color = background_color
        self.margin = margin



    def update_position(self):
        self.position = ((int(self.percent_position[0]*self.parent_window_size[0]/100)+self.parent_window_position[0]),
                (int(self.percent_position[1]*self.parent_window_size[1]/100)+self.parent_window_position[1]))
    
    def update_size(self):
        self.size = (max(int(self.percent_size[0]/100 *self.parent_window_size[0]), 0),
                    max(int(self.percent_size[1]/100 *self.parent_window_size[1]), 0))

    def update_size_and_position(self, frame_list):
        self.update_position()
        if self.dynamic_size == True:
            if isinstance(frame_list, list):
                self.compute_dynamic_size(frame_list)
            else:
                raise AttributeError(frame_list)
        else:
            self.update_size()

    def compute_dynamic_size(self, frame_list):
        self.size = (1,1)
        max_x = self.parent_window_size[0] + self.parent_window_position[0]
        max_y = self.parent_window_size[1] + self.parent_window_position[1]

        if self.dynamic_fill_y:
            max_y = self.find_max_y_dynamic_size(frame_list, max_y)
            self.size = (int(max(max_x - self.position[0] - self.margin, 0)), int(max(max_y - self.position[1]- self.margin, 0)))
            max_x = self.find_max_x_dynamic_size(frame_list, max_x)
            self.size = (int(max(max_x - self.position[0] - self.margin, 0)), int(max(max_y - self.position[1]- self.margin, 0)))
        else:
            max_x = self.find_max_x_dynamic_size(frame_list, max_x)
            self.size = (int(max(max_x - self.position[0]  - self.margin, 0)), int(max(max_y - self.position[1]- self.margin, 0)))
            max_y = self.find_max_y_dynamic_size(frame_list, max_y)
            self.size = (int(max(max_x - self.position[0] - self.margin, 0)), int(max(max_y - self.position[1]- self.margin, 0)))

    def find_max_x_dynamic_size(self, frame_list, max_x):
        for frame in frame_list:
            if frame.get_id() != self.id:
                frame_pos = frame.get_position()  
                frame_size = frame.get_size()                     
                if frame_pos[0] < max_x and frame_pos[0] + frame_size[0] > self.position[0] and self.is_frame_facing(frame, "x"):
                    max_x = frame_pos[0]-1
        return max_x

    def find_max_y_dynamic_size(self, frame_list, max_y):
        for frame in frame_list:
            if frame.get_id() != self.id:
                frame_pos = frame.get_position()    
                frame_size = frame.get_size()                                              
                if frame_pos[1] < max_y and frame_pos[1] + frame_size[1] > self.position[1] and self.is_frame_facing(frame, "y"):
                    max_y = frame_pos[1]-1
        return max_y

    
    def is_frame_facing(self, frame, direction = "x"):
        frame_position = frame.get_position()
        frame_size = frame.get_size()
        if direction == "x":
            if self.position[1] >= frame_position[1] and self.position[1] < frame_position[1] + frame_size[1]:
                return True
            if self.position[1] + self.size[1] >= frame_position[1] and self.position[1] + self.size[1] <= frame_position[1] + frame_size[1]:
                return True
            if self.position[1] <= frame_position[1] and self.position[1] + self.size[1] > frame_position[1] + frame_size[1]:
                return True
            return False
        elif direction == "y":
            if self.position[0] >= frame_position[0] and self.position[0] < frame_position[0] + frame_size[0]:
                return True
            if self.position[0] + self.size[0] >= frame_position[0] and self.position[0] + self.size[0] <= frame_position[0] + frame_size[0]:
                return True
            if self.position[0] <= frame_position[0] and self.position[0] + self.size[0] > frame_position[0] + frame_size[0]:
                return True
            return False
        else:
            raise AttributeError(direction)


    def display(self, frame_list=None):
        self.update_size_and_position(frame_list)
        if self.background_color is not None:
            pygame.draw.rect(self.parent_window, self.background_color, (*self.position, *self.size), 0)
        pygame.draw.rect(self.parent_window, (0, 0, 0), (*self.position, *self.size), 2) # borders
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
        return (x*100/self.size[0], y*100/self.size[1])

    
    def size_frame_to_screen(self, size):
        if isinstance(size, tuple):
            return (max(int(size[0]*100 /self.size[0]), 0),
                    max(int(size[1]*100 /self.size[1]), 0))
        return max(int(size *self.size[0]/100), 0)

    def size_screen_to_frame(self, size):
        if isinstance(size, tuple):
            return (max(int(size[0]/100 *self.size[0]),0),
                    max(int(size[1]/100 *self.size[1]),0))
        return max(int(size/100 *self.size[0]), 0)

    def pos_frame_to_screen(self, pos):
        return (max(int(pos[0]*self.size[0]/100)+self.position[0], 0),
                max(int(pos[1]*self.size[1]/100)+self.position[1], 0))
    
    def get_position(self):
        return self.position

    def get_size(self):
        return self.size
    
    def set_margin(self, margin):
        self.margin = int(margin)
    


class Container(Frame):
    def __init__(self, parent_window, position=(0,0), size=(100,100), dynamic_size=False, dynamic_fill_y=False, hovering=False, background_color=(255,255,255)):
        super().__init__(parent_window, position=position, size=size, dynamic_size=dynamic_size, hovering=hovering, background_color=background_color)
        self.list_frames = []

    def add_frame(self, frame):
        if isinstance(frame, list):
            self.list_frames.extend(frame)
        else:
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
    
    def set_inner_margin(self, margin):
        for frame in self.list_frames:
            frame.set_margin(margin)


class Content():
    def __init__(self, unique_id=0, content=[]):
        self.id = unique_id
        self.content = []
        self.add_content(content)
    
    def add_content(self, content):
        if isinstance(content, list):
            self.content.extend(content)
        else:
            self.content.append(content)
    
    def get_id(self):
        return self.id 
    
    def get_content(self):
        return self.content
class DynamicContentContainer(Container):
    def __init__(self, parent_window, position=(0,0), size=(100,100), dynamic_size=False, dynamic_fill_y=False, hovering=False, background_color=(255,255,255)):
        super().__init__(parent_window, position=position, size=size, dynamic_size=dynamic_size, dynamic_fill_y=dynamic_fill_y, hovering=hovering, background_color=background_color)
        self.list_content = []

    def add_content(self, unique_id, content):
        found=0
        for cnt in self.list_content:
            if cnt.get_id() == unique_id:
                found=1
                cnt.add_content(content)
                break
        if not found:
            self.list_content.append(Content(unique_id, content))
    
    def load_content(self, unique_id):
        for cnt in self.list_content:
            if cnt.get_id() == unique_id:
                self.list_frames = cnt.content


class TextFrame(Frame):
    def __init__(self, parent_window, position=(0,0), size=(100,100), dynamic_size=False, dynamic_fill_y=False, hovering=False, background_color=None, text_color = (255, 255, 255)):
        super().__init__(parent_window, position=position, size=size, dynamic_size=dynamic_size, hovering=hovering, background_color=background_color)
        self.text_color = text_color
        self.height_line_jump = 20 # static
        self.font_size = 6 # percent

    def max_line_len(self, text):
        max_len = 0
        for line in text.split('\n'):
            len_line = len(line)
            if len_line > max_len:
                max_len = len_line
        return max_len
    
    def print_text(self, text, centered_level=1, maximize=False):
        if maximize:
            screen_font_size = self.size_frame_to_screen(self.font_size*25/self.max_line_len(text))
        else:
            screen_font_size = self.size_frame_to_screen(self.font_size)
        screen_height_line_jump = int(self.height_line_jump * screen_font_size/25) 
        if centered_level == 1:
            line_pos_x, line_pos_y = self.pos_frame_to_screen((2, 50))
            line_pos_y -= screen_height_line_jump * text.count("\n") + screen_height_line_jump//2
        else:
            line_pos_x, line_pos_y = self.pos_frame_to_screen((2, 2))
        for line in text.split("\n"):
            myfont = pygame.font.SysFont("freemono", screen_font_size)
            label = myfont.render(line, 1, self.text_color)
            self.parent_window.blit(label, (line_pos_x, line_pos_y))
            line_pos_y += screen_height_line_jump

class PermanantTextFrame(TextFrame):
    def __init__(self, parent_window, text, position=(0,0), size=(100,70), dynamic_size=False, dynamic_fill_y=False, hovering=False, background_color=None, text_color=(255,255,255), maximize_size=False):
        super().__init__(parent_window, position=position, size=size, dynamic_size=dynamic_size, dynamic_fill_y=dynamic_fill_y, hovering=hovering, background_color=background_color, text_color=text_color)
        self.text = text
        self.maximize_size = maximize_size
    
    def extra_display(self):
        self.print_text(self.text, 1, self.maximize_size)

class FpsFrame(TextFrame):
    def __init__(self, parent_window, position=(0,0), size=(100,100), dynamic_size=False, dynamic_fill_y=False, hovering=False, background_color=None, text_color=(255,255,255)):
        super().__init__(parent_window, position=position, size=size, dynamic_size=dynamic_size, dynamic_fill_y=dynamic_fill_y, hovering=hovering, background_color=background_color, text_color=text_color)
        self.previous_time = 0
        self.count_per_second = 0
        self.fps = 0

    def extra_display(self):
        actual_time = time.time()
        self.count_per_second += 1
        time_diff = (actual_time - self.previous_time)
        if time_diff > 0.5:
            self.fps = int(self.count_per_second/time_diff)
            self.count_per_second = 0
            self.previous_time = actual_time
        self.print_text(str(self.fps), maximize=True)
    


class ModeFrame(TextFrame):
    def __init__(self, parent_window, position=(0,0), size=(100,70), dynamic_size=False, dynamic_fill_y=False, hovering=False, background_color=BACKGROUND_COLOR, text_color = (255, 255, 255)):
        super().__init__(parent_window, position=position, size=size, dynamic_size=dynamic_size, hovering=hovering, background_color=background_color, text_color=text_color)
        self.current_mode = StratApp.HOME
    
    def extra_display(self):
        if self.current_mode == StratApp.HOME:
            self.print_text("HOME")
        elif self.current_mode == StratApp.EDIT_PATH:
            self.print_text("EDIT PATH")
        elif self.current_mode == StratApp.RENDER_PATH:
            self.print_text("RENDER PATH")
        elif self.current_mode == StratApp.TEST_PATHFINDING:
            self.print_text("TEST PATHFINDING")

    def set_current_mode(self, current_mode):
        self.current_mode = current_mode


class FixedRatioFrame(Frame):
    def __init__(self, parent_window, ratio_x_to_y=1, position=(0,0), size=(100,100), hovering=False, background_color=None, margin=0):
        super().__init__(parent_window, position=position, size=size, dynamic_size=True, hovering=hovering, background_color=background_color, margin=margin)
        self.ratio_x_to_y = ratio_x_to_y

    def update_size_and_position(self, frame_list):
        self.update_position()
        self.compute_dynamic_size(frame_list)
    
    def compute_dynamic_size(self, frame_list):
        max_x = self.parent_window_size[0] + self.parent_window_position[0]
        max_y = self.parent_window_size[1] + self.parent_window_position[1]

        max_x = self.find_max_x_dynamic_size(frame_list, max_x)
        self.size = (max(max_x - self.position[0]  - self.margin, 0), max(max_y - self.position[1]- self.margin, 0))
        max_y = self.find_max_y_dynamic_size(frame_list, max_y)
        self.size = (max(max_x - self.position[0] - self.margin, 0), max(max_y - self.position[1]- self.margin, 0))

        if self.size[0] * self.ratio_x_to_y < self.size[1] :
            self.size = (int(self.size[0]), int(self.size[0] * self.ratio_x_to_y))
        else:
            self.size = (int(self.size[1]/self.ratio_x_to_y), int(self.size[1]))




class BoardApp(FixedRatioFrame):
    def __init__(self, parent_window, ratio_x_to_y=1, position=(0,0), size=(100,100), hovering=False, background_color=None, margin=0):
        super().__init__(parent_window, ratio_x_to_y=ratio_x_to_y, position=position, size=size, hovering=hovering, background_color=background_color, margin=margin)
        self.real_size = (300, 200)  # En cm
        self.board = pp.Field(self.real_size[0] * 10, self.real_size[1] * 10, 200)
        self.add_list_obstacles(DEFAULT_LIST_OBSTACLE)
        self.astar = pp.Castar()
        board_path = Path("ressources/board_1920_1280.png").absolute()
        file = open(board_path, 'r')
        self.raw_board = pygame.image.load(file).convert()



    def extra_display(self):
        board = pygame.transform.scale(
            self.raw_board, self.size)
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
        start = tuple([int(start[i]) for i in range(len(start))])
        end = tuple([int(end[i]) for i in range(len(end))])
        err, list_nodes = self.astar.find_path(pp.Coordinates(*start), pp.Coordinates(*end), self.board)
        if err:
            print("Astar error " + str(err))
            return None
        return list_nodes

    def pos_board_to_frame(self, pos):
        return ((pos[0]*100/self.real_size[0]),
            (pos[1]*100/self.real_size[1]))

    def pos_frame_to_board(self, pos):
        return ((pos[0]*self.real_size[0]/100),
            (pos[1]*self.real_size[1]/100))
    
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
        return ((size[0]*self.real_size[0]/self.size[0]),
            (size[1]*self.real_size[1]/self.size[1]))

class StratApp(object):
    HOME = 0
    EDIT_PATH = 1
    TEST_PATHFINDING = 2
    RENDER_PATH = 3

    def __init__(self):
        pygame.init()
        self.main_window = pygame.display.set_mode(
            (1920, 1080), RESIZABLE | DOUBLEBUF)
        self.main_container = Container(self.main_window, size=(100,100), background_color=BACKGROUND_COLOR)
        self.running = 1
        self.board = BoardApp(self.main_window, ratio_x_to_y=2/3, position=(0,20))
        self.last_wsize = pygame.display.get_surface().get_size()

        self.constructed_path = []
        self.mode = StratApp.HOME

        self.waypoints_pathfinding = []
        self.path_pathfinding = []

        self.mode_frame = ModeFrame(self.main_window, position=(0,0), dynamic_size=True, background_color=(255, 204, 0), text_color = (0,0,0))
        self.info_frame = PermanantTextFrame(self.main_window,"CTF STRAT \nEDITION 2020", position=(50,0), dynamic_size=True, background_color=(77, 166, 255), text_color = (0,0,0))
        self.top_container = Container(self.main_window,position=(0,0), size=(100,20), dynamic_size=False ,background_color=(42, 36, 36))
        self.top_container.add_frame(self.mode_frame)
        self.top_container.add_frame(self.info_frame)
        self.left_container = Container(self.main_window,position=(0,0), dynamic_size=True, background_color=(42, 36, 36))
        self.dyn_container = DynamicContentContainer(self.main_window,position=(0,0), dynamic_size=True, dynamic_fill_y=True, background_color=(42, 36, 36))
        self.dyn_container.add_content(StratApp.HOME, PermanantTextFrame(self.main_window,"ESC : Home\nE : Edit path\nR : Render Path\nP : Test Pathfinding", maximize_size=True))
        self.dyn_container.add_content(StratApp.RENDER_PATH, PermanantTextFrame(self.main_window,"Rendered!"))
        self.right_container = Container(self.main_window, position=(85,0), dynamic_size=True, dynamic_fill_y=True, background_color=(42, 36, 36))
        self.right_container.add_frame([self.dyn_container, FpsFrame(self.main_window, position=(0,80), dynamic_size=True, background_color=(200, 36, 36))])
        self.left_container.add_frame([self.top_container, self.board])
        self.main_container.add_frame([self.left_container,self.right_container])

        self.dyn_container.load_content(StratApp.HOME)


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
            self.main_container.update_parent_window_infos(event.size)
        elif event.type == KEYDOWN:
            if event.key == K_ESCAPE or event.key == K_n:
                self.mode = StratApp.HOME
            if event.key == pygame.K_p:
                self.mode = StratApp.TEST_PATHFINDING
            if event.key == pygame.K_e:
                self.mode = StratApp.EDIT_PATH
            if event.key == pygame.K_r:
                self.mode = StratApp.RENDER_PATH
                self.render_path()
            self.mode_frame.set_current_mode(self.mode)
            self.dyn_container.load_content(self.mode)
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
        self.main_container.display()
        if self.mode in [StratApp.EDIT_PATH, StratApp.RENDER_PATH]:
            self.board.print_waypoints(self.constructed_path)
        if self.mode in [StratApp.TEST_PATHFINDING, StratApp.RENDER_PATH]:
            self.board.print_nodes(self.path_pathfinding)
    


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
            returned_value = self.board.pathfinding(wp0, wp1)
            if returned_value is not None:
                self.path_pathfinding = returned_value
                print("Astar from", wp0, " to ", wp1)
    
    def render_path(self):
        self.path_pathfinding = []
        if len(self.constructed_path) < 2 : return
        path = self.constructed_path.copy()
        wp0 = path.pop(0)
        wp1 = path.pop(0)
        returned_value = self.board.pathfinding(wp0, wp1)
        if returned_value is not None:
            self.path_pathfinding.extend(returned_value)
        for node in path: 
            wp0 = wp1
            wp1 = node
            returned_value = self.board.pathfinding(wp0,wp1)
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
