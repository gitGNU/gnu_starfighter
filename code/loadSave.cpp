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

#include "loadSave.h"

/*
Reads in each save game that it finds and gives it an appropriate
description using the area variable contained in the game binary
data. It returns the slot number (1 - 10) of the most recently
used file. On the title screen, this is used to determine whether
a player can "Continue Current Game" and "Load Saved Game".
*/
int initSaveSlots()
{
	char fileName[PATH_MAX];
	int imagePos = 350;
	Game tempGame;
	struct stat fileInfo;
	int modTime = 0;
	int continueSaveIndex = 0;

	FILE *fp;

	//READ SAVE GAME DATA
	for (int i = 0 ; i < 5 ; i++)
	{
		sprintf(fileName, "%ssave%.2d.dat", engine.userHomeDirectory, (i + 1));
		fp = fopen(fileName, "rb");
		if (fp == NULL)
		{
			sprintf(saveSlot[i], "%.2d - Empty", (i + 1));
			if (engine.gameSection == SECTION_TITLE)
				graphics.textSurface(13 + i, saveSlot[i], -1, imagePos, FONT_WHITE);
		}
		else
		{
			if (fread(&tempGame, sizeof(Game), 1, fp) != 1)
			{
				sprintf(saveSlot[i], "%.2d - Corrupt Game Data", (i + 1));
			}
			else
			{
				sprintf(saveSlot[i], "%.2d - %s, %s", (i + 1), systemNames[tempGame.system], tempGame.stationedName);
				if (engine.gameSection == SECTION_TITLE)
					graphics.textSurface(13 + i, saveSlot[i], -1, imagePos, FONT_WHITE);
			}

			if (stat(fileName, &fileInfo) != -1)
			{
				if (fileInfo.st_mtime > modTime)
					{modTime = fileInfo.st_mtime; continueSaveIndex = (i + 1);}
			}

			fclose(fp);
		}
		imagePos += 20;
	}

	return continueSaveIndex;
}

/*
Fill in later...
*/
signed char loadGame(int slot)
{
	char filename[PATH_MAX];
	FILE *fp;
	sprintf(filename, "%ssave%.2d.dat", engine.userHomeDirectory, slot);

	fp = fopen(filename, "rb");

	if (fp == NULL)
		return 0;

	if (fread(&currentGame, sizeof(Game), 1, fp) != 1)
	{
		printf("Save game error. The file was not of the expected format.\n");
		fclose(fp);
		return 0;
	}

	fclose(fp);

	weapon[0] = currentGame.playerWeapon;
	weapon[1] = currentGame.playerWeapon2;
	player = currentGame.thePlayer;

	// Re-init all the planets in this system...
	initPlanetMissions(currentGame.system);

	// ... and then override with completition status
	for (int i = 0 ; i < 10 ; i++)
		systemPlanet[i].missionCompleted = currentGame.missionCompleted[i];

	return 1;
}

void saveGame(int slot)
{
	if ((slot < 1) || (slot > 5))
	{
		printf("Error - Saves may only be 1 to 5\n");
		return;
	}

	FILE *fp;
	char fileName[PATH_MAX];
	sprintf(fileName, "%ssave%.2d.dat", engine.userHomeDirectory, slot);
	fp = fopen(fileName, "wb");

	currentGame.playerWeapon = weapon[0];
	currentGame.playerWeapon2 = weapon[1];
	currentGame.thePlayer = player;
	for (int i = 0 ; i < 10 ; i++)
		currentGame.missionCompleted[i] = systemPlanet[i].missionCompleted;

	if (fp != NULL)
	{
		if (fwrite(&currentGame, sizeof(Game), 1, fp) != 1)
		{
			printf("Error Saving Game to Slot %d\n", slot);
		}
		fclose(fp);
	}
	else
	{
		printf("Error Saving Game to Slot %d\n", slot);
	}

	// Recall to update the save slots... (lazy, yes)
	initSaveSlots();
	engine.keyState[SDLK_LCTRL] = engine.keyState[SDLK_RCTRL] = 0;
}

