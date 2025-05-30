#include "DirectedGraph.cpp"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

// 主函数
int main2() {
  DirectedGraph graph;

  // 提示用户输入文本文件路径
  cout << "Enter the path to your text file (e.g., 123.txt): ";
  string filePath;
  getline(cin, filePath);

  // 构建图
  graph.buildGraphFromText(filePath);

  cout << "\nGraph Processing System" << endl;
  cout << "-----------------------" << endl;

  while (true) {
    cout << "\nMenu:" << endl;
    cout << "1. Show directed graph (and save to file)" << endl;
    cout << "2. Query bridge words" << endl;
    cout << "3. Generate new text with bridge words" << endl;
    cout << "4. Calculate shortest path" << endl;
    cout << "5. Calculate PageRank" << endl;
    cout << "6. Random walk" << endl;
    cout << "7. Exit" << endl;
    cout << "Enter your choice (1-7): ";

    int choice;
    cin >> choice;
    cin.ignore(); // 清除输入缓冲区

    switch (choice) {
    case 1: {
      graph.showDirectedGraph();
      break;
    }
    case 2: {
      cout << "Enter two words (separated by space): ";
      string word1, word2;
      cin >> word1 >> word2;
      cout << graph.queryBridgeWords(word1, word2) << endl;
      break;
    }
    case 3: {
      cout << "Enter a line of text: ";
      string inputText;
      getline(cin, inputText);
      cout << "Generated text: " << graph.generateNewText(inputText) << endl;
      break;
    }
    case 4: {
      cout << "Enter one or two words (separated by space): ";
      string input;
      getline(cin, input);
      stringstream ss(input);
      string word1, word2;
      ss >> word1;
      ss >> word2;

      if (word2.empty()) {
        cout << graph.calcShortestPath(word1) << endl;
      } else {
        cout << graph.calcShortestPath(word1, word2) << endl;
      }
      break;
    }
    case 5: {
      cout << "Enter a word: ";
      string word;
      cin >> word;
      double pr = graph.calcPageRank(word);
      if (pr >= 0) {
        cout << "PageRank of '" << word << "': " << fixed << setprecision(4)
             << pr << endl;
      } else {
        cout << "Word '" << word << "' not found in the graph!" << endl;
      }
      break;
    }
    case 6: {
      string walkResult = graph.randomWalk();
      cout << "Random walk: " << walkResult << endl;
      break;
    }
    case 7: {
      cout << "Exiting..." << endl;
      return 0;
    }
    default: {
      cout << "Invalid choice. Please try again." << endl;
      break;
    }
    }
  }

  return 0;
}