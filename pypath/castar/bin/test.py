#!/usr/bin/env python3
import unittest
from castar import * 

class TestAstar(unittest.TestCase):

    def setUp(self):
        self.field = Field(200)
        self.castar = Castar()
        return super().setUp()
    
    def test_return_tuple(self):
        self.assertIsInstance(self.castar.find_path(Coordinates(10,10), Coordinates(100,100),self.field), tuple, "Astar return a list")
    
    def test_return_exact_number_of_coords_straight_line(self):
        err, path = self.castar.find_path(Coordinates(0,0), Coordinates(0,20),self.field)
        self.assertEqual(21,len(path))
        self.assertTrue(err == 0)


class TestAstarSimplified(unittest.TestCase):

    def setUp(self):
        self.field = Field(200)
        self.castar = Castar()
        return super().setUp()
    
    def test_return_tuple(self):
        self.assertIsInstance(self.castar.find_path_simplified(Coordinates(10,10), Coordinates(100,100), self.field), tuple, "Astar return a list")
    
    def test_return_one_coord_straight_line(self):
        err, path = self.castar.find_path_simplified(Coordinates(10,10), Coordinates(10,100), self.field)
        self.assertEqual(1,len(path))
        self.assertTrue(err == 0)
    
    def test_complex_path(self):
        self.field.add_obstacle(Rectangle(Coordinates(60,20), Size(2,40)))
        self.field.add_obstacle(Rectangle(Coordinates(150,40), Size(10,40)))
        self.field.add_obstacle(Rectangle(Coordinates(80,80), Size(200,2)))
        err, path = self.castar.find_path_simplified(Coordinates(10,10), Coordinates(10,100), self.field)
        self.assertTrue(err == 0)

class TestField(unittest.TestCase):

    def setUp(self):
        self.field = Field(200)
        return super().setUp()
    
    def test_add_obstacle(self):
        self.assertTrue(self.field.add_obstacle(Rectangle(Coordinates(10,10), Size(20,20))) == None)
    
    def test_delete_obstacle(self):
        self.assertTrue(self.field.add_obstacle(Rectangle(Coordinates(10,10), Size(20,20))) == None)
        self.assertTrue(self.field.delete_obstacle(Coordinates(10,10)))
    
    def test_delete_enough_obstacle(self):
        self.assertTrue(self.field.add_obstacle(Rectangle(Coordinates(10,10), Size(20,20))) == None)
        self.assertTrue(self.field.add_obstacle(Rectangle(Coordinates(10,10), Size(20,200))) == None)
        self.assertTrue(self.field.delete_obstacle(Coordinates(10,10)))
        self.assertTrue(self.field.delete_obstacle(Coordinates(10,10)))
        self.assertTrue(self.field.delete_obstacle(Coordinates(10,10)) == False)

    
if __name__ == "__main__":
    unittest.main()