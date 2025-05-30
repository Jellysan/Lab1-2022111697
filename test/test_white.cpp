#include "../DirectedGraph.cpp" // 主程序代码，含 randomWalk 实现
#include <fstream>
#include <gtest/gtest.h>

// 测试专用图类
class TestableDirectedGraph : public DirectedGraph {
public:
  void addTestEdge(const std::string &from, const std::string &to) {
    nodes.insert(from);
    nodes.insert(to);
    adjacencyList[from][to]++;
  }
};

class RandomWalkTest : public ::testing::Test {
protected:
  TestableDirectedGraph graph;
};

// 测试用例1：空图
TEST_F(RandomWalkTest, EmptyGraph) {
  std::string result = graph.randomWalk();
  EXPECT_EQ(result, "Graph is empty!");
}

// 测试用例2：单节点无出边
TEST_F(RandomWalkTest, SingleNodeNoEdges) {
  graph.nodes.insert("A");
  std::string result = graph.randomWalk();
  EXPECT_EQ(result, "A");
}

// 测试用例3：A->B
TEST_F(RandomWalkTest, OneEdge) {
  graph.addTestEdge("A", "B");
  std::string result = graph.randomWalk();
  EXPECT_TRUE(result == "A B" || result == "B");
}

// 测试用例4：A->B, B->A
TEST_F(RandomWalkTest, TwoEdgesCycle) {
  graph.addTestEdge("A", "B");
  graph.addTestEdge("B", "A");
  std::string result = graph.randomWalk();
  EXPECT_TRUE(result == "A B A" || result == "B A B" || result == "A" ||
              result == "B");
}

// 测试用例5：A->B, B->C, B->A
TEST_F(RandomWalkTest, ThreeEdgesComplex) {
  graph.addTestEdge("A", "B");
  graph.addTestEdge("B", "C");
  graph.addTestEdge("B", "A");
  std::string result = graph.randomWalk();
  EXPECT_TRUE(result == "A B C" || result == "B C" || result == "A B A" ||
              result == "C" || result == "B A B C");
}

// 测试用例6：A->B, B->C, C->B
TEST_F(RandomWalkTest, ThreeEdgesCycle) {
  graph.addTestEdge("A", "B");
  graph.addTestEdge("B", "C");
  graph.addTestEdge("C", "B");
  EXPECT_NO_THROW({
    std::string result = graph.randomWalk();
    EXPECT_FALSE(result.empty());
  });
}

// 测试用例7：A->B, B->C, C->D
TEST_F(RandomWalkTest, FourNodesChain) {
  graph.addTestEdge("A", "B");
  graph.addTestEdge("B", "C");
  graph.addTestEdge("C", "D");
  std::string result = graph.randomWalk();
  EXPECT_TRUE(result == "A B C D" || result == "B C D" || result == "C D" ||
              result == "D");
}

// 测试用例8：文件保存成功
TEST_F(RandomWalkTest, FileWriteSuccess) {
  graph.addTestEdge("A", "B");

  // 捕获控制台输出
  testing::internal::CaptureStdout();
  std::string result = graph.randomWalk();
  std::string output = testing::internal::GetCapturedStdout();

  // 输出验证
  EXPECT_TRUE(result == "A B" || result == "B" || result == "B A");
  EXPECT_NE(output.find(
                "Random walk result saved to "
                "D:\\Code\\Clion\\software_engineering\\lab1\\random_walk.txt"),
            std::string::npos)
      << "应包含保存成功提示";
}

// 主函数
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
