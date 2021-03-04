import zgraph


class TestAdjacencyListUGraph(object):
    def test_init(self):
        t = zgraph.AdjacencyListUGraph()
        assert isinstance(t, zgraph.AdjacencyListUGraph)

    def test_add_node(self):
        t = zgraph.AdjacencyListUGraph()
        for i in range(5):
            t.add_node(i)

    def test_add_edge(self):
        t = zgraph.AdjacencyListUGraph()
        for i in range(5):
            t.add_edge(i, i + 1)
