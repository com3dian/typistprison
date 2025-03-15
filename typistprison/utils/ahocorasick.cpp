#include "ahocorasick.h"

AhoCorasick::AhoCorasick() {
    root = new TrieNode();
}

void AhoCorasick::insert(const std::string& word, int index) {
    TrieNode* curr = root;
    for (char c : word) {
        if (!curr->children.count(c)) {
            curr->children[c] = new TrieNode();
        }
        curr = curr->children[c];
    }
    curr->output.push_back(index);
}

void AhoCorasick::buildFailureLinks() {
    std::queue<TrieNode*> q;
    for (auto& [c, child] : root->children) {
        child->failLink = root;
        q.push(child);
    }
    while (!q.empty()) {
        TrieNode* curr = q.front();
        q.pop();

        for (auto& [c, child] : curr->children) {
            TrieNode* fail = curr->failLink;
            while (fail && !fail->children.count(c)) {
                fail = fail->failLink;
            }
            child->failLink = fail ? fail->children[c] : root;
            child->output.insert(child->output.end(),
                                 child->failLink->output.begin(),
                                 child->failLink->output.end());
            q.push(child);
        }
    }
}

std::vector<std::pair<int, int>> AhoCorasick::search(const std::string& text) {
    TrieNode* curr = root;
    std::vector<std::pair<int, int>> result;
    for (int i = 0; i < text.size(); i++) {
        char c = text[i];
        while (curr && !curr->children.count(c)) {
            curr = curr->failLink;
        }
        curr = curr ? curr->children[c] : root;
        for (int index : curr->output) {
            result.push_back(std::make_pair(index, i));
        }
    }
    return result;
}