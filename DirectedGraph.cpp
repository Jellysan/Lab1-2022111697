#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <vector>
using namespace std;

// 有向图类定义
class DirectedGraph {
private:
  // 邻接表表示的有向图
  // 所有节点集合
  map<string, double> pageRank; // PageRank值存储

  // 预处理文本：转换为小写，移除标点符号和非字母字符
  string preprocessText(const string &text) {
    string result;
    for (char c : text) {
      if (isalpha(c)) {
        result += tolower(c);
      } else if (isspace(c) || c == '\n' || c == '\r') {
        result += ' ';
      }
      // 忽略其他所有字符
    }
    return result;
  }

  // 分割文本为单词列表
  vector<string> splitWords(const string &text) {
    vector<string> words;
    stringstream ss(text);
    string word;
    while (ss >> word) {
      words.push_back(word);
    }
    return words;
  }

  // 生成DOT格式的图描述
  string generateDotFormat() const {
    stringstream dot;
    dot << "digraph G {\n";
    dot << "  rankdir=LR;\n";
    dot << "  node [shape=circle];\n";

    // 添加所有节点
    for (const auto &node : nodes) {
      dot << "  \"" << node << "\";\n";
    }

    // 添加所有边
    for (const auto &from : adjacencyList) {
      for (const auto &to : from.second) {
        dot << "  \"" << from.first << "\" -> \"" << to.first << "\" [label=\""
            << to.second << "\"];\n";
      }
    }

    dot << "}\n";
    return dot.str();
  }

protected:
  set<string> nodes;

  map<string, map<string, int>> adjacencyList;

public:
  // 从文本构建有向图
  void buildGraphFromText(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
      cerr << "Error: Could not open file " << filename << endl;
      return;
    }

    string line;
    string fullText;
    while (getline(file, line)) {
      fullText += preprocessText(line) + " ";
    }
    file.close();

    vector<string> words = splitWords(fullText);
    if (words.empty()) {
      cerr << "Error: No valid words found in the file." << endl;
      return;
    }

