#pragma once

#include <string>
#include <unordered_map>

class Trie {
  public:
    Trie() { root = new Node(); }

    ~Trie() { clear(root); }

    void insert(const std::string& word) {
        Node* node = root;
        for (char c : word) {
            if (!node->children.contains(c)) { node->children[c] = new Node(); }
            node = node->children[c];
        }
        node->isEnd = true;
    }

    bool search(const std::string& word) {
        Node* node = root;
        for (char c : word) {
            if (!node->children.contains(c)) { return false; }
            node = node->children[c];
        }
        return node->isEnd;
    }

    bool startsWith(const std::string& prefix) {
        Node* node = root;
        for (char c : prefix) {
            if (!node->children.contains(c)) { return false; }
            node = node->children[c];
        }
        return true;
    }

  private:
    struct Node {
        std::unordered_map<char, Node*> children;
        bool isEnd;

        Node() : isEnd(false) {}
    };

    void clear(Node* node) {
        for (auto& [_, child] : node->children) { clear(child); }
        delete node;
    }

    Node* root;
};
