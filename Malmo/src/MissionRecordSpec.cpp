// --------------------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft Corporation.  All rights reserved.
// --------------------------------------------------------------------------------------------------------------------

// Local:
#include "MissionRecordSpec.h"

// Boost:
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

// STL:
#include <exception>
#include <iostream>

namespace malmo
{
    MissionRecordSpec::MissionRecordSpec()
        : is_recording(false)
        , is_recording_mp4(false)
        , is_recording_observations(false)
        , is_recording_rewards(false)
        , is_recording_commands(false)
        , mp4_bit_rate(0)
        , mp4_fps(0)
    {
    }

    MissionRecordSpec::MissionRecordSpec(std::string destination)
        : is_recording(true)
        , is_recording_mp4(false)
        , is_recording_observations(false)
        , is_recording_rewards(false)
        , is_recording_commands(false)
        , mp4_bit_rate(0)
        , mp4_fps(0)
        , destination(destination)
    {
        boost::uuids::random_generator gen;
        boost::uuids::uuid temp_uuid = gen();
        this->temp_dir = boost::filesystem::path(".");
        this->temp_dir = this->temp_dir / "mission_records" / boost::uuids::to_string(temp_uuid);
        this->mp4_path = (this->temp_dir / "video.mp4").string();
        this->observations_path = (this->temp_dir / "observations.txt").string();
        this->rewards_path = (this->temp_dir / "rewards.txt").string();
        this->commands_path = (this->temp_dir / "commands.txt").string();
        this->mission_init_path = (this->temp_dir / "missionInit.xml").string();
 
        std::ofstream file(destination, std::ofstream::binary);
        if (file.fail()) {
           std::cout << "ERROR: Cannot write to " << destination << " - check the path exists and you have permission to write there." << std::endl;
           throw std::runtime_error("Can not write to recording destination.");
       }
    }

    void MissionRecordSpec::recordMP4(int frames_per_second, int64_t bit_rate)
    {
        if (!this->is_recording){
            throw std::runtime_error("Mission is not being recorded.");
        }

        this->is_recording_mp4 = true;
        this->mp4_fps = frames_per_second;
        this->mp4_bit_rate = bit_rate;
    }

    void MissionRecordSpec::recordObservations()
    {
        if (!this->is_recording){
            throw std::runtime_error("Mission is not being recorded.");
        }

        this->is_recording_observations = true;
    }

    void MissionRecordSpec::recordRewards()
    {
        if (!this->is_recording){
            throw std::runtime_error("Mission is not being recorded.");
        }

        this->is_recording_rewards = true;
    }

    void MissionRecordSpec::recordCommands()
    {
        if (!this->is_recording){
            throw std::runtime_error("Mission is not being recorded.");
        }

        this->is_recording_commands = true;
    }

    std::string MissionRecordSpec::getTemporaryDirectory()
    {
        if (!this->is_recording){
            throw std::runtime_error("Mission is not being recorded.");
        }

        if (boost::filesystem::exists(this->temp_dir)){
            return this->temp_dir.string();
        }else{
            throw std::runtime_error("Mission record does not yet exist. Temporary directory will be created once a mission has begun.");
        }
    }
}