    // 构建邻接表
    for (size_t i = 0; i < words.size() - 1; i++) {
      string from = words[i];
      string to = words[i + 1];
      nodes.insert(from);
      nodes.insert(to);
      adjacencyList[from][to]++;
    }
  }

  // 展示有向图并保存为图形文件
  void showDirectedGraph() {
    cout << "Directed Graph:" << endl;
    cout << "----------------" << endl;
    for (const auto &node : nodes) {
      cout << node << " -> ";
      if (adjacencyList.find(node) != adjacencyList.end()) {
        bool first = true;
        for (const auto &edge : adjacencyList.at(node)) {
          if (!first)
            cout << ", ";
          cout << edge.first << "(" << edge.second << ")";
          first = false;
        }
      }
      cout << endl;
    }
    cout << "----------------" << endl;

    // 生成并保存 DOT 文件
    std::string dotContent = generateDotFormat();
    std::ofstream dotFile("graph.dot");
    if (dotFile.is_open()) {
      dotFile << dotContent;
      dotFile.close();
      std::cout << "Graph DOT file saved as graph.dot" << std::endl;

      // 尝试使用Graphviz生成图片，并指定输出路径
      std::string outputPath =
          "D:/Code/Clion/software_engineering/lab1/graph.png";
#ifdef _WIN32
      std::string command = "dot -Tpng graph.dot -o " + outputPath;
      system(command.c_str());
#else
      std::string command =
          "dot -Tpng graph.dot -o " + outputPath + " 2>/dev/null";
      system(command.c_str());
#endif
      std::cout << "Graph image saved as " << outputPath
                << " (if Graphviz is installed)" << std::endl;
    } else {
      std::cerr << "Error: Could not save graph DOT file" << std::endl;
    }
  }

  // 查询桥接词（改进输出格式）
  string queryBridgeWords(const string &word1, const string &word2) {
    string w1 = word1;
    string w2 = word2;
    transform(w1.begin(), w1.end(), w1.begin(), ::tolower);
    transform(w2.begin(), w2.end(), w2.begin(), ::tolower);

    // 检查单词是否存在
    if (nodes.find(w1) == nodes.end() && nodes.find(w2) == nodes.end()) {
      return "No \"" + word1 + "\" or \"" + word2 + "\" in the graph!";
    } else if (nodes.find(w1) == nodes.end()) {
      return "No \"" + word1 + "\" in the graph!";
    } else if (nodes.find(w2) == nodes.end()) {
      return "No \"" + word2 + "\" in the graph!";
    }

    set<string> bridgeWords;
    // 获取word1的所有出边节点
    if (adjacencyList.find(w1) != adjacencyList.end()) {
      for (const auto &edge : adjacencyList.at(w1)) {
        string potentialBridge = edge.first;
        // 检查potentialBridge是否有到word2的边
        if (adjacencyList.find(potentialBridge) != adjacencyList.end() &&
            adjacencyList.at(potentialBridge).find(w2) !=
                adjacencyList.at(potentialBridge).end()) {
          bridgeWords.insert(potentialBridge);
        }
      }
    }

    if (bridgeWords.empty()) {
      return "No bridge words from \"" + word1 + "\" to \"" + word2 + "\"!";
    }

    string result =
        "The bridge words from \"" + word1 + "\" to \"" + word2 + "\" ";
    if (bridgeWords.size() == 1) {
      result += "is: \"" + *bridgeWords.begin() + "\"";
    } else {
      result += "are: ";
      size_t count = 0;
      for (const string &word : bridgeWords) {
        if (count > 0) {
          if (count == bridgeWords.size() - 1) {
            result += " and ";
          } else {
            result += ", ";
          }
        }
        result += "\"" + word + "\"";
        count++;
      }
    }
    result += ".";

    return result;
  }

  // 根据桥接词生成新文本
  string generateNewText(const string &inputText) {
    string processedText = preprocessText(inputText);
    vector<string> words = splitWords(processedText);
    if (words.size() < 2) {
      return inputText; // 不足两个单词，无法插入桥接词
    }

    string result;
    random_device rd;
    mt19937 gen(rd());

    for (size_t i = 0; i < words.size() - 1; i++) {
      result += words[i] + " ";

      string w1 = words[i];
      string w2 = words[i + 1];

      // 检查单词是否存在
      if (nodes.find(w1) == nodes.end() || nodes.find(w2) == nodes.end()) {
        continue;
      }

      set<string> bridgeWords;
      // 获取桥接词
      if (adjacencyList.find(w1) != adjacencyList.end()) {
        for (const auto &edge : adjacencyList.at(w1)) {
          string potentialBridge = edge.first;
          if (adjacencyList.find(potentialBridge) != adjacencyList.end() &&
              adjacencyList.at(potentialBridge).find(w2) !=
                  adjacencyList.at(potentialBridge).end()) {
            bridgeWords.insert(potentialBridge);
          }
        }
      }

      if (!bridgeWords.empty()) {
        // 随机选择一个桥接词
        uniform_int_distribution<> dis(0, bridgeWords.size() - 1);
        int index = dis(gen);
        auto it = bridgeWords.begin();
        advance(it, index);
        result += *it + " ";
      }
    }
    result += words.back(); // 添加最后一个单词

    return result;
  }

  // 计算两个单词之间的最短路径（增加单单词功能）
  string calcShortestPath(const string &word1, const string &word2 = "") {
    string w1 = word1;
    string w2 = word2;
    transform(w1.begin(), w1.end(), w1.begin(), ::tolower);
    transform(w2.begin(), w2.end(), w2.begin(), ::tolower);

    // 检查单词是否存在
    if (nodes.find(w1) == nodes.end()) {
      return "No \"" + word1 + "\" in the graph!";
    }

    // 单单词模式：计算到所有其他节点的最短路径
    if (w2.empty()) {
      string result = "Shortest paths from \"" + word1 + "\":\n";

      // Dijkstra算法实现（单源最短路径）
      map<string, int> distances;
      map<string, string> previous;
      priority_queue<pair<int, string>, vector<pair<int, string>>,
                     greater<pair<int, string>>>
          pq;

      // 初始化距离
      for (const string &node : nodes) {
        distances[node] = numeric_limits<int>::max();
      }
      distances[w1] = 0;
      pq.push({0, w1});

      while (!pq.empty()) {
        string current = pq.top().second;
        pq.pop();

        if (adjacencyList.find(current) == adjacencyList.end()) {
          continue; // 当前节点没有出边
        }

        for (const auto &edge : adjacencyList.at(current)) {
          string neighbor = edge.first;
          int weight = edge.second;
          int newDist = distances[current] + weight;

          if (newDist < distances[neighbor]) {
            distances[neighbor] = newDist;
            previous[neighbor] = current;
            pq.push({newDist, neighbor});
          }
        }
      }

      // 输出所有可达路径
      for (const string &node : nodes) {
        if (node == w1)
          continue;

        if (distances[node] == numeric_limits<int>::max()) {
          result += "  No path to \"" + node + "\"\n";
          continue;
        }

        // 构建路径字符串
        stack<string> path;
        string current = node;
        while (current != w1) {
          path.push(current);
          current = previous[current];
        }
        path.push(w1);

        result += "  To \"" + node + "\": ";
        while (!path.empty()) {
          result += path.top();
          path.pop();
          if (!path.empty()) {
            result += " -> ";
          }
        }
        result += " (Length: " + to_string(distances[node]) + ")\n";
      }

      return result;
    }

    // 双单词模式：原功能
    if (nodes.find(w2) == nodes.end()) {
      return "No \"" + word2 + "\" in the graph!";
    }

    // Dijkstra算法实现
    map<string, int> distances;
    map<string, string> previous;
    priority_queue<pair<int, string>, vector<pair<int, string>>,
                   greater<pair<int, string>>>
        pq;

    // 初始化距离
    for (const string &node : nodes) {
      distances[node] = numeric_limits<int>::max();
    }
    distances[w1] = 0;
    pq.push({0, w1});

    while (!pq.empty()) {
      string current = pq.top().second;
      pq.pop();

      if (current == w2) {
        break; // 找到目标节点
      }

      if (adjacencyList.find(current) == adjacencyList.end()) {
        continue; // 当前节点没有出边
      }

      for (const auto &edge : adjacencyList.at(current)) {
        string neighbor = edge.first;
        int weight = edge.second;
        int newDist = distances[current] + weight;

        if (newDist < distances[neighbor]) {
          distances[neighbor] = newDist;
          previous[neighbor] = current;
          pq.push({newDist, neighbor});
        }
      }
    }

    if (distances[w2] == numeric_limits<int>::max()) {
      return "No path from \"" + word1 + "\" to \"" + word2 + "\"!";
    }

    // 构建路径字符串
    stack<string> path;
    string current = w2;
    while (current != w1) {
      path.push(current);
      current = previous[current];
    }
    path.push(w1);

    string result;
    while (!path.empty()) {
      result += path.top();
      path.pop();
      if (!path.empty()) {
        result += " -> ";
      }
    }
    result += " (Length: " + to_string(distances[w2]) + ")";

    return result;
  }

  // 计算PageRank
  double calcPageRank(const string &word, double d = 0.85,
                      int iterations = 100) {
    string w = word;
    transform(w.begin(), w.end(), w.begin(), ::tolower);

    if (nodes.find(w) == nodes.end()) {
      return -1.0; // 单词不存在
    }

    // 初始化PageRank值
    double initialRank = 1.0 / nodes.size();
    for (const string &node : nodes) {
      pageRank[node] = initialRank;
    }

    // 迭代计算PageRank
    for (int i = 0; i < iterations; i++) {
      map<string, double> newRank;

      // 计算每个节点的新PR值
      for (const string &node : nodes) {
        double sum = 0.0;

        // 找出所有指向node的节点
        for (const string &inNode : nodes) {
          if (adjacencyList.find(inNode) != adjacencyList.end() &&
              adjacencyList[inNode].find(node) != adjacencyList[inNode].end()) {
            // 计算贡献值: PR(inNode) / L(inNode)
            sum += pageRank[inNode] / adjacencyList[inNode].size();
          }
        }

        newRank[node] = (1.0 - d) / nodes.size() + d * sum;
      }

      // 更新PageRank值
      for (const auto &entry : newRank) {
        pageRank[entry.first] = entry.second;
      }
    }

    return pageRank[w];
  }

  // 随机游走（改进文件保存）
  string randomWalk() {
    if (nodes.empty()) {
      return "Graph is empty!";
    }

    // 随机选择一个起始节点
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, nodes.size() - 1);
    auto it = nodes.begin();
    advance(it, dis(gen));
    string current = *it;

    set<pair<string, string>> visitedEdges;
    string result;
    result += current;

    while (true) {
      // 检查当前节点是否有出边
      if (adjacencyList.find(current) == adjacencyList.end() ||
          adjacencyList[current].empty()) {
        break;
      }

      // 随机选择一个出边
      vector<pair<string, int>> edges(adjacencyList[current].begin(),
                                      adjacencyList[current].end());
      uniform_int_distribution<> edgeDis(0, edges.size() - 1);
      int edgeIndex = edgeDis(gen);
      string next = edges[edgeIndex].first;

      // 检查是否已经访问过这条边
      if (visitedEdges.find({current, next}) != visitedEdges.end()) {
        break;
      }

      visitedEdges.insert({current, next});
      current = next;
      result += " " + current;
    }

    // 将结果写入文件
    ofstream outFile(
        "D:\\Code\\Clion\\software_engineering\\lab1\\random_walk.txt");
    if (outFile.is_open()) {
      outFile << result;
      outFile.close();
      cout << "Random walk result saved to "
              "D:\\Code\\Clion\\software_engineering\\lab1\\random_walk.txt"
           << endl;
    } else {
      cerr << "Error: Could not save random walk result" << endl;
    }

    return result;
  }
};
