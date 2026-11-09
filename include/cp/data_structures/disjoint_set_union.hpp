#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>

namespace cp {

class DisjointSetUnion {
public:
    explicit DisjointSetUnion(std::size_t size) : nodes_(size), components_(size)
    {
        for (std::size_t index = 0; index < size; ++index) {
            nodes_[index].parent = index;
        }
    }

    [[nodiscard]] std::size_t size() const noexcept { return nodes_.size(); }
    [[nodiscard]] std::size_t components() const noexcept
    {
        return nodes_.empty() ? 0 : components_;
    }

    [[nodiscard]] std::size_t find(std::size_t index)
    {
        if (index >= size()) {
            throw std::out_of_range("index is outside the disjoint set union");
        }
        while (index != nodes_[index].parent) {
            // Skip one level of the parent chain (path halving).
            nodes_[index].parent = nodes_[nodes_[index].parent].parent;
            index = nodes_[index].parent;
        }
        return index;
    }

    bool unite(std::size_t left, std::size_t right)
    {
        auto left_root = find(left);
        auto right_root = find(right);
        if (left_root == right_root) {
            return false;
        }
        if (nodes_[left_root].size < nodes_[right_root].size) {
            std::swap(left_root, right_root);
        }
        nodes_[right_root].parent = left_root;
        nodes_[left_root].size += nodes_[right_root].size;
        --components_;
        return true;
    }

    [[nodiscard]] bool same(std::size_t left, std::size_t right)
    {
        const auto left_root = find(left);
        return left_root == find(right);
    }

    [[nodiscard]] std::size_t component_size(std::size_t index)
    {
        return nodes_[find(index)].size;
    }

private:
    struct Node {
        std::size_t parent = 0;
        std::size_t size = 1;
    };

    std::vector<Node> nodes_;
    std::size_t components_;
};

}  // namespace cp