void createSavesSurface(SDL_Surface *savesSurface, signed char clickedSlot)
{
	graphics.blevelRect(savesSurface, 0, 0, 348, 298, 0x00, 0x00, 0x00);

 	int y = 10;

	for (int i = 0 ; i < 5 ; i++)
	{
		if (clickedSlot == i)
			graphics.blevelRect(savesSurface, 5, y, 338, 25, 0x99, 0x00, 0x00);
		else
			graphics.blevelRect(savesSurface, 5, y, 338, 25, 0x00, 0x00, 0x99);
		graphics.drawString(saveSlot[i], 70, y + 5, FONT_WHITE, savesSurface);
		y += 30;
	}

	graphics.drawString("*** HELP ***", 120, 170, FONT_WHITE, savesSurface);

	switch(clickedSlot)
	{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
			graphics.blevelRect(savesSurface, 5, 265, 100, 25, 0x00, 0x99, 0x00);
			graphics.blevelRect(savesSurface, 125, 265, 100, 25, 0x99, 0x99, 0x00);
			graphics.blevelRect(savesSurface, 243, 265, 100, 25, 0x99, 0x00, 0x00);
			graphics.drawString("SAVE", 40, 270, FONT_WHITE, savesSurface);
			graphics.drawString("CANCEL", 150, 270, FONT_WHITE, savesSurface);
			graphics.drawString("DELETE", 270, 270, FONT_WHITE, savesSurface);

			graphics.drawString("SAVE will save the game", 17, 200, FONT_WHITE, savesSurface);
			graphics.drawString("CANCEL will unselect that slot", 17, 220, FONT_WHITE, savesSurface);
			graphics.drawString("DELETE will remove the save", 17, 240, FONT_WHITE, savesSurface);
			break;
		case -1:
			graphics.drawString("First click a Save game slot to use", 17, 200, FONT_WHITE, savesSurface);
			break;
		case -10:
			graphics.drawString("Game Saved", 130, 200, FONT_WHITE, savesSurface);
			break;
		case -11:
			graphics.drawString("Save Deleted", 130, 200, FONT_WHITE, savesSurface);
			break;
	}

	engine.keyState[SDLK_LCTRL] = engine.keyState[SDLK_RCTRL] = 0;
}

/*
Displays the save slot available. For use with an interface that
has the cursor enabled. It returns the index number of the slot clicked
so that the function invoking it can perform a load or save on that slot.
*/
int showSaveSlots(SDL_Surface *savesSurface, signed char saveSlot)
{
	SDL_Rect r;
	r.x = 201;
	r.y = 115;
	r.w = 348;
	r.h = 25;

	int clickedSlot = -1;

	if ((engine.keyState[SDLK_LCTRL]) || (engine.keyState[SDLK_RCTRL]))
	{
		for (int i = 0 ; i < 5 ; i++)
		{
			if (Collision::collision(engine.cursor_x + 13, engine.cursor_y + 13, 6, 6, r.x, r.y, r.w, r.h))
			{
				clickedSlot = i;
				createSavesSurface(savesSurface, i);
			}
			r.y += 30;
		}

		if (Collision::collision(engine.cursor_x + 13, engine.cursor_y + 13, 6, 6, 215, 365, 100, 25))
		{
			saveGame(saveSlot + 1);
			createSavesSurface(savesSurface, -10);
		}

		if (Collision::collision(engine.cursor_x + 13, engine.cursor_y + 13, 6, 6, 335, 365, 100, 25))
			createSavesSurface(savesSurface, -1);

		if (Collision::collision(engine.cursor_x + 13, engine.cursor_y + 13, 6, 6, 453, 365, 100, 25))
		{
			char filename[PATH_MAX];
			sprintf(filename, "%ssave%.2d.dat", engine.userHomeDirectory, (saveSlot + 1));
			remove(filename);
			initSaveSlots();
			createSavesSurface(savesSurface, -11);
		}
	}

	if (clickedSlot > -1)
		saveSlot = clickedSlot;

	return saveSlot;
}