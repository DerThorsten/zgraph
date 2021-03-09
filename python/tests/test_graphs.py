import pytest
import numpy as np
import zgraph


@pytest.fixture()
def adjanceny_list_ugraph_chain():
    g = zgraph.AdjacencyListUGraph()
    for i in range(5):
        g.add_edge(i, i + 1)
    return g


@pytest.fixture()
def adjanceny_list_ugraph_grid_graph():
    g = zgraph.AdjacencyListUGraph()
    shape = [4, 5]
    node = lambda x, y: x * shape[1] + y
    for x in range(shape[0]):
        for y in range(shape[1]):
            if x + 1 < shape[0]:
                g.add_edge(node(x, y), node(x + 1, y))
            if y + 1 < shape[1]:
                g.add_edge(node(x, y), node(x, y + 1))
    return g


ugraphs = [
    pytest.lazy_fixture("adjanceny_list_ugraph_chain"),
    pytest.lazy_fixture("adjanceny_list_ugraph_grid_graph"),
]
digraphs = []
graph = ugraphs + digraphs
UGRAPHS = pytest.mark.parametrize("graph,", ugraphs)
DIGRAPHS = pytest.mark.parametrize("graph,", digraphs)
GRAPHS = pytest.mark.parametrize("graph,", graph)


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

    def test_edges(self):
        t = zgraph.AdjacencyListUGraph()
        for i in range(5):
            t.add_edge(i, i + 1)

        edges = t.edges_array()
        np.testing.assert_array_equal(edges, np.arange(t.num_edges()))

    def test_nodes(self):
        t = zgraph.AdjacencyListUGraph()
        for i in range(5):
            t.add_node(i)

        nodes = t.nodes_array()
        np.testing.assert_array_equal(nodes, np.arange(t.num_nodes()))

    def test_endpoints(self):
        t = zgraph.AdjacencyListUGraph()
        for i in range(3):
            t.add_edge(i, i + 1)

        endpoints = t.endpoints_array()
        np.testing.assert_array_equal(endpoints, np.array([[0, 1], [1, 2], [2, 3]]))


class TestGraph(object):
    @GRAPHS
    def test_edge_iter(self, graph):
        edges_from_iter = [edge for edge in graph.edge_iter()]
        assert len(edges_from_iter) == graph.num_edges()
        edges = graph.edges_array()
        np.testing.assert_array_equal(np.array(edges_from_iter), edges)

    @GRAPHS
    def test_node_iter(self, graph):
        nodes_from_iter = [node for node in graph.node_iter()]
        assert len(set(nodes_from_iter)) == len(nodes_from_iter)
        assert len(nodes_from_iter) == graph.num_nodes()
        nodes = graph.nodes_array()
        np.testing.assert_array_equal(np.array(nodes_from_iter), nodes)

    @GRAPHS
    def test_adjacency_iter(self, graph):

        edges = []
        endpoints = []

        for node_u in graph.node_iter():
            for node_v, edge in graph.adjacency_iter(node_u):
                edges.append(edge)
                endpoints.append((node_u, node_v))
                u, v = graph.endpoints(edge)

        assert len(edges) == 2 * graph.num_edges()

    @GRAPHS
    def test_adjacency_iter(self, graph):

        edges = []
        endpoints = []

        for node_u in graph.node_iter():
            for node_v, edge in graph.adjacency_iter(node_u):

                if node_u == node_v:
                    assert graph.allow_self_loops

                edges.append(edge)
                endpoints.append((node_u, node_v))

                u, v = graph.endpoints(edge)
                if u == v:
                    assert graph.allow_self_loops
                elif graph.is_directed:
                    assert u == node_u
                    assert v == node_v
                elif not graph.is_directed:

                    assert u in [node_u, node_v]
                    assert v in [node_u, node_v]

        # assert False
        assert len(edges) == 2 * graph.num_edges()


class TestUGraph(object):
    pass


class TestBfs(object):
    @GRAPHS
    def test_bfs(self, graph):

        bfs = zgraph.bfs_factory(graph=graph)

        nodes = [n for n in graph.node_iter()]

        # run
        bfs(start_nodes=nodes[0:2])
