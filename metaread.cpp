/***************************************************************************
 *   Copyright (C) 2007 by Tanim Islam   *
 *   tanim.islam@lra.ens.fr   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

// C++ headers
#include <sstream>
#include <iostream>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/regex.hpp>

// local headers
#include "metadata.h"
namespace bfs = boost::filesystem;
using namespace std;

bool is_valid_file(bfs::path pth, string filename) {
  pth = bfs::system_complete(bfs::path(filename));
  if (!bfs::exists(pth)) return false;
  return true;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "error. Proper syntax: " << argv[0] << " filename" << std::endl;
    return 1;
  }
  
  // code to make sure we can read out filenames with spaces in them
  if (!bfs::exists(bfs::system_complete(bfs::path(argv[1])))) {
    std::cout << "error. " << argv[1] << " does not exist. Exiting..." << std::endl;
    return -1;
  }
  Metadata metad = Metadata(string(argv[1]));
  stringstream ss("");
  ss << "artist:\t" << metad.get_artist() << std::endl;
  ss << "title:\t" << metad.get_title() << std::endl;
  ss << "album:\t" << metad.get_album() << std::endl;
  ss << "track:\t" << metad.get_track() << std::endl;
  ss << "year:\t" << metad.get_year() << std::endl;
	ss << "duration:\t" << metad.get_duration() << std::endl;
  ss << "comment:\t" << metad.get_comment() << std::endl;
  cout << ss.str();
  return 1;
}
