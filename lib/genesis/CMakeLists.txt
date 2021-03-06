# Genesis - A toolkit for working with phylogenetic data.
# Copyright (C) 2014-2017 Lucas Czech
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# Contact:
# Lucas Czech <lucas.czech@h-its.org>
# Exelixis Lab, Heidelberg Institute for Theoretical Studies
# Schloss-Wolfsbrunnenweg 35, D-69118 Heidelberg, Germany

# ------------------------------------------------------------------------------
#   Build Options
# ------------------------------------------------------------------------------

set (LIBRARY_OUTPUT_PATH    ${PROJECT_SOURCE_DIR}/bin)

include_directories (${PROJECT_SOURCE_DIR}/lib)

# ------------------------------------------------------------------------------
#   Sources
# ------------------------------------------------------------------------------

IF( GENESIS_UNITY_BUILD )

    # See main CMake file for the monolith macro.
    ASSEMBLE_MONOLITH( ${PROJECT_SOURCE_DIR}/lib/genesis "${GENESIS_UNITY_BUILD}" "lib" genesis_lib_sources )

ELSE()

    # Find all source files. The CMake documentation does not recommend this,
    # but for our purposes, this is good enough for now. Futhremore, we use a make file
    # that offers an update target to get around this limitation.
    file (GLOB_RECURSE genesis_lib_sources   ${PROJECT_SOURCE_DIR}/lib/genesis/*.cpp)
    # file (GLOB_RECURSE genesis_lib_headers   ${PROJECT_SOURCE_DIR}/lib/genesis/*.hpp)
    # file (GLOB_RECURSE genesis_lib_templates ${PROJECT_SOURCE_DIR}/lib/genesis/*.tpp)

ENDIF()

# ------------------------------------------------------------------------------
#   Build Libraries
# ------------------------------------------------------------------------------

if (GENESIS_BUILD_SHARED_LIB)
    message (STATUS "${ColorBlue}Building shared lib${ColorEnd}")
    add_library           (genesis_lib_shared SHARED ${genesis_lib_sources})
    set_target_properties (genesis_lib_shared PROPERTIES OUTPUT_NAME genesis)

    # Link against any external libraries, e.g. Pthreads.
    target_link_libraries (genesis_lib_shared ${GENESIS_INTERNAL_LINK_LIBRARIES})

    # Add this shared library to our exported libraries, which can then be used by the parent scope.
    set( GENESIS_SHARED_LIBRARY genesis_lib_shared PARENT_SCOPE )
    set( GENESIS_LIBRARY genesis_lib_shared PARENT_SCOPE )

endif()

if (GENESIS_BUILD_STATIC_LIB)
    message (STATUS "${ColorBlue}Building static lib${ColorEnd}")
    add_library           (genesis_lib_static STATIC ${genesis_lib_sources})
    set_target_properties (genesis_lib_static PROPERTIES OUTPUT_NAME genesis)

    # Link against any external libraries, e.g. Pthreads.
    target_link_libraries (genesis_lib_static ${GENESIS_INTERNAL_LINK_LIBRARIES})

    # Add this static library to our exported libraries, which can then be used by the parent scope.
    # In case that we also build the shared lib, GENESIS_LIBRARY is overwritten, which is on
    # purpose. This way, if both libraries are available, we prefer to use the static one.
    # This is more useful for external projects that use Genesis as a dependency: They probably
    # do not want to run into dynamic linker issues when the shared library is not found after
    # moving around their binary. So better link statically.
    # Also, as the static library has to be actively activated (by setting GENESIS_BUILD_STATIC_LIB),
    # there already was a user action once we are here, so the user actually wants the static one.
    set( GENESIS_STATIC_LIBRARY genesis_lib_static PARENT_SCOPE )
    set( GENESIS_LIBRARY genesis_lib_static PARENT_SCOPE )
endif()
