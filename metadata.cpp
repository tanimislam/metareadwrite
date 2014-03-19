/**
 * C libraries
 */
#include <mp4v2/mp4v2.h>
#include "metadata.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * C++ libraries
 */
#include <cstring>
#include <sstream>
#include <iostream>
#include <boost/regex.hpp>
#include <tag.h>
#include <fileref.h>
using namespace std;
using namespace boost;

int get_state(string filename) {
  int state = -1;
  if (regex_search(filename, regex("\\.(m4a|mp4)$"))) state = 1;
  if (regex_search(filename, regex("\\.(mp3|ogg)$"))) state = 2;
  if (regex_search(filename, regex("\\.mpc$"))) state = 2;
  if (regex_search(filename, regex("\\.flac$"))) state = 2;
  return state;
}

Metadata::Metadata(string filename) {
  int state = get_state(filename);
  if (state == 1) {
    MP4FileHandle hFile = MP4Read(filename.c_str());
    if (!MP4_IS_VALID_FILE_HANDLE(hFile)) return;
    TagLib::String st("");
        
    // get length of file
    MP4Duration mp4Duration = MP4GetDuration(hFile);
    duration = (int)( mp4Duration * 1.0/1000 + 0.5);
    
    const MP4Tags *mp4tags = MP4TagsAlloc();    
    if (!MP4TagsFetch( mp4tags, hFile)) {
      MP4TagsFree( mp4tags);
      return;
    }
    if (mp4tags->artist != NULL) { // status = MP4GetMetadataArtist(hFile, &my_string)
      st = mp4tags->artist;
      artist = st.to8Bit(false);
    }    
    if (mp4tags->name != NULL) { // status = MP4GetMetadataName(hFile, &my_string)
      st = mp4tags->artist;
      title = st.to8Bit(false);
    }
    if (mp4tags->comments != NULL) { // status = MP4GetMetadataComment(hFile, &my_string)
      st = mp4tags->comments;
      comment = st.to8Bit(false);
    }    
    if (mp4tags->album != NULL) { // status = MP4GetMetadataAlbum(hFile, &my_string)
      st = mp4tags->album;
      album = st.to8Bit(false);
    }
    if (mp4tags->track != NULL) { //     status = MP4GetMetadataTrack(hFile, &foo1, &foo2)
      track = mp4tags->track->index;
    }
    if (mp4tags->releaseDate != NULL) { // status = MP4GetMetadataYear(hFile, &my_string);
      year = atoi(mp4tags->releaseDate);
    }
    MP4TagsFree( mp4tags);
    MP4Close(hFile);
  }
  else if (state == 2) {
    TagLib::FileRef fr(filename.c_str());
    if (fr.isNull() || !fr.tag()) return;
    artist = (fr.tag()->artist()).to8Bit(false);
    title = (fr.tag()->title()).to8Bit(false);
    album = (fr.tag()->album()).to8Bit(false);
    comment = (fr.tag()->comment()).to8Bit(false);
    track = fr.tag()->track();
    year = fr.tag()->year();
    duration = fr.audioProperties()->length();
  }
}

bool Metadata::clear_tracks() {
  
  // clear out all the info from metainf
  track = -1;
  year = -1;
  artist = "";
  title = "";
  album = "";
  comment = "";
  return true;
}

