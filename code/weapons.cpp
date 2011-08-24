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

#include "weapons.h"

void setWeaponShapes()
{
	for (int i = 0 ; i < MAX_WEAPONS ; i++)
	{
		weapon[i].image[0] = graphics.shape[weapon[i].imageIndex[0]];
		weapon[i].image[1] = graphics.shape[weapon[i].imageIndex[1]];
	}
}

#if USEPACK

void loadWeapons()
{
	int dataLocation = locateDataInPak("data/weapons.dat", 1);
	int id, ammo, damage, reload, speed, image1, image2, flags;

	FILE *fp;

	fp = fopen(PACKLOCATION, "rb");
	fseek(fp, dataLocation, SEEK_SET);

	for (int i = 0 ; i < MAX_WEAPONS ; i++)
	{
		fscanf(fp, "%d", &id);
		fscanf(fp, "%d", &ammo);
		fscanf(fp, "%d", &damage);
		fscanf(fp, "%d", &reload);
		fscanf(fp, "%d", &speed);
		fscanf(fp, "%d", &image1);
		fscanf(fp, "%d", &image2);
		fscanf(fp, "%d", &flags);

		weapon[i].id = id;
		weapon[i].ammo[0] = ammo;
		weapon[i].damage = damage;
		weapon[i].reload[0] = reload;
		weapon[i].speed = speed;
		weapon[i].imageIndex[0] = image1;
		weapon[i].imageIndex[1] = image2;
		weapon[i].flags = flags;
	}

	fclose(fp);
}

void initWeapons() {loadWeapons();}

#else

void saveWeapons()
{
	FILE *fp;

	fp = fopen("data/weapons.dat", "wb");
	if (fp == NULL)
	{
		printf("Unable to write Weapon Data File\n");
		exit(1);
	}

	for (int i = 0 ; i < MAX_WEAPONS ; i++)
	{	
		fprintf(fp, "%d ", weapon[i].id);
		fprintf(fp, "%d ", weapon[i].ammo[0]);
		fprintf(fp, "%d ", weapon[i].damage);
		fprintf(fp, "%d ", weapon[i].reload[0]);
		fprintf(fp, "%d ", weapon[i].speed);
		fprintf(fp, "%d ", weapon[i].imageIndex[0]);
		fprintf(fp, "%d ", weapon[i].imageIndex[1]);
		fprintf(fp, "%d\n", weapon[i].flags);
	}

	// Put an extra line for the PAK file "just in case"
	fprintf(fp, "\n");

	fclose(fp);
}

/*
A list of predefined weaponary. Will most probably
be placed into a data file in the final build.
*/
void initWeapons()
{
	// Player's weapon (this NEVER allocated to anything else)
	weapon[W_PLAYER_WEAPON].id = WT_PLASMA;
	weapon[W_PLAYER_WEAPON].ammo[0] = 1;
	weapon[W_PLAYER_WEAPON].damage = 1;
	weapon[W_PLAYER_WEAPON].reload[0] = 15;
	weapon[W_PLAYER_WEAPON].speed = 10;
	weapon[W_PLAYER_WEAPON].imageIndex[0] = 0;
	weapon[W_PLAYER_WEAPON].imageIndex[1] = 0;
   weapon[W_PLAYER_WEAPON].flags = WF_STRAIGHT;

	// Nor is this one!
	weapon[W_PLAYER_WEAPON2] = weapon[W_PLAYER_WEAPON];

	// Single Shot
	weapon[W_SINGLE_SHOT].id = WT_PLASMA;
	weapon[W_SINGLE_SHOT].ammo[0] = 1;
	weapon[W_SINGLE_SHOT].damage = 1;
	weapon[W_SINGLE_SHOT].reload[0] = 15;
	weapon[W_SINGLE_SHOT].speed = 10;
	weapon[W_SINGLE_SHOT].imageIndex[0] = 0;
	weapon[W_SINGLE_SHOT].imageIndex[1] = 1;
   weapon[W_SINGLE_SHOT].flags = WF_STRAIGHT;

	// Double Shot
	weapon[W_DOUBLE_SHOT] = weapon[W_SINGLE_SHOT];
	weapon[W_DOUBLE_SHOT].ammo[0] = 2;

	// Triple Shot
	weapon[W_TRIPLE_SHOT] = weapon[W_SINGLE_SHOT];
	weapon[W_TRIPLE_SHOT].ammo[0] = 3;

	// Rockets
	weapon[W_ROCKETS].id = WT_ROCKET;
	weapon[W_ROCKETS].ammo[0] = 1;
	weapon[W_ROCKETS].damage = 15;
	weapon[W_ROCKETS].reload[0] = 45;
	weapon[W_ROCKETS].speed = 20;
  	weapon[W_ROCKETS].flags = WF_STRAIGHT;
  	weapon[W_ROCKETS].imageIndex[0] = 2;
	weapon[W_ROCKETS].imageIndex[1] = 3;

	// Double Rockets (uses ROCKETS as base)
	weapon[W_DOUBLE_ROCKETS] = weapon[W_ROCKETS];
	weapon[W_DOUBLE_ROCKETS].ammo[0] = 2;
	weapon[W_DOUBLE_ROCKETS].reload[0] = 80;

	// Micro Rockets
	weapon[W_MICRO_ROCKETS].id = WT_ROCKET;
	weapon[W_MICRO_ROCKETS].ammo[0] = 5;
	weapon[W_MICRO_ROCKETS].damage = 3;
	weapon[W_MICRO_ROCKETS].reload[0] = 30;
	weapon[W_MICRO_ROCKETS].speed = 15;
	weapon[W_MICRO_ROCKETS].flags = WF_STRAIGHT + WF_VARIABLE_SPEED;
	weapon[W_MICRO_ROCKETS].imageIndex[0] = 2;
	weapon[W_MICRO_ROCKETS].imageIndex[1] = 3;

	// Energy Ray
	weapon[W_ENERGYRAY].id = WT_ENERGYRAY;
	weapon[W_ENERGYRAY].ammo[0] = 255;
	weapon[W_ENERGYRAY].damage = 1;
	weapon[W_ENERGYRAY].reload[0] = 25; // reload for energy ray is never used
	weapon[W_ENERGYRAY].speed = 15;
	weapon[W_ENERGYRAY].flags = WF_STRAIGHT;

	// Laser
	weapon[W_LASER].id = WT_LASER;
	weapon[W_LASER].ammo[0] = 1;
	weapon[W_LASER].damage = 3;
	weapon[W_LASER].reload[0] = 1;
	weapon[W_LASER].speed = 10;
	weapon[W_LASER].imageIndex[0] = 1;
	weapon[W_LASER].imageIndex[1] = 1;
   weapon[W_LASER].flags = WF_STRAIGHT;

	// Beam up weapon
	weapon[W_CHARGER].id = WT_CHARGER;
	weapon[W_CHARGER].ammo[0] = 1;
	weapon[W_CHARGER].damage = 1;
	weapon[W_CHARGER].reload[0] = 0;
	weapon[W_CHARGER].speed = 12;
  	weapon[W_CHARGER].flags = WF_STRAIGHT;
  	weapon[W_CHARGER].imageIndex[0] = 33;
	weapon[W_CHARGER].imageIndex[1] = 34;

 	// Homing missile
	weapon[W_HOMING_MISSILE].id = WT_ROCKET;
	weapon[W_HOMING_MISSILE].ammo[0] = 1;
	weapon[W_HOMING_MISSILE].damage = 15;
	weapon[W_HOMING_MISSILE].reload[0] = 35;
	weapon[W_HOMING_MISSILE].speed = 10;
  	weapon[W_HOMING_MISSILE].flags = WF_STRAIGHT + WF_HOMING;
  	weapon[W_HOMING_MISSILE].imageIndex[0] = 4;
	weapon[W_HOMING_MISSILE].imageIndex[1] = 4;

  	// Double homing missile
	weapon[W_DOUBLE_HOMING_MISSILES] = weapon[W_HOMING_MISSILE];
  	weapon[W_DOUBLE_HOMING_MISSILES].ammo[0] = 2;
  	weapon[W_DOUBLE_HOMING_MISSILES].reload[0] = 65;
  	weapon[W_DOUBLE_HOMING_MISSILES].imageIndex[0] = 4;
	weapon[W_DOUBLE_HOMING_MISSILES].imageIndex[1] = 4;

	// Micro homing missiles
	weapon[W_MICRO_HOMING_MISSILES].id = WT_ROCKET;
	weapon[W_MICRO_HOMING_MISSILES].ammo[0] = 5;
	weapon[W_MICRO_HOMING_MISSILES].damage = 12;
	weapon[W_MICRO_HOMING_MISSILES].reload[0] = 65;
	weapon[W_MICRO_HOMING_MISSILES].speed = 3;
  	weapon[W_MICRO_HOMING_MISSILES].flags = WF_STRAIGHT + WF_HOMING;
  	weapon[W_MICRO_HOMING_MISSILES].imageIndex[0] = 4;
	weapon[W_MICRO_HOMING_MISSILES].imageIndex[1] = 4;

	// Aimed plasma bolt (2x damage)
	weapon[W_AIMED_SHOT].id = WT_DIRECTIONAL;
	weapon[W_AIMED_SHOT].ammo[0] = 1;
	weapon[W_AIMED_SHOT].damage = 2;
	weapon[W_AIMED_SHOT].reload[0] = 15;
	weapon[W_AIMED_SHOT].speed = 0;
	weapon[W_AIMED_SHOT].flags = WF_STRAIGHT + WF_AIMED;
	weapon[W_AIMED_SHOT].imageIndex[0] = 33;
	weapon[W_AIMED_SHOT].imageIndex[1] = 34;

	// 3 way spread weapon
	weapon[W_SPREADSHOT].id = WT_SPREAD;
	weapon[W_SPREADSHOT].ammo[0] = 3;
	weapon[W_SPREADSHOT].damage = 1;
	weapon[W_SPREADSHOT].reload[0] = 10;
	weapon[W_SPREADSHOT].speed = 10;
	weapon[W_SPREADSHOT].flags = WF_THIN_SPREAD;
	weapon[W_SPREADSHOT].imageIndex[0] = 0;
	weapon[W_SPREADSHOT].imageIndex[1] = 1;

	// Sid's ion cannon like weapon
	weapon[W_IONCANNON].id = WT_PLASMA;
	weapon[W_IONCANNON].ammo[0] = 1;
	weapon[W_IONCANNON].damage = 1;
	weapon[W_IONCANNON].reload[0] = 2;
	weapon[W_IONCANNON].speed = 10;
	weapon[W_IONCANNON].flags = WF_STRAIGHT + WF_DISABLE + WF_AIMED;
	weapon[W_IONCANNON].imageIndex[0] = 35;
	weapon[W_IONCANNON].imageIndex[1] = 35;

	// Directional Shock Missile - Used by Kline in final battle
	weapon[W_DIRSHOCKMISSILE].id = WT_ROCKET;
	weapon[W_DIRSHOCKMISSILE].ammo[0] = 5;
	weapon[W_DIRSHOCKMISSILE].damage = 20;
	weapon[W_DIRSHOCKMISSILE].reload[0] = 60;
	weapon[W_DIRSHOCKMISSILE].speed = 0;
	weapon[W_DIRSHOCKMISSILE].flags = WF_STRAIGHT + WF_AIMED + WF_TIMEDEXPLOSION;
	weapon[W_DIRSHOCKMISSILE].imageIndex[0] = 4;
	weapon[W_DIRSHOCKMISSILE].imageIndex[1] = 4;

	saveWeapons();
}

#endif