#pragma once

#include <deque>

template <typename TItem, typename TContainer = std::deque<TItem>>
class Stack
{
private:
    TContainer container{};

public:
    using container_type = TContainer;
    using value_type = typename TContainer::value_type;
    using reference = typename TContainer::reference;

    std::size_t size() const
    {
        return container.size();
    }
    bool empty() const
    {
        return container.empty();
    }

    template <typename T>
    void push(T&& _value)
    {
        container.push_back(std::forward<T>(_value));
    }

    void pop(value_type& _value);

    reference top()
    {
        return container.back();
    }
};

template <typename TItem, typename TContainer>
void Stack<TItem, TContainer>::pop(value_type& _value)
{
    _value = container.back();
    container.pop_back();
}

namespace TemplateTemplateParam
{
    template <
        typename TItem, 
        template <typename, typename> class TContainer = std::deque, 
        typename TAllocator = std::allocator<TItem>
    >
    class Stack
    {
    private:
        TContainer<TItem, TAllocator> container{};

    public:
        using container_type = TContainer<TItem, TAllocator>;
        using value_type = typename TContainer<TItem, TAllocator>::value_type;
        using reference = typename TContainer<TItem, TAllocator>::reference;

        std::size_t size() const
        {
            return container.size();
        }
        bool empty() const
        {
            return container.empty();
        }

        template <typename T>
        void push(T&& _value)
        {
            container.push_back(std::forward<T>(_value));
        }

        void pop(value_type& _value);

        reference top()
        {
            return container.back();
        }
    };

    template <typename TItem, template <typename, typename> class TContainer, typename TAllocator>
    void Stack<TItem, TContainer, TAllocator>::pop(value_type& _value)
    {
        _value = container.back();
        container.pop_back();
    }
} // namespace TemplateTemplateParam