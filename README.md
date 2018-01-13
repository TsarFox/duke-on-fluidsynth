# Duke on FluidSynth

This is a patchset I wrote for eduke32 that replaces the SDL2_Mixer backend,
since its MIDI playback didn't work on my machine.

I should probably implement this as some sort of SVN patch, since right now the
patching script merely copies my source files into the tree.

Also, this was only tested on the latest SVN checkout (r6576). It _might_ work
on the stable releases, but I don't know for sure.

This version doesn't have a setting for the soundfont yet; the path is hardcoded
to `/usr/share/soundfonts/FluidR3_GM.sf2`, and if you want to change that,
you'll have to edit line 93 of `patch/sdlmusic.cpp`.

# Demo Video

[![Duke on FluidSynth](http://img.youtube.com/vi/mxkctwRZlHo/0.jpg)](http://www.youtube.com/watch?v=mxkctwRZlHo "Duke on FluidSynth")

# Installation

```
$ python patch.py /path/to/eduke32/source/tree
```
