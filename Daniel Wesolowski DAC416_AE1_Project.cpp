// Daniel Wesolowski DAC416_AE1_Project.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <conio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <ctime>
#include <cstdlib>


using namespace std;

bool gameFinished = false;
bool isPlayerMove = true;
int turnCounter;

struct CharacterEntity {
	string name;
	int health;
	int maxHealth;
	int energy;
	int maxEnergy;
	int baseRegen; // Every round get 10 energy.
	bool isDodging;
	bool isRegen;
	bool isHealing;
};

struct AbilitySpecs {
	string name;
	ostringstream description;

	int minDmg; // Attack = 1 | Smite = 5
	int maxDmg; // Attack = 8 | Smite = 20
	int maxHitChance; // Special - 50 max
	int cost;
	int buff; // Heal = energy / 2 = new health. Dodge = 30% reduce to other players hit change
	int debuff; // Regenerating = 10% more chance other player will hit you.
};


CharacterEntity Player;
CharacterEntity Enemy;

AbilitySpecs Slash;
AbilitySpecs Special;
AbilitySpecs Dodge; // debuff = 2
AbilitySpecs Regen; // buff = 4
AbilitySpecs Heal;

void arena();

int generateRandomNumber(int maxNumber) {
	int randomNumber = NULL;
	randomNumber = (rand() % maxNumber) + 1;
	return randomNumber;
}

void initPlayer() {
	Player.name = "Player";
	Player.health = 100;
	Player.energy = 100;
	Player.maxHealth = 100;
	Player.maxEnergy = 100;
	Player.baseRegen = 10;
	Player.isDodging = false;
	Player.isHealing = false;
	Player.isRegen = false;

}

void initEnemy() {
	Enemy.name = "Enemy";
	Enemy.health = 100;
	Enemy.energy = 100;
	Enemy.maxHealth = 100;
	Enemy.maxEnergy = 100;
	Enemy.baseRegen = 10;
	Enemy.isDodging = false;
	Enemy.isHealing = false;
	Enemy.isRegen = false;
}

void initSkills() {
	Slash.name = "Slash";
	Slash.minDmg = 1;
	Slash.maxDmg = 10;
	Slash.maxHitChance = 80;
	Slash.cost = 0;
	Slash.buff = 0;
	Slash.debuff = 0;
	Slash.description << "Does " << Slash.minDmg << " mininum dmg and " << Slash.maxDmg << " max dmg, at the cost of " << Slash.cost << " energy.";

	Special.name = "Special";
	Special.minDmg = 5;
	Special.maxDmg = 20;
	Special.cost = 50;
	Special.maxHitChance = 50;
	Special.buff = 0;
	Special.debuff = 0;
	Special.description << "Does " << Special.minDmg << " minimum dmg and " << Special.maxDmg << " max dmg, at the cost of " << "energy.";

	Dodge.name = "Dodge";
	Dodge.buff = 30;
	Dodge.debuff = 2;
	Dodge.description << "Decreases enemys chance of hitting by 30%, but decreases energy charging rate by 50% during this turn.";


	Regen.name = "Re-charge";
	Regen.buff = 4;
	Regen.debuff = 10;
	Regen.description << "Player recharges energy at 4 times the normal rate. 10% more chance opponent will hit you.";

	Heal.name = "Heal";
	Heal.buff = 50;
	Heal.debuff = 0;
	Heal.description << " Convert half of the energy into health";

}




/*
/
/ changeTurn() Regenerates players energy based on their status
/ (aka isDodging.)
/
*/
void changeTurn(bool canMoveAgain) {

	if (canMoveAgain) {
		cout << "< Press enter to continue >" << endl;
		_getch();
		arena();
		return;
	}

	turnCounter++; // You add one to turnCoutner to know how many turns have been done in the game.


	if (!isPlayerMove) { // If it WAS players move.
		int energyToRecharge = Player.baseRegen;

		if (Player.isDodging) {
			energyToRecharge /= Dodge.debuff; // Debuff is 50% off normal recharge, therefore divide by 2.
			Player.isDodging = false;
		}
		else if (Player.isRegen) {
			energyToRecharge *= Regen.buff; // Buff is *4 normal recharge. So 10*4, you'll regen 40.
			Player.isRegen = false;
		}

		if (Player.isHealing) Player.isHealing = false;

		cout << "You've regened " << energyToRecharge << " energy this round." << endl;
		cout << endl;

		Player.energy += energyToRecharge;
		if (Player.energy > Player.maxEnergy) Player.energy = Player.maxEnergy;
		isPlayerMove = true;
	}
	else {
		int energyToRecharge = Enemy.baseRegen;

		if (Enemy.isDodging) {
			energyToRecharge /= Dodge.debuff;
			Enemy.isDodging = false;
		}
		else if (Enemy.isRegen) {
			energyToRecharge *= Regen.buff;
			Enemy.isRegen = false;
		}

		Enemy.energy += energyToRecharge;
		if (Enemy.energy > Enemy.maxEnergy) Enemy.energy = Enemy.maxEnergy;

		cout << "Enemy regened " << energyToRecharge << " energy this round." << endl;
		isPlayerMove = false;

		if (Enemy.isHealing) Enemy.isHealing = false;
	}
	cout << "< Press enter to continue >" << endl;
	_getch();
	arena();
}

