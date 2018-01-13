#!/usr/bin/env python

import os
import shutil
import sys

PATCHDIR = "./patch/"

ADDITIONS = [
    ("fluidsynth_inc.h", "source/build/include/fluidsynth_inc.h"),
]

REPLACEMENTS = [
    ("GNUmakefile", "GNUmakefile"),
    ("config.cpp", "source/duke3d/src/config.cpp"),
    ("game.h", "source/duke3d/src/game.h"),
    ("menus.cpp", "source/duke3d/src/menus.cpp"),
    ("osdcmds.cpp", "source/duke3d/src/osdcmds.cpp"),
    ("sdlmusic.cpp", "source/duke3d/src/sdlmusic.cpp"),
]

out = sys.stdout.write
err = sys.stderr.write


if __name__ == "__main__":
    if len(sys.argv) != 2:
        err("usage: {} PATH_TO_SOURCE\n".format(sys.argv[0]))
        sys.exit(1)

    source = sys.argv[1]

    if not os.path.exists(source) or not os.path.isdir(source):
        err("'{}' does not exist\n".format(source))
        sys.exit(1)

    if not os.path.exists(PATCHDIR) or not os.path.isdir(PATCHDIR):
        err("patch directory does not exist\n".format(source))
        sys.exit(1)

    for patch, dest in ADDITIONS:
        out("Adding {} to {}...\n".format(patch, dest))
        shutil.copyfile(os.path.join(PATCHDIR, patch), os.path.join(source, dest))

    for patch, dest in REPLACEMENTS:
        out("Patching {}...\n".format(dest))
        shutil.copyfile(os.path.join(PATCHDIR, patch), os.path.join(source, dest))

    err("\n")
    err("IF EDUKE32 CRASHES ON STARTUP:\n")
    err("1) Open ~/.eduke32/settings.cfg\n")
    err('2) Change snd_fluidsynthbackend to "4"\n')
