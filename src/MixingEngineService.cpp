#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
    // Your implementation here

    std::cout << "[MixingEngineService] Initialized with 2 empty decks.\n";

}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    // Your implementation here
    std::cout << "[MixingEngineService] Cleaning up decks...\n";

    for(int i = 0; i < 2; i++){
        if(decks[i] != nullptr){
            delete decks[i];
            decks[i] = nullptr;
        }
    }
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    // Your implementation here
    std::cout << "\n=== Loading Track to Deck ===\n";
    PointerWrapper<AudioTrack> clone = track.clone(); 
    if(!clone){
        std::cout << "[ERROR] Track: " << track.get_title() << " failed to clone \n";
        return -1;
    }

    // finding the dect that is not active
    size_t  target_deck = 1 - active_deck;
    std::cout << "[Deck Switch] Target deck: " << target_deck <<"\n";

    // unloading deck
    if(decks[target_deck]){
        delete decks[target_deck];
        decks[target_deck] = nullptr;
    }

    // simulating
    clone->load();
    clone->analyze_beatgrid();

    //BPM Management
    if(decks[active_deck] && auto_sync){
        if(!can_mix_tracks(clone)){
            sync_bpm(clone);
        }
    }else {
        std::cout << "[Sync BPM] Cannot sync - one of the decks is empty.\n";
    }

    //transfer ownership
    decks[target_deck] = clone.release();
    std::cout << "[Load Complete] '" << track.get_title() << "' is now loaded on deck " << target_deck <<"\n";

   /*
    size_t old_active_deck = active_deck;
    if(decks[old_active_deck] != nullptr){
        std::cout << "[Unload] Unloading previous deck " << old_active_deck << " (" << decks[old_active_deck]->get_title() << ")\n";
        delete decks[old_active_deck];
        decks[old_active_deck] = nullptr;
    }
    */ 
    

    //switching decks
    active_deck = target_deck;

    std::cout << "[Active Deck] Switched to deck " << target_deck << "\n";

    return target_deck;
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    if(decks[active_deck] == nullptr){
        return false;
    }

    if(!track){
        return false;
    }

    int bpm_active = decks[active_deck]->get_bpm();
    int bpm_track = track->get_bpm();
    int bpm_diff;

    if(bpm_active >= bpm_track){
        bpm_diff = bpm_active - bpm_track;
    }else{
        bpm_diff = bpm_track - bpm_active;
    }

    if(bpm_diff <= bpm_tolerance){
        return true;
    }else{
        return false;
    }
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    if(decks[active_deck] == nullptr || !track){
        return;
    }
    int bpm_track = track->get_bpm();
    int bpm_active = decks[active_deck]->get_bpm();

    int avg_bpm = (bpm_track + bpm_active)/2;

    track->set_bpm(avg_bpm);

    std::cout << "[Sync BPM] Syncing BPM from " << bpm_track << " to " << avg_bpm << "\n";
}

