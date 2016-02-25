/**
 * @brief
 *
 * @file
 * @ingroup sequence
 */

#include "sequence/io/phylip_writer.hpp"

#include "sequence/sequence_set.hpp"
#include "sequence/sequence.hpp"
#include "utils/core/fs.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace genesis {
namespace sequence {

// =================================================================================================
//     Writing
// =================================================================================================

// void PhylipWriter::to_stream_interleaved( SequenceSet const& sset, std::ostream& os ) const
// {
//
// }

// void PhylipWriter::to_stream_sequential( SequenceSet const& sset, std::ostream& os ) const
// {
//
// }

/**
 * @brief Write Sequences of a SequenceSet to a stream in Phylip format.
 */
void PhylipWriter::to_stream( SequenceSet const& sset, std::ostream& os ) const
{
    // Check if this is an alignment and get its sequence length.
    size_t length = 0;
    for( Sequence const& s : sset ) {
        if (length == 0) {
            length = s.length();
            continue;
        }
        if (s.length() != length) {
            throw std::runtime_error( "Sequences do not have the same length." );
        }
    }
    if (length == 0) {
        throw std::runtime_error( "Sequences are empty." );
    }

    // Write sequences to stream.
    os << sset.size() << " " << length << "\n";
    for (Sequence const& s : sset) {
        // Write label.
        if( label_length_ == 0 ) {
            os << s.label() << " ";
        } else {
            os << s.label().substr( 0, label_length_ );
            if( label_length_ > s.label().length() ) {
                os << std::string( label_length_ - s.label().length(), ' ' );
            }
        }

        // Write sequence. If needed, add new line at every line_length_ position.
        if( line_length_ > 0 ) {
            for( size_t i = 0; i < s.length(); i += line_length_ ) {
                // Write line_length_ many characters.
                // (If the string is shorter, as many characters as possible are used.)
                os << s.sites().substr( i, line_length_ ) << "\n";
            }
        } else {
            os << s.sites() << "\n";
        }
    }
}

/**
 * @brief Write Sequences of a SequenceSet to a file in Phylip format.
 */
void PhylipWriter::to_file( SequenceSet const& sset, std::string const& fn ) const
{
    if( utils::file_exists( fn ) ) {
        throw std::runtime_error( "File '" + fn + "' already exists." );
    }

    std::ofstream ofs( fn );
    if( ofs.fail() ) {
        throw std::runtime_error( "Cannot write to file '" + fn + "'." );
    }

    to_stream( sset, ofs );
}

/**
 * @brief Return Sequences of a SequenceSet in form of a Phylip formatted string.
 *
 * Caveat: This might be a long string! If you simply want to examine a Sequence or SequenceSet,
 * have a look at the print() and print_color() functions.
 */
std::string PhylipWriter::to_string ( SequenceSet const& sset ) const
{
    std::ostringstream oss;
    to_stream( sset, oss );
    return oss.str();
}

// =================================================================================================
//     Properties
// =================================================================================================

/**
 * @brief Set the length of the label in front of the sequences.
 *
 * Phylip has the weird property that labels are written in front of sequences and do not need
 * to have a delimiter, but instead are simply the first `n` characters of the string. This value
 * determines after how many chars the label ends and the actual sequence begins.
 *
 * If set to 0 (default), a relaxed version of Phylip is used, where the sequence begin is
 * automatically detected. Labels can then be of arbitrary lengths, as long as they do not contain
 * white spaces. After the label, a space is appended.
 *
 * If set to a value greater than 0, the label will be cut off after that many chars. For shorter
 * labels, the remaining number is filled with spaces.
 * The functions returns the PhylipWriter object to allow fluent interfaces.
 */
PhylipWriter& PhylipWriter::label_length( size_t value )
{
    label_length_ = value;
    return *this;
}

/**
 * @brief Return the currently set label length.
 *
 * See the setter label_length( size_t ) for details.
 */
size_t PhylipWriter::label_length() const
{
    return label_length_;
}

/**
 * @brief Set the line length, which determines after how many chars (Sequence sites) lines breaks
 * are inserted when writing the Phylip file.
 *
 * Default is `80`. If set to `0`, no breaks are inserted.
 * The functions returns the PhylipWriter object to allow fluent interfaces.
 */
PhylipWriter& PhylipWriter::line_length( size_t value )
{
    line_length_ = value;
    return *this;
}

/**
 * @brief Get the current line length.
 *
 * See the setter line_length( size_t ) for details.
 */
size_t PhylipWriter::line_length() const
{
    return line_length_;
}

} // namespace sequence
} // namespace genesis