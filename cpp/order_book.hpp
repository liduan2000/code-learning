#pragma once

#include <cstdint>
#include <deque>
#include <format>
#include <iostream>
#include <map>
#include <unordered_map>

enum class OrderType { LIMIT, MARKET };

enum class OrderSide { BUY, SELL };

struct Order {
    uint64_t id;
    OrderType type;
    OrderSide side;
    double price;
    double qty;
    uint64_t timestamp;
};

class Orderbook {
  public:
    void placeOrder(Order& order) {
        if (order.type == OrderType::LIMIT) {
            if (order.side == OrderSide::BUY) {
                bids_[order.price].push_back(order);
                orderMap_[order.id] = std::make_pair(order.price, std::prev(bids_[order.price].end()));
            } else {
                asks_[order.price].push_back(order);
                orderMap_[order.id] = std::make_pair(order.price, std::prev(asks_[order.price].end()));
            }
            matchOrders();
        } else {
            executeMarketOrder(order);
        }
    }

    void amendOrder(Order& order) {
        auto it = orderMap_.find(order.id);
        if (it == orderMap_.end()) {
            std::cout << std::format("order id {} not found\n", order.id);
            return;
        }

        auto [old_price, old_it] = it->second;
        if (old_price == order.price) {
            old_it->qty = order.qty;
            old_it->timestamp = order.timestamp;
            return;
        }

        OrderSide side = old_it->side;
        if (old_it->side == OrderSide::BUY) {
            bids_[old_price].erase(old_it);
            if (bids_[old_price].empty()) { bids_.erase(old_price); }
            bids_[order.price].push_back(order);
            orderMap_[order.id] = {order.price, std::prev(bids_[order.price].end())};
        } else {
            asks_[old_price].erase(old_it);
            if (asks_[old_price].empty()) { asks_.erase(old_price); }
            asks_[order.price].push_back(order);
            orderMap_[order.id] = {order.price, std::prev(asks_[order.price].end())};
        }
    }

    void cancelOrder(uint64_t order_id) {
        auto it = orderMap_.find(order_id);
        if (it == orderMap_.end()) {
            std::cout << std::format("order id {} not found\n", order_id);
            return;
        }
        auto [price, order_it] = it->second;
        if (order_it->side == OrderSide::BUY) {
            bids_[price].erase(order_it);
            if (bids_[price].empty()) { bids_.erase(price); }
        } else {
            asks_[price].erase(order_it);
            if (asks_[price].empty()) { asks_.erase(price); }
        }
        orderMap_.erase(it);
    }

    void printOrderbook() {
        std::cout << "Asks:\n";
        for (const auto& [price, orders] : asks_) {
            for (const auto& order : orders) {
                std::cout << std::format("SELL: id={}, price={}, qty={}, timestamp={}\n", order.id, order.price,
                                         order.qty, order.timestamp);
            }
        }
        std::cout << "Bids:\n";
        for (const auto& [price, orders] : bids_) {
            for (const auto& order : orders) {
                std::cout << std::format("BUY: id={}, price={}, qty={}, timestamp={}\n", order.id, order.price,
                                         order.qty, order.timestamp);
            }
        }
    }

  private:
    void executeMarketOrder(Order& order) {
        if (order.side == OrderSide::BUY) {
            while (order.qty > 0. && !asks_.empty()) {
                auto& bestAsk = asks_.begin()->second.front();
                double toSub = std::min(order.qty, bestAsk.qty);
                order.qty -= toSub;
                bestAsk.qty -= toSub;
                if (bestAsk.qty == 0.) {
                    orderMap_.erase(asks_.begin()->second.front().id);
                    asks_.begin()->second.pop_front();
                }
                if (asks_.begin()->second.empty()) { asks_.erase(asks_.begin()); }
            }
        } else {
            while (order.qty > 0. && !bids_.empty()) {
                auto& bestBid = bids_.begin()->second.front();
                double toSub = std::min(order.qty, bestBid.qty);
                order.qty -= toSub;
                bestBid.qty -= toSub;
                if (bestBid.qty == 0.) {
                    orderMap_.erase(bids_.begin()->second.front().id);
                    bids_.begin()->second.pop_front();
                }
                if (bids_.begin()->second.empty()) { bids_.erase(bids_.begin()); }
            }
        }
    }

    void matchOrders() {
        while (!bids_.empty() && !asks_.empty() && bids_.begin()->first >= asks_.begin()->first) {
            auto& [bidPrice, bestBids] = *bids_.begin();
            auto& [askPrice, bestAsks] = *asks_.begin();
            while (!bestBids.empty() && !bestAsks.empty()) {
                double toSub = std::min(bestBids.front().qty, bestAsks.front().qty);
                bestBids.front().qty -= toSub;
                bestAsks.front().qty -= toSub;
                if (bestBids.front().qty == 0.) {
                    orderMap_.erase(bestBids.front().id);
                    bestBids.pop_front();
                }
                if (bestAsks.front().qty == 0.) {
                    orderMap_.erase(bestAsks.front().id);
                    bestAsks.pop_front();
                }
            }
            if (bestBids.empty()) { bids_.erase(bids_.begin()); }
            if (bestAsks.empty()) { asks_.erase(asks_.begin()); }
        }
    }

    std::map<double, std::deque<Order>, std::greater<>> bids_; // desc
    std::map<double, std::deque<Order>, std::less<>> asks_;    // asc
    // order_id : {price, it}
    std::unordered_map<uint64_t, std::pair<double, std::deque<Order>::iterator>> orderMap_;
};
