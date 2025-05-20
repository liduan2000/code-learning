#pragma once

#include <iostream>
#include <string>

#include "node.hpp"

class LinkedList {
  public:
    // create Linked List
    ListNode* createList() {
        std::string s_val;
        std::cin >> s_val;
        if (s_val == "#") { return nullptr; }
        ListNode* node = new ListNode(stoi(s_val));
        node->next = createList();
        return node;
    }

    // reverse the Linked List(iteration)
    ListNode* reverseListI(ListNode* head) {
        ListNode* newhead = new ListNode(0, head);
        ListNode* head_mem = head;
        while (head) {
            ListNode* cur_node = head;
            head = head->next;
            cur_node->next = newhead;
            newhead = cur_node;
        }
        ListNode* del_node = head_mem->next;
        head_mem->next = nullptr;
        delete del_node;
        return newhead;
    }

    // reverse the Linked List(recurtion)
    ListNode* reverseListR(ListNode* head) {
        if (!head || !head->next) { return head; }
        ListNode* node = reverseListR(head->next);
        head->next->next = head;
        head->next = nullptr;
        return node;
    }

    // traverse the Linked List
    void traverseList(ListNode* head) {
        while (head) {
            std::cout << head->val << " ";
            head = head->next;
        }
        std::cout << std::endl;
    }

    // delete the Linked List
    void deleteList(ListNode* head) {
        while (head) {
            ListNode* l = head;
            head = head->next;
            delete l;
        }
    }
};