void slashMove() {
	if (isPlayerMove) {
		int hitChance = Slash.maxHitChance;

		if (Enemy.isDodging) hitChance -= Dodge.buff; // hitChange = hitChange - Dodge.debuff;
		if (Enemy.isRegen) hitChance += Regen.debuff;

		int randomNumber = generateRandomNumber(100);

		if (randomNumber <= hitChance) {
			int moveMinDmg = Slash.minDmg;
			int moveMaxDmg = Slash.maxDmg;

			randomNumber = generateRandomNumber(moveMaxDmg);
			if (randomNumber > moveMaxDmg) randomNumber = moveMaxDmg;

			Enemy.health -= randomNumber;

			cout << string(100, '\n');
			cout << "You've used normal attack on the Enemy, and hit them for " << randomNumber << endl;
			changeTurn(false);
		}
		else {

			cout << string(100, '\n');
			cout << "You've missed." << endl;
			changeTurn(false);
		}

	}
	else {
		int hitChance = Slash.maxHitChance;

		if (Player.isDodging) hitChance -= Dodge.buff; // hitChange = hitChange - Dodge.debuff;
		if (Player.isRegen) hitChance += Regen.debuff;

		int randomNumber = generateRandomNumber(20);

		if (randomNumber <= hitChance) {
			int moveMinDmg = Slash.minDmg;
			int moveMaxDmg = Slash.maxDmg;

			randomNumber = generateRandomNumber(moveMaxDmg);
			if (randomNumber > moveMaxDmg) randomNumber = moveMaxDmg;

			Player.health -= randomNumber;

			cout << string(100, '\n');
			cout << "Enemy used normal move on you, and hit you for " << randomNumber << endl;
			changeTurn(false);
		}
		else {

			cout << string(100, '\n');
			cout << "The enemy missed." << endl;
			changeTurn(false);
		}
	}
}

void specialMove() {

	if (isPlayerMove) {

		if (Player.energy >= Special.cost) {
			int hitChance = Special.maxHitChance;
			if (Enemy.isDodging) hitChance -= Dodge.buff; // hitChange = hitChange - Dodge.debuff;
			if (Enemy.isRegen) hitChance += Regen.debuff;

			int randomNumber = generateRandomNumber(50);

			if (randomNumber <= hitChance) {
				int moveMinDmg = Special.minDmg;
				int moveMaxDmg = Special.maxDmg;

				randomNumber = generateRandomNumber(moveMaxDmg);
				if (randomNumber < moveMinDmg) randomNumber = moveMinDmg;

				Enemy.health -= randomNumber;

				Player.energy -= Special.cost;

				cout << string(100, '\n');
				cout << "You've used special attack and hitted the Enemy for " << randomNumber << endl;
				changeTurn(false);
			}
			else {
				Player.energy -= Special.cost;
				cout << string(100, '\n');
				cout << "You've missed." << endl;
				changeTurn(false);
			}
		}
		else {
			cout << string(100, '\n');
			cout << "Not enough energy" << endl;
			changeTurn(true);
		}
	}
	else {
		int hitChance = Special.maxHitChance;

		if (Player.isDodging) hitChance -= Dodge.buff; // hitChange = hitChange - Dodge.debuff;
		if (Player.isRegen) hitChance += Regen.debuff;

		int randomNumber = generateRandomNumber(100);

		if (randomNumber <= hitChance) {
			int moveMinDmg = Special.minDmg;
			int moveMaxDmg = Special.maxDmg;

			randomNumber = generateRandomNumber(moveMaxDmg);
			if (randomNumber < moveMinDmg) randomNumber = moveMinDmg;

			Player.health -= randomNumber;
			Enemy.energy -= Special.cost;

			cout << string(100, '\n');
			cout << "Enemy used special attack on you and hitted you for " << randomNumber << endl;
			changeTurn(false);
		}
		else {

			cout << string(100, '\n');
			cout << "The enemy missed." << endl;
			changeTurn(false);
		}
	}

}

void dodgeMove() {

	if (isPlayerMove)
	{
		Player.isDodging = true;
		cout << string(100, '\n');
		cout << "You are dodging." << endl;
		changeTurn(false);
	}
	else
	{
		Enemy.isDodging = false;
		cout << string(100, '\n');
		cout << "Enemy is dodging." << endl;
		changeTurn(false);
	}

}

void regenMove() {

	if (isPlayerMove)
	{
		if (Player.energy == Player.maxEnergy) {

			cout << string(100, '\n');
			cout << "Your energy is full." << endl;
			changeTurn(true);


		}
		else {
			cout << string(100, '\n');
			cout << "You're now regenerating energy at " << Regen.buff << " times the rate." << endl;
			cout << "Enemy got 10% more chance to hit you" << endl;
			Player.isRegen = true;
			changeTurn(false);

		}
	}
}

