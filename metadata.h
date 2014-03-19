#ifndef I_metadata_h
#define I_metadata_h
#include <string>
using namespace std;

class Metadata {
 private:
  string artist;
  string title;
  string album;
  string comment;
  int track;
  int year;
  int duration;
 public:
  Metadata() {
    track = -1;
    year = -1;
    duration = -1;
    artist = "";
    title = "";
    album = "";
    comment = "";
  }
  Metadata(string filename);
  ~Metadata() { }
  bool fill_out_tracks(string filename);
  bool fill_in_tracks(string filename);
  bool clear_tracks();
  
  // accessor functions
  string get_artist() { return artist;}
  string get_title() { return title;}
  string get_comment() { return comment;}
  string get_album() { return album;}
  int get_year() { return year;}
  int get_track() { return track;}
  int get_duration() { return duration;}
  
  // modifier functions
  void set_artist(string input) { artist = input;}
  void set_title(string input) { title = input;}
  void set_comment(string input) { comment = input;}
  void set_album(string input) { album = input;}
  void set_year(int yr) { year = yr;}
  void set_track(int trck) { track = trck;}
};

int get_state(string filename);

#endif
