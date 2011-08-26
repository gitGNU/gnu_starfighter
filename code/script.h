/*
Copyright (C) 2003 Parallel Realities

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include <stdlib.h>
#include <string.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>

#include "defs.h"
#include "structs.h"
#include "classes.h"

extern void doStarfield();
extern void getPlayerInput();
extern void loadGameGraphics();
extern void loadBackground(const char *filename);
extern void setRadioMessage(signed char face, const char *in, int priority);
extern void doExplosions();
extern void addEngine(object *craft);
extern void doExplosions();
extern void resetLists();
extern int getFace(const char *face);
extern int locateDataInPak(const char *file, bool required);
extern void flushInput();

extern Game currentGame;
extern object enemy[MAX_ALIENS];
extern object player;
extern object defEnemy[MAX_DEFALIENS];
extern globalEngineVariables engine;
extern Graphics graphics;
extern event gameEvent[10];
extern cutMsg cutMessage[10];
