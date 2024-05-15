#include "localdefs.h"
#include <fstream>
#include "html.hpp"
#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>

#include <iostream>
using namespace std;

void usage(int argc, char *argv[]) {
  cerr << "Usage: " << argv[0] << " <filename>" << endl;
}

int main(int argc, char *argv[]) {
  const char *pFilename = 0;

  if (argc != 2) {
    usage(argc, argv);
    return 255;
  }

  pFilename = argv[1];
  ifstream ifs(pFilename);

  string line;
  while (getline(ifs, line)) {
    vector<string> constituents;
    boost::split(constituents, line, boost::is_any_of(","));
    vector<string>::const_iterator cx = constituents.begin();
    while (cx != constituents.end()) {
      string s = *cx;
      boost::trim(s);
      cout << "**" << s << "**<-->" << flush;
      cx++;
    }
    cout << endl;
  }

  return 0;
}
