#pragma once
#include <variant>
#include <vector>
#include <iterator>
#include <concepts>

class NoItemTag {};

template <std::semiregular T, std::semiregular T2 = T>
class ItemTree;

template <std::semiregular T, std::semiregular T2>
using ItemComponent = std::variant<T, ItemTree<T, T2>>;

template <std::semiregular T, std::semiregular T2>
class ItemTree
    : public std::vector<ItemComponent<T, T2>>
{
public:
    T2 item{};

    ItemTree() = default;

    ItemTree(T2 const& item)
        : item{ item }
    {

    }

    ItemTree(T2&& item) noexcept
        : item{ std::move(item) }
    {

    }

    template <typename... Us>
        requires std::is_constructible_v<T2, Us...>
    ItemTree(Us&&... us)
        : item(std::forward<Us>(us)...)
    {

    }

    ~ItemTree() = default;

    ItemTree(ItemTree const&) = default;
    ItemTree(ItemTree&&) noexcept = default;
    ItemTree& operator=(ItemTree const&) = default;
    ItemTree& operator=(ItemTree&&) = default;

    template <typename U>
        requires std::is_same_v<std::remove_cvref_t<U>, ItemTree<T, T2>>
    ItemTree& Add(U&& u)&
    {
        std::vector<ItemComponent<T, T2>>::push_back(std::forward<U>(u));

        return *this;
    }

    template <typename... Ts>
    ItemTree& Add(Ts&&... ts)&
    {
        std::vector<ItemComponent<T, T2>>::push_back(T(std::forward<Ts>(ts)...));

        return *this;
    }

    template <typename U>
        requires std::is_same_v<std::remove_cvref_t<U>, ItemTree<T, T2>>
    ItemTree&& Add(U&& u)&&
    {
        std::vector<ItemComponent<T, T2>>::push_back(std::forward<U>(u));

        return std::move(*this);
    }

    template <typename... Ts>
    ItemTree&& Add(Ts&&... ts)&&
    {
        std::vector<ItemComponent<T, T2>>::push_back(T(std::forward<Ts>(ts)...));

        return std::move(*this);
    }

    ItemTree& Add(std::initializer_list<T> l, T item = T{})
    {
        ItemTree<T> menu(item);

        std::move(std::begin(l), std::end(l), std::back_inserter(menu));

        return Add(menu);
    }
};