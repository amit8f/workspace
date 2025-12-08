#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist), library() {}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
    int count = 0;
    for(const auto& track_name : library_tracks){
        AudioTrack* new_track = nullptr;
        if(track_name.type == "MP3"){
            
            int bitrate = track_name.extra_param1;

            new_track = new MP3Track(track_name.title,track_name.artists,track_name.duration_seconds,track_name.bpm,bitrate,(bool)track_name.extra_param2);

        }else if(track_name.type == "WAV"){
            int sample_rate = track_name.extra_param1;
            int bit_depth = track_name.extra_param2;

            new_track = new WAVTrack(track_name.title,track_name.artists,track_name.duration_seconds,track_name.bpm,sample_rate,bit_depth);
        }

        if(new_track){
            this->library.push_back(new_track);
            count++;
        }
    }
    std::cout << "[INFO] Track library built: " << count << " tracks loaded\n";
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    // Your implementation here
    AudioTrack* pointer = playlist.find_track(track_title);
    return pointer;
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    // Your implementation here
    int count = 0;
    std::cout << "[INFO] Loading playlist: " << playlist_name << "\n";
    Playlist p(playlist_name);
    this->playlist = p;

    for(const auto& index : track_indices){
        if(index >= 1 || index <= ((int)library.size())){

            PointerWrapper<AudioTrack> cloned_track = library[index-1]->clone();
            //AudioTrack* raw_pointer_track = cloned_track.release(); //unwrap

            if(!cloned_track){
                std::cout << "[ERROR] clone didnt work \n"; 
            }else{
                cloned_track->load();
                cloned_track->analyze_beatgrid();

                playlist.add_track(cloned_track.release());
                count++;

                //std::cout << "Added " << raw_pointer_track->get_title() << " to playlist " << playlist_name << "\n";
            }
        }else{
            std::cout << "[WARNING] Invalid track index: " << index << "\n";
        }
    }
    std::cout << "[INFO] Playlist loaded: " << playlist_name << " (" << count << " tracks)\n";
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    // Your implementation here
    std::vector<std::string> titles;

    std::vector<AudioTrack*> copy_tracks = playlist.getTracks();

    for(AudioTrack* tracks : copy_tracks){
            titles.push_back(tracks->get_title());
    }
    return titles;
}
