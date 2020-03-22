#!/usr/bin/env python3
import unittest
from castar import * 

class TestAstar(unittest.TestCase):

    def setUp(self):
        return super().setUp()
    
    def test_return_list(self):
        self.assertIsInstance(astar(Coordinates(10,10), Coordinates(100,100)), list, "Astar return a list")
    
    def test_return_exact_number_of_coords_straight_line(self):
        self.assertEqual(21,len(astar(Coordinates(0,0), 
        Coordinates(0,20))))


class TestAstarSimplified(unittest.TestCase):

    def setUp(self):
        return super().setUp()
    
    def test_return_list(self):
        self.assertIsInstance(astar_simplified(Coordinates(10,10), Coordinates(100,100)), list, "Astar return a list")
    
    def test_return_one_coord_straight_line(self):
        self.assertEqual(1,len(astar_simplified(Coordinates(10,10), Coordinates(10,100))))

class TestBoard(unittest.TestCase):

    def setUp(self):
        return super().setUp()
    
    def test_add_obstacle(self):
        self.assertTrue(add_obstacle(Rectangle(Coordinates(10,10),Size(20,20)))== None)
    
if __name__ == "__main__":
    unittest.main()