class binary_heap:  # implementation that has no duplicate
    def __init__(self):
        self._A = [
            None
        ]  # the underlying data structure, we will ignore index 0, set index 0 to be 'dummy' (None in Python)

    # three helper navigation function, written in bit manipulation format to improve runtime speed, all three are fast O(1) functions
    def _parent(self, i):
        return (
            i >> 1
        )  # or i//2 , we use underscore to tell others that this is supposedely a private method

    def _left(self, i):
        return (
            i << 1
        )  # or i*2  , we use underscore to tell others that this is supposedely a private method

    def _right(self, i):
        return (
            i << 1
        ) + 1  # or i*2+1, we use underscore to tell others that this is supposedely a private method

    def _shift_up(
        self, i
    ):  # O(log n), we use underscore to tell others that this is supposedely a private method
        if i == 1:
            return  # at root, do nothing
        if self._A[i] > self._A[self._parent(i)]:  # violate property with parent
            self._A[i], self._A[self._parent(i)] = (
                self._A[self._parent(i)],
                self._A[i],
            )  # swap upwards
            self._shift_up(
                self._parent(i)
            )  # recurse, at most O(log n) steps back to the root

    def _shift_down(
        self, i
    ):  # O(log n), we use underscore to tell others that this is supposedely a private method
        swap_id = i
        if (
            self._left(i) < len(self._A) and self._A[i] < self._A[self._left(i)]
        ):  # compare with left child, if exists
            swap_id = self._left(i)
        if (
            self._right(i) < len(self._A) and self._A[swap_id] < self._A[self._right(i)]
        ):  # compare with right child, if exists
            swap_id = self._right(i)
        if swap_id != i:  # need to swap with the larger of the two children
            self._A[i], self._A[swap_id] = (
                self._A[swap_id],
                self._A[i],
            )  # swap downwards with the larger of the two children
            self._shift_down(
                swap_id
            )  # recurse, at most O(log n) steps to one of the bottom-most leaf

    def push(self, x):  # O(log n)
        self._A.append(
            x
        )  # append to the back of Python list, the only possible insertion point, O(1)
        self._shift_up(len(self._A) - 1)  # shift upwards, O(log n) at worst

    def pop(self):  # O(log n)
        if self.empty():
            return  # safeguard
        self._A[1], self._A[-1] = (
            self._A[-1],
            self._A[1],
        )  # swap with the last existing leaf, O(1)
        self._A.pop()  # reduce list size by one, O(1)
        self._shift_down(1)  # fix heap property downwards, O(log n) at worst

    def top(self):  # O(1)
        return self._A[1]  # this is the root

    def empty(self):  # O(1)
        return len(self._A) == 1  # when A = [None] only

    def size(self):  # O(1)
        return len(self._A) - 1  # excluding None at index 0


print("Our own Binary Heap")
pq = (
    binary_heap()
)  # this shows that binary (max) heap can be used as an efficient ADT Priority Queue
print(pq.empty())  # should be 1/true as pq content is []

pq.push(5)
print(pq.empty())  # should be 0/false as pq content is [5 (max)]

pq.push(2)
pq.push(7)
pq.push(3)
print(pq.top())  # should be 7 as pq content is [7 (max),5,3,2] -> [5 (max),3,2]
pq.pop()

pq.push(1)
print(pq.top())  # should be 5 as pq content is [5 (max),3,2,1]
print(pq.top())  # should still be 5 as pq content is [5 (max),3,2,1] -> [3 (max),2,1]
pq.pop()

pq.push(8)
pq.push(8)  # duplicates is ok
while not pq.empty():  # will print 8, 8, 3, 2, 1, in that order
    print(pq.top())
    pq.pop()


print()
print(
    "Python heapq version (ignore the -)"
)  # this is a MIN heap, to get a max heap, we negate all numbers
from heapq import heappush, heappop

pq_heapq = []
print(pq_heapq == [])  # should be True as pq_heapq content is []

heappush(pq_heapq, -5)  # insert negation
print(pq_heapq == [])  # should be False as pq_heapq content is [-5 (min)]

heappush(pq_heapq, -2)
heappush(pq_heapq, -7)
heappush(pq_heapq, -3)
print(pq_heapq[0])  # should be -7 as pq_heapq content is [-7 (min),-5,-3,-2]
heappop(pq_heapq)  # now it is [-5 (min),-3,-2]

heappush(pq_heapq, -1)
print(pq_heapq[0])  # should be -5 as pq_heapq content is [-5 (min),-3,-2,-1]
print(pq_heapq[0])  # should still be -5 as pq_heapq content is {-5 (min),-3,-2,-1]
heappop(pq_heapq)  # now it is [-3 (min),-2,-1]

heappush(pq_heapq, -8)
heappush(pq_heapq, -8)  # duplicates is ok
while pq_heapq:  # will print -8, -8, -3, -2, -1, in that order
    print(pq_heapq[0])
    heappop(pq_heapq)


print()
print("Equivalency testing on a very large test case")

import time, random

# large random test
begin = time.time()

ours = binary_heap()
theirs = []
assert ours.empty() and theirs == []  # both empty at the start
N = 100000  # usually just a few seconds (10x smaller than the C++ version)
for _ in range(N):  # insert N random integers to both data structures
    value = random.randint(0, 2**31 - 1)
    ours.push(value)
    heappush(theirs, -value)  # insert negation

assert not ours.empty() and theirs != []  # both not empty (has N entries) by now
assert ours.size() == len(theirs)  # heap size should match
while not ours.empty():
    assert (
        ours.top() == -theirs[0]
    )  # max value should match (note that theirs contains negative values)
    ours.pop()
    heappop(theirs)
    assert ours.size() == len(theirs)  # heap size should match
assert ours.empty() and theirs == []  # both empty at the end

print("Test time = %fs" % (time.time() - begin))
print("If there is no assertion (Run Time Error), then all is good")
