//-------------------------------------------------------------------------
/*
Copyright (C) 2018 Jakob L. Kreuze

This file is part of EDuke32.

EDuke32 is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License version 2
as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
//-------------------------------------------------------------------------

/*
 * Replacement for the SDL2_Mixer reimplementation of Jim Dose's FX_MAN
 * routines, for anyone troubled by the shitty internal version of TiMidity.
 */

#include "fluidsynth_inc.h"

#include "duke3d.h"
#include "game.h"
#include "music.h"
#include "midi.h"

static bool music_initialized = false;

static fluid_synth_t *synth;
static fluid_settings_t *settings;
static fluid_player_t *player;
static fluid_audio_driver_t *driver;

static const char *backend_names[10] = {
    "unknown",
    "jack",
    "alsa",
    "oss",
    "pulseaudio",
    "coreaudio",
    "dsound",
    "portaudio",
    "sndman",
    "dart",
};


const char *MUSIC_ErrorString(int32_t ErrorNumber)
{
    switch (ErrorNumber)
    {
    default:
        return "Unknown error.";
    }

    return NULL;
}

int32_t MUSIC_Init(int32_t SoundCard, int32_t Address)
{
    int32_t backend = ud.config.FluidsynthBackend;

    if (getenv("EDUKE32_MUSIC_CMD")) {
        initprintf("External MIDI player not supported by Fluidsynth backend");
    }

    if (music_initialized) {
        // Music system is already initialized
        // return MUSIC_Error;
        return MUSIC_Ok;
    }

    settings = new_fluid_settings();

    MUSIC_SetVolume(ud.config.MusicVolume);

    if (backend) {
        fluid_settings_setstr(settings, "audio.driver", backend_names[backend]);
    }

    synth = new_fluid_synth(settings);
    player = new_fluid_player(synth);
    driver = new_fluid_audio_driver(settings, synth);

    MUSIC_SetLoopFlag(MUSIC_LoopSong); // Loop by default
    fluid_synth_sfload(synth, "/usr/share/soundfonts/FluidR3_GM.sf2", 1);

    return MIDI_Ok;
}

int32_t MUSIC_Shutdown(void)
{
    delete_fluid_player(player);
    delete_fluid_audio_driver(driver);
    delete_fluid_synth(synth);
    delete_fluid_settings(settings);

    music_initialized = false;
    return MUSIC_Ok;
}

void MUSIC_SetVolume(int32_t volume)
{
    double val = ((double) volume) * 0.00390625;
    fluid_settings_setnum(settings, "synth.gain", val);
}

int32_t MUSIC_GetVolume(void)
{
    double val;
    fluid_settings_getnum(settings, "synth.gain", &val);
    return (int32_t) (val / 0.00390625);
}

void MUSIC_SetLoopFlag(int32_t loopflag)
{
    fluid_player_set_loop(player, loopflag == MUSIC_LoopSong ? -1 : 1);
}

void MUSIC_Continue(void)
{
    fluid_player_play(player);
}

void MUSIC_Pause(void)
{
    fluid_player_stop(player);
}

int32_t MUSIC_StopSong(void)
{
    fluid_player_stop(player);
    MUSIC_Shutdown();
    MUSIC_Init(0, 0);
    return MUSIC_Ok;
}

// Fun fact to any onlookers: The part that parses the MIDI file and
// figures out how long it is worked on the first try and it's a format
// I've never worked with before. I celebrated.
int32_t MUSIC_PlaySong(char *song, int32_t loopflag)
{
    char *tracks;
    size_t file_size;
    uint16_t num_tracks;

    tracks = song + 0x14;
    num_tracks = *((uint16_t *) (song + 0x10));
    file_size = 0x14; // Size of the header.

    while (num_tracks--) {
        uint16_t track_size;

        if (!memcmp(tracks, "MTrk", 4)) {
            break;
        }

        track_size = *((uint16_t *) (tracks + 0x04));
        file_size += track_size + 0x08;
        tracks += track_size + 0x08;
    }

    fluid_player_add_mem(player, song, file_size);
    fluid_player_play(player);
    return MUSIC_Ok;
}

int32_t MUSIC_InitMidi(int32_t card, midifuncs *Funcs, int32_t Address)
{
    return MIDI_Ok;
}

void MUSIC_Update(void) {}
