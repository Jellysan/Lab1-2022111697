#include "../DirectedGraph.cpp" // 包含主程序代码
#include <gtest/gtest.h>

// 创建一个测试专用的图类
class TestableDirectedGraph : public DirectedGraph {
public:
  // 添加测试辅助方法
  void addTestEdge(const std::string &from, const std::string &to) {
    nodes.insert(from);
    nodes.insert(to);
    adjacencyList[from][to]++;
  }
};

class BridgeWordsTest : public ::testing::Test {
protected:
  TestableDirectedGraph graph;

  void SetUp() override {
    // 构建测试图
    graph.addTestEdge("to", "explore");
    graph.addTestEdge("explore", "strange");
    graph.addTestEdge("strange", "new");
    graph.addTestEdge("new", "world");
    graph.addTestEdge("the", "report");
    graph.addTestEdge("scientist", "carefully");
    graph.addTestEdge("carefully", "data");
    graph.addTestEdge("scientist", "again");
    graph.addTestEdge("again", "data");
  }
};

// 测试用例1 - 单桥接词
TEST_F(BridgeWordsTest, SingleBridgeWord) {
  string result = graph.queryBridgeWords("strange", "world");
  EXPECT_TRUE(result.find("new") != string::npos) << "应包含桥接词'new'";
  EXPECT_EQ(result,
            "The bridge words from \"strange\" to \"world\" is: \"new\".");
}

// 测试用例2 - 多桥接词
TEST_F(BridgeWordsTest, MultipleBridgeWords) {
  string result = graph.queryBridgeWords("scientist", "data");
  bool has_carefully = result.find("carefully") != string::npos;
  bool has_again = result.find("again") != string::npos;

  EXPECT_TRUE(has_carefully && has_again) << "应包含'carefully'和'again'";
  EXPECT_TRUE(result.find("are:") != string::npos) << "多桥接词应使用复数形式";
}

// 测试用例3 - 无桥接词
TEST_F(BridgeWordsTest, NoBridgeWords) {
  EXPECT_EQ(graph.queryBridgeWords("the", "report"),
            "No bridge words from \"the\" to \"report\"!");
}

// 测试用例4 - 无效单词
TEST_F(BridgeWordsTest, NonExistentWord) {
  EXPECT_EQ(graph.queryBridgeWords("to", "hagongda"),
            "No \"hagongda\" in the graph!");
}

// 测试用例5 - 空输入
TEST_F(BridgeWordsTest, EmptyInput) {
  EXPECT_EQ(graph.queryBridgeWords(",", "yes"),
            "No \",\" or \"yes\" in the graph!");
}

// 测试用例6 - 大小写转换
TEST_F(BridgeWordsTest, CaseInsensitive) {
  string result = graph.queryBridgeWords("TO", "STRANGE");
  EXPECT_EQ(result,
            "The bridge words from \"TO\" to \"STRANGE\" is: \"explore\".");
}

// 测试用例7 - 自环
TEST_F(BridgeWordsTest, Selfcircle) {
  EXPECT_EQ(graph.queryBridgeWords("carefully", "carefully"),
            "No bridge words from \"carefully\" to \"carefully\"!");
}

int main2(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}