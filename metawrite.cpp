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

void usage() {
  std::cout << "error. Proper syntax: metawrite <field> <filename>" << std::endl;
  std::cout << "The valid fields are: " << std::endl;
  std::cout << " -t <title>" << std::endl;
  std::cout << " -a <artist>" << std::endl;
  std::cout << " -A <album>" << std::endl;
  std::cout << " -y <year>" << std::endl;
  std::cout << " -T <track>" << std::endl;
  exit(1);
}

bool file_exists(std::string filename) {
  boost::filesystem::path pth(filename);
  if (boost::filesystem::exists(pth)) return true;
  else return false;
}

bool is_argument(std::string argument) {
  return (argument.size() == 2 && argument.substr(0,1) == std::string("-"));
}

int main(int argc, char **argv) {
  if (argc < 2) usage();
	
  // get list of filenames and arguments
  std::vector<std::string> fileList;
  while (argc > 0 && file_exists(std::string(argv[argc-1]))) {
    fileList.push_back(std::string(argv[argc-1]));
    argc--;
  }
  if (fileList.empty()) usage();
	  
  Metadata metad;
  for (int i=1; i<argc-1; i+=2) {
    if (is_argument(std::string(argv[i])) && !is_argument(std::string(argv[i+1]))) {
      char field = argv[i][1];
      std::string argument = argv[i+1];
      switch(field) {
      case 't':
	metad.set_title(argument);
	break;
      case 'a':
	metad.set_artist(argument);
	break;
      case 'A':
	metad.set_album(argument);
	break;
      case 'c':
	metad.set_comment(argument);
	break;
      case 'y':
	metad.set_year(atoi(argument.c_str()));
	break;
      case 'T':
	metad.set_track(atoi(argument.c_str()));
	break;
      default:
	usage();
	break;
      }
    }
  }
  for (int i=0; i< (int)fileList.size(); i++) metad.fill_in_tracks(fileList[i]);
  return 1;
}