void healMove() {
	if (isPlayerMove) {																					 // If it's players move
		if (!Player.isHealing) {																			// Make sure player is NOT healing
			if (Player.energy >= 20) {																			// Check if player has above 20 energy
				double doubleHealthToRegen = Player.energy / 2;												// If you have 25 energy you'd get a decimal if you divide it by 2
				int healthToRegen = round(doubleHealthToRegen);						// Round the number above to nearest whole digit

				Player.health += healthToRegen;													
																								//  add new health to exdisting health
				if (Player.health > Player.maxHealth) Player.health = Player.maxHealth;										// Make sure that the player is still within his maxHealth, if he's over it set it to max.
				Player.energy = 0;
				cout << string(100, '\n');
				cout << "You've healed " << healthToRegen << endl;							// Tell player how much they healed
				Player.isHealing = true;												 // set isHealing to true, so they can't heal again.
				changeTurn(true);														// changeTurn(true) because they CAN move again.
			}
			else {
				cout << string(100, '\n');
				cout << "You don't have enough energy, try again." << endl;													// If they don't have enough health tell them
				changeTurn(true);																		// get them back to their options and let them move again.
			}
		}
		else {
			cout << "You already healed, try different move" << endl;						
			changeTurn(true);													
		}
	}
	else {
		if (!Enemy.isHealing) {
			if (Enemy.energy >= 20) {
				double doubleHeathToRegen = Enemy.energy / 2;
				int healthToRegen = round(doubleHeathToRegen);

				Enemy.health += healthToRegen;

				if (Enemy.health > Player.maxHealth) Player.health = Player.maxEnergy;

				Enemy.energy = 0;
				cout << "Enemy healed for " << healthToRegen << endl;
				Player.isHealing = false;
				changeTurn(true);
			}
		}
		else {
			changeTurn(true);
		}
	}
}

void enemyLogic() {
	int specialAttackChance = 20,
		normalAttackChange = 80,
		dodgeMoveChance = 30;

	if (Player.health < 10) {
		specialMove();
	}
	else if (Enemy.energy >= 50 && Player.isRegen) {
		specialMove();
	}
	else if (Enemy.energy > specialAttackChance && Enemy.health > specialAttackChance && Enemy.energy > Special.cost) {
		if (Player.isDodging) {
			slashMove();
		}
		else { specialMove(); }
	}
	else if (Enemy.health > normalAttackChange) {
		slashMove();
	}
	else if (Enemy.health <= 35 && Enemy.energy >= 30)
	{
		healMove();
		int randomNumber = generateRandomNumber(10);
		if (randomNumber <= 3) dodgeMove();
		else regenMove();
		
	}
	else if (Enemy.energy > dodgeMoveChance) {
		int randomNumber = generateRandomNumber(10);
		if (randomNumber <= 3) dodgeMove();
		else slashMove();
	}
	else {
		healMove();
	}
}

void endGame() {

	if (Player.health <= 0)
	{
		cout << string(100, '\n');
		cout << "Enemy Won" << endl;
	}

	else
	{
		cout << string(100, '\n');
		cout << "You Won" << endl;
	}

}

void arena() {
	if (Player.health > 0 && Enemy.health > 0) {

		if (isPlayerMove) {

			int playerChoice = NULL;

		battleArena:
			cout << string(100, '\n');
			cout << " | " << Player.name << " health: " << Player.health << " | ";
			cout.width(75);
			cout << " | " << Enemy.name << " health: " << Enemy.health << " | " << endl;
			cout << " | " << Player.name << " energy: " << Player.energy << " | ";
			cout.width(75);
			cout << " | " << Enemy.name << " energy: " << Enemy.energy << " | " << endl;
			cout << string(5, '\n');
			cout << "Turn number: " << turnCounter / 2 << endl;
			cout << string(15, '\n');
			


			cout << "1 - " << Slash.name << " | " << Slash.description.str() << endl;
			cout << "2 - " << Special.name << " | " << Special.description.str() << endl;
			cout << "3 - " << Dodge.name << " | " << Dodge.description.str() << endl;
			cout << "4 - " << Regen.name << " | " << Regen.description.str() << endl;
			cout << "5 - " << Heal.name << " | " << Heal.description.str() << endl;
			cout << endl;

			cin >> playerChoice;

			if (playerChoice == 1) slashMove();
			else if (playerChoice == 2) specialMove();
			else if (playerChoice == 3) dodgeMove();
			else if (playerChoice == 4) regenMove();
			else if (playerChoice == 5) healMove();
			else {
				cout << "Wrong choice, try again." << endl;
				goto battleArena;
			}
		}
		else {
			enemyLogic();
		}

	}
	else { // If game is finished
		gameFinished = true;
		endGame();
	}
}

int main() {
	srand(time(NULL));
	bool startup = true;

	if (startup) {
		initPlayer();
		initEnemy();
		initSkills();

		cout << string(100, '\n');
		cout << "Whats your name ? " << endl;
		cin >> Player.name;

		gameFinished = false;
		startup = false;
	}

	do { arena(); } while (!gameFinished); // Will execute arena() whilst game isn't finished. When game is finished code below will execute.

	_getch();
}

