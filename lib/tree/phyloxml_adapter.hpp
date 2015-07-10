#ifndef GENESIS_TREE_PHYLOXML_ADAPTER_H_
#define GENESIS_TREE_PHYLOXML_ADAPTER_H_

/**
 * @brief Header of PhyloxmlAdapter class.
 *
 * @file
 * @ingroup tree
 */

#include "tree/phyloxml_processor.hpp"
#include "utils/color.hpp"
#include "utils/utils.hpp"
#include "utils/xml_document.hpp"

namespace genesis {

// =================================================================================================
//     PhyloXML Adapter
// =================================================================================================

/**
 * @brief
 */
template <typename Tree>
class PhyloxmlAdapter
{
public:

    // -------------------------------------------------------------------------
    //     Constructor and Destructor
    // -------------------------------------------------------------------------

    typedef Tree TreeType;

    virtual ~PhyloxmlAdapter() {};

    // template <class PreorderIteratorType>
    // inline void populate_clade (XmlElement* clade, PreorderIteratorType& it)
    // {
    //     // Do nothing by default.
    //     (void) clade;
    //     (void) it;
    // }

    // -------------------------------------------------------------------------
    //     Property Setters
    // -------------------------------------------------------------------------

protected:

    inline void set_name (XmlElement* clade, const std::string& name)
    {
        auto name_e = make_unique<XmlElement>("name");
        name_e->append_markup(name);
        clade->content.push_back(std::move(name_e));
    }

    inline void set_branch_length (XmlElement* clade, const double length)
    {
        auto bl_e = make_unique<XmlElement>("branch_length");
        bl_e->append_markup(std::to_string(length));
        clade->content.push_back(std::move(bl_e));
    }

    inline void set_color (XmlElement* clade, unsigned char r, unsigned char g, unsigned char b)
    {
        auto re = make_unique<XmlElement>("red");
        re->append_markup(std::to_string(r));

        auto ge = make_unique<XmlElement>("green");
        ge->append_markup(std::to_string(g));

        auto be = make_unique<XmlElement>("blue");
        be->append_markup(std::to_string(b));

        auto color = make_unique<XmlElement>("color");
        color->content.push_back(std::move(re));
        color->content.push_back(std::move(ge));
        color->content.push_back(std::move(be));

        clade->content.push_back(std::move(color));
    }

    inline void set_color (XmlElement* clade, Color color)
    {
        set_color (clade, color.r, color.g, color.b);
    }

};

} // namespace genesis

#endif // include guard