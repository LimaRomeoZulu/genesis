#ifndef GENESIS_TAXONOMY_TAXON_H_
#define GENESIS_TAXONOMY_TAXON_H_

/*
    Genesis - A toolkit for working with phylogenetic data.
    Copyright (C) 2014-2016 Lucas Czech

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Contact:
    Lucas Czech <lucas.czech@h-its.org>
    Exelixis Lab, Heidelberg Institute for Theoretical Studies
    Schloss-Wolfsbrunnenweg 35, D-69118 Heidelberg, Germany
*/

/**
 * @brief
 *
 * @file
 * @ingroup taxonomy
 */

#include "taxonomy/taxonomy.hpp"

namespace genesis {
namespace taxonomy {

/**
 * @brief Store a Taxon, i.e., an element in a Taxonomy, with its name, rank and sub-taxa.
 *
 * This class models a taxon within the hierarchy of a Taxonomy. Each such taxon can have a
 * @link name() name@endlink assigned to it, a @link rank() taxonomic rank@endlink, and an
 * arbitrarily nested set of sub-taxa.
 * The name and the rank are both free-form strings which can have any value.
 *
 * Each taxon is itself also a Taxonomy, in terms of class inheritance. This also makes some
 * biological sense, as a taxon can be seen as the taxonomy of its sub-taxa. We however only
 * consider the Taxonomy as the top level of the hierarchy. For more information, see there.
 */
class Taxon : public Taxonomy
{
public:

    // -------------------------------------------------------------------------
    //     Typedefs and Enums
    // -------------------------------------------------------------------------

    /**
     * @brief Taxonomy is a friend, as it needs access to parent_ in order to ensure consistency.
     */
    friend Taxonomy;

    // -------------------------------------------------------------------------
    //     Constructors and Rule of Five
    // -------------------------------------------------------------------------

    /**
     * @brief Default constructor. Does nothing.
     */
    Taxon()
        : parent_(nullptr)
    {}

    /**
     * @brief Constructor that uses the given name for the Taxon.
     */
    Taxon( std::string name )
        : name_(name)
        , parent_(nullptr)
    {}

    /**
     * @brief Default destructor.
     */
    virtual ~Taxon() = default;

    Taxon( Taxon const& ) = default;
    Taxon( Taxon&& )      = default;

    Taxon& operator= ( Taxon const& ) = default;
    Taxon& operator= ( Taxon&& )      = default;

    void swap( Taxon& other )
    {
        using std::swap;
        Taxonomy::swap( other );
        swap( name_,   other.name_ );
        swap( parent_, other.parent_ );
    }

    // -------------------------------------------------------------------------
    //     Properties
    // -------------------------------------------------------------------------

    std::string const& name() const;
    void               name( std::string const& value );

    std::string const& rank() const;
    void               rank( std::string const& value );

    Taxon const* parent () const;
    Taxon*       parent ();

    // -------------------------------------------------------------------------
    //     Protected Implementation Details
    // -------------------------------------------------------------------------

protected:

    Taxon& add_child_( Taxon&& child ) override;

    // -------------------------------------------------------------------------
    //     Data Members
    // -------------------------------------------------------------------------

private:

    std::string name_;
    std::string rank_;

    Taxon*      parent_;
};

} // namespace taxonomy
} // namespace genesis

#endif // include guard
