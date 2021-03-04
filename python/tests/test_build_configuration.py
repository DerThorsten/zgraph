import unittest
import zgraph


class TestBuildConfig(unittest.TestCase):
    def test_version(self):
        config = zgraph.BuildConfiguration
        self.assertEqual(config.VERSION_MAJOR, 0)
        self.assertEqual(config.VERSION_MINOR, 1)
        self.assertEqual(config.VERSION_PATCH, 0)
