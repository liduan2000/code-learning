#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>

using namespace std;

// Linked List structure
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr){};
    ListNode(int v) : val(v), next(nullptr){};
    ListNode(int v, ListNode *n) : val(v), next(n){};
};

class OneWayList {
   public:
    // Create Linked List
    ListNode *createList() {
        string s_val;
        cin >> s_val;
        if (s_val == "#") return nullptr;
        ListNode *node = new ListNode(stoi(s_val));
        node->next = createList();
        return node;
    }

    // reverse the Linked List(iteration)
    ListNode *reverseListI(ListNode *head) {
        ListNode *newhead = new ListNode(0, head);
        ListNode *head_mem = head;
        while (head) {
            ListNode *cur_node = head;
            head = head->next;
            cur_node->next = newhead;
            newhead = cur_node;
        }
        ListNode *del_node = head_mem->next;
        head_mem->next = nullptr;
        delete del_node;
        return newhead;
    }

    // reverse the Linked List(recurtion)
    ListNode *reverseListR(ListNode *head) {
        if (!head || !head->next) {
            return head;
        }
        ListNode *node = reverseListRecur(head->next);
        head->next->next = head;
        head->next = nullptr;
        return node;
    }

    // traverse the Linked List
    void traverseList(ListNode *head) {
        while (head) {
            cout << head->val << " ";
            head = head->next;
        }
        cout << endl;
    }

    // delete the Linked List
    void deleteList(ListNode *head) {
        while (head) {
            ListNode *l = head;
            head = head->next;
            delete l;
        }
    }
};

int main() {
    OneWayList list;
    ListNode *head = list.createList();
    list.traverseList(head);
    head = list.reverseListI(head);
    list.traverseList(head);
    list.deleteList(head);
}
