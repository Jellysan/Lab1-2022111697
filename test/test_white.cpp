#include "../DirectedGraph.cpp" // 包含主程序代码
#include <gtest/gtest.h>

// 创建测试图类
class TestableDirectedGraph : public DirectedGraph {
public:
  void addTestEdge(const std::string &from, const std::string &to) {
    nodes.insert(from);
    nodes.insert(to);
    adjacencyList[from][to]++;
  }

  void clearGraph() {
    nodes.clear();
    adjacencyList.clear();
  }
};

class RandomWalkTest : public ::testing::Test {
protected:
  TestableDirectedGraph graph;

  void SetUp() override { graph.clearGraph(); }
};

// 测试用例1 - 正常游走后遇重复边终止
TEST_F(RandomWalkTest, WalkAndEndOnVisitedEdge) {
  graph.addTestEdge("scientist", "carefully");
  graph.addTestEdge("carefully", "analyzed");
  graph.addTestEdge("analyzed", "data");
  graph.addTestEdge("data", "scientist"); // 形成环

  std::string result = graph.randomWalk();
  EXPECT_TRUE(result.find("scientist") != std::string::npos);
  EXPECT_TRUE(result.find("carefully") != std::string::npos);
  EXPECT_TRUE(result.find("analyzed") != std::string::npos);
  EXPECT_TRUE(result.find("data") != std::string::npos);
}

// 测试用例2 - 起始节点无出边
TEST_F(RandomWalkTest, StartNodeWithoutOutEdges) {
  graph.nodes.insert("scientist"); // 无出边
  std::string result = graph.randomWalk();
  EXPECT_EQ(result, "scientist");
}

// 测试用例3 - 起始边即重复
TEST_F(RandomWalkTest, FirstEdgeAlreadyVisited) {
  graph.addTestEdge("scientist", "data");
  graph.addTestEdge("data", "scientist");

  // 手动模拟已访问过的边（通过调用一次 randomWalk 让它游走，第二次立即终止）
  std::string first = graph.randomWalk();  // 第一次会执行一次边
  std::string second = graph.randomWalk(); // 再次调用，应立刻遇到重复边终止

  EXPECT_TRUE(second.size() <= first.size());
}

// 测试用例4 - 游走一条边后终止
TEST_F(RandomWalkTest, WalkOneEdgeThenStop) {
  graph.addTestEdge("scientist", "again");
  // again 无出边
  std::string result = graph.randomWalk();
  EXPECT_EQ(result, "scientist again");
}

// 测试用例5 - 空图
TEST_F(RandomWalkTest, EmptyGraph) {
  std::string result = graph.randomWalk();
  EXPECT_EQ(result, "Graph is empty!");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
