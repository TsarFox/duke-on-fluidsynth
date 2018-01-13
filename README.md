# Duke on Fluidsynth

This is a patchset I wrote for eduke32 that replaces the SDL2_Mixer backend,
since its MIDI playback didn't work on my machine.

I should probably implement this as some sort of SVN patch, since right now the
patching script merely copies my source files into the tree.

Also, this was only tested on the latest SVN checkout (r6576). It _might_ work
on the stable releases, but I don't know for sure.

# Demo Video

[![Duke on Fluidsynth Demo](http://img.youtube.com/vi/mxkctwRZlHo/0.jpg)](http://www.youtube.com/watch?v=mxkctwRZlHo "Duke on Fluidsynth Demo")

# Installation

```
$ python patch.py /path/to/eduke32/source/tree
```