bool Metadata::fill_out_tracks(string filename) {  
  
  // first, clear out all the info from metainf
  clear_tracks();
  
  // what format is the file?
  int state = get_state(filename);
  if (state == 1) {
    MP4FileHandle hFile = MP4Read(filename.c_str());
    if (!MP4_IS_VALID_FILE_HANDLE(hFile)) return false;
    TagLib::String st("");
        
    // get length of file
    MP4Duration mp4Duration = MP4GetDuration(hFile);
    duration = (int)( mp4Duration * 1.0/1000 + 0.5);
    
    const MP4Tags *mp4tags = MP4TagsAlloc();    
    if (!MP4TagsFetch( mp4tags, hFile)) {
      MP4TagsFree( mp4tags);
      return false;
    }
    if (mp4tags->artist != NULL) { // status = MP4GetMetadataArtist(hFile, &my_string)
      st = mp4tags->artist;
      artist = st.to8Bit(false);
    }    
    if (mp4tags->name != NULL) { // status = MP4GetMetadataName(hFile, &my_string)
      st = mp4tags->artist;
      title = st.to8Bit(false);
    }
    if (mp4tags->comments != NULL) { // status = MP4GetMetadataComment(hFile, &my_string)
      st = mp4tags->comments;
      comment = st.to8Bit(false);
    }    
    if (mp4tags->album != NULL) { // status = MP4GetMetadataAlbum(hFile, &my_string)
      st = mp4tags->album;
      album = st.to8Bit(false);
    }
    if (mp4tags->track != NULL) { //     status = MP4GetMetadataTrack(hFile, &foo1, &foo2)
      track = mp4tags->track->index;
    }
    if (mp4tags->releaseDate != NULL) { // status = MP4GetMetadataYear(hFile, &my_string);
      year = atoi(mp4tags->releaseDate);
    }
    MP4TagsFree( mp4tags);
    MP4Close(hFile);
    return true;
  }
  else if (state == 2) {
    TagLib::FileRef fr(filename.c_str());
    if (fr.isNull() || !fr.tag()) return false;
    artist = (fr.tag()->artist()).to8Bit(false);
    title = (fr.tag()->title()).to8Bit(false);
    album = (fr.tag()->album()).to8Bit(false);
    comment = (fr.tag()->comment()).to8Bit(false);
    track = fr.tag()->track();
    year = fr.tag()->year();
    return true;
  }
  else return false;
}

bool Metadata::fill_in_tracks(string filename) {
  
  // what format is the file?
  int state = get_state(filename);
  if (state == 1) {
    std::stringstream ss; ss << year;
    MP4FileHandle hFile = MP4Modify(filename.c_str(), 0);
    if (!MP4_IS_VALID_FILE_HANDLE(hFile)) return false;
    const MP4Tags *mp4tags = MP4TagsAlloc();
    if (!MP4TagsFetch( mp4tags, hFile)) {
      MP4TagsFree( mp4tags);
      return false;
    }
    if (artist.size() != 0) MP4TagsSetArtist( mp4tags, artist.c_str());
    if (title.size() != 0) MP4TagsSetName( mp4tags, title.c_str()); // MP4SetMetadataName(hFile, title.c_str());
    if (comment.size() != 0) MP4TagsSetComments( mp4tags, comment.c_str()); // MP4SetMetadataComment(hFile, comment.c_str());
    if (album.size() != 0) MP4TagsSetAlbum( mp4tags, album.c_str()); // MP4SetMetadataAlbum(hFile, album.c_str());
    if (track > 0) {
      MP4TagTrack trk;
      trk.index = track;
      trk.total = 0;
      MP4TagsSetTrack( mp4tags, &trk); // MP4SetMetadataTrack(hFile, (u_int16_t)track, 0);
    }
    if (year > 0) MP4TagsSetReleaseDate( mp4tags, ss.str().c_str()); // MP4SetMetadataYear(hFile, ss.str().c_str());
    MP4TagsStore( mp4tags, hFile );
    MP4TagsFree( mp4tags );
    MP4Close(hFile);
    return true;
  }
  else if (state == 2) {
    TagLib::FileRef fr(filename.c_str());
    if (fr.isNull() || !fr.tag()) return false;
    if (album.size() != 0) fr.tag()->setAlbum(album);
    if (title.size() != 0) fr.tag()->setTitle(title);
    if (artist.size() != 0) fr.tag()->setArtist(artist);
    if (comment.size() != 0) fr.tag()->setComment(comment);
    if (track > 0) fr.tag()->setTrack((uint)track);
    if (year > 0) fr.tag()->setYear((uint)year);
    fr.save();
    return true;
  }
  else return false;
}
