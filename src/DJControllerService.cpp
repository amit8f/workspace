#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */

 // manages cache (hit or miss or evict) using polymorphic cloning.
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    // Your implementation here 
    if (cache.contains(track.get_title())){
        cache.get(track.get_title());
        return 1;
    }

    PointerWrapper<AudioTrack> clone = track.clone();
    AudioTrack* raw_clone = clone.release(); //unwraping

    if(!raw_clone){
        std::cout << "ERROR failed to clone";
        return -2;
    }

    raw_clone->load();
    raw_clone->analyze_beatgrid();

    PointerWrapper<AudioTrack> clone_wrapped(raw_clone);//rewrap
    
    bool evicted = cache.put(std::move(clone_wrapped));
    if(evicted){
        return -1;
    }else{
        return 0;
    }
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */

 // get the track from the cache.
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    // Your implementation here
    return cache.get(track_title); //returns pointer or nullptr.
}
