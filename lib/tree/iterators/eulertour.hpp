#ifndef GENESIS_TREE_ITERATORS_EULERTOUR_H_
#define GENESIS_TREE_ITERATORS_EULERTOUR_H_

/**
 * @brief
 *
 * @file
 * @ingroup tree
 */

#include <iterator>

namespace genesis {

// =================================================================================================
//     Euler Tour Iterator
// =================================================================================================

template <typename LinkType, typename NodeType, typename EdgeType>
class TreeIteratorEulertour
{
public:
    // -----------------------------------------------------
    //     Typedefs
    // -----------------------------------------------------

    typedef TreeIteratorEulertour<LinkType, NodeType, EdgeType> self_type;
    typedef std::forward_iterator_tag iterator_category;

    // typedef NodeType  value_type;
    // typedef size_t    difference_type;
    // typedef NodeType* pointer;
    // typedef NodeType& reference;
    //
    // inline reference operator * ()
    // {
    //     return *(link_->node());
    // }
    //
    // inline pointer operator -> ()
    // {
    //     return link_->node();
    // }

    // -----------------------------------------------------
    //     Constructor
    // -----------------------------------------------------

    TreeIteratorEulertour (LinkType* link) : link_(link), start_(link)
    {}

    // TreeIteratorEulertour () :
    //     link_(nullptr),
    //     start_(nullptr)
    // {}
    //
    // TreeIteratorEulertour (const LinkType* link) :
    //     link_(link),
    //     start_(link)
    // {}
    //
    // TreeIteratorEulertour (const NodeType* node) :
    //     link_(node->primary_link()->next()),
    //     start_(node->primary_link()->next())
    // {}

    // -----------------------------------------------------
    //     Iteration
    // -----------------------------------------------------

    // inline self_type begin()
    // {
    //     return self_type(start_);
    // }
    //
    // inline self_type end()
    // {
    //     return self_type(nullptr);
    // }

    // -----------------------------------------------------
    //     Operators
    // -----------------------------------------------------

    inline self_type& operator ++ ()
    {
        link_ = link_->outer()->next();
        if (link_ == start_) {
            link_ = nullptr;
        }
        return *this;
    }

    inline self_type operator ++ (int)
    {
        self_type tmp = *this;
        ++(*this);
        return tmp;
    }

    inline bool operator == (const self_type &other) const
    {
        return other.link_ == link_;
    }

    inline bool operator != (const self_type &other) const
    {
        return !(other == *this);
    }

    // -----------------------------------------------------
    //     Members
    // -----------------------------------------------------

    inline LinkType* link() const
    {
        return link_;
    }

    inline NodeType* node() const
    {
        return link_->node();
    }

    inline EdgeType* edge() const
    {
        return link_->edge();
    }

    inline LinkType* start_link() const
    {
        return start_;
    }

    inline NodeType* start_node() const
    {
        return start_->node();
    }

protected:
    LinkType* link_;
    LinkType* start_;
};

// =================================================================================================
//     Euler Tour Iterator Range
// =================================================================================================

// template<class TreeType>
// TreeIteratorEulertour<
//     typename TreeType::LinkType,
//     typename TreeType::NodeType,
//     typename TreeType::EdgeType
// > eulertour(const TreeType& tree)
// {
//     return TreeIteratorEulertour(tree.root_link());
// }

} // namespace genesis

#endif // include guard
