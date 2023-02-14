#include <stdlib.h>
#include "hlt.hpp"
#include "networking.hpp"
#include "bot_functions.h"

using namespace hlt;

// Adding coordinates for every site
void set_coords(GameMap& presentMap) {
	int height = presentMap.height;
	int width = presentMap.width;

	for (unsigned short i = 0; i < height; i++) {
		for (unsigned short j = 0; j < width; j++) {
			Site &site = presentMap.getSite({j, i});
			site.x = j;
			site.y = i;
		}
	}	
}

// Sets scores for all sites around the bot
void set_border_scores(GameMap presentMap, unsigned char myID,
				std::vector<Site> &scoreQueue) {
	int height = presentMap.height;
	int width = presentMap.width;

    for (unsigned short i = 0; i < height; i++) {
		for (unsigned short j = 0; j < width; j++) {
			Site &site = presentMap.getSite({j, i});
			
			if (site.owner != myID) {
				bool site_on_border = false;
			
				for (unsigned char d : CARDINALS) {
					Site &neighbour = presentMap.getSite({j, i}, d);
						
					if (neighbour.owner == myID) {
						site_on_border = true;
						break;
					}
				}

				// Constants obtained experimentally
				if (site_on_border) {
					int new_production = site.production * 5;
					int new_strength = site.strength * 7;
					site.score = new_production - (new_strength / 10);
					scoreQueue.push_back(site);
				}
			}
		}
	}
}

// Comparator function used to sort Site vector by score
bool compare_by_score(Site &site1, Site &site2) {
	return site1.score < site2.score;
}


// Checks if a particular site is located on the border of the bot's territory
bool on_border(unsigned short int x, unsigned short int y, GameMap &presentMap, int myID) {
	for (unsigned char curr_cardinal : CARDINALS) {
		Site neighbour = presentMap.getSite({x, y}, curr_cardinal);

		if (neighbour.owner != myID) {
			return true;
		}
	}
	return false;
}

// Obtains best move for a cell at the current time
Move get_best_move(unsigned short int x, unsigned short int y, GameMap &presentMap, int myID) {
	Site site = presentMap.getSite({x, y});
	unsigned char aux_cardinal = 0;

	int max_production = MINIMUM_VAL;

	if (on_border(x, y, presentMap, myID)) {
		for (unsigned char curr_cardinal : CARDINALS) {
			Site neighbour = presentMap.getSite({x, y}, curr_cardinal);
			
			// Checks if neighbour is owned by the player
			if (neighbour.owner != myID) {
				Site aux = presentMap.getSite({neighbour.x, neighbour.y}, curr_cardinal);

				// Checks if strength is lower and production is higher
				if (neighbour.strength < site.strength) {
					if (neighbour.production > max_production) {
						aux_cardinal = curr_cardinal;
						max_production = neighbour.production;
					
						// Checks if the site is owned by the player/enemy/nobody
						if (aux.owner == 0 ||
							(aux.owner != 0 && aux.owner != myID && neighbour.owner != 0) ||
							(aux.owner == myID) || 
							aux.strength < site.strength) {
							aux_cardinal = curr_cardinal;
							max_production = neighbour.production;
						} else {
							return {{x, y}, STILL};
						}
					}
				}
			}
		}
	}

	if (max_production != MINIMUM_VAL) {
		return {{x, y}, aux_cardinal};
	}

	// Stands STILL if production is 5 times larger than strength / strength is 255
	int multiplied_prod = 5 * site.production;
	if (site.strength < multiplied_prod) {
		if (site.strength < MAX_STRENGTH) {
			return {{x, y}, STILL};
		}
	}

	Site destination = presentMap.getSite({x, y}, site.direction);

	if (destination.owner == myID || destination.strength < site.strength) {
		return {{x, y}, site.direction};
	} else {
		return {{x, y}, STILL};
	}
}

// Uses de getMove function to populate move set
void populate_move_set(GameMap presentMap, int myID, std::set<Move> &moves, int height, int width) {
	for (unsigned short a = 0; a < height; a++) {
		for (unsigned short b = 0; b < width; b++) {
			if (presentMap.getSite({b, a}).owner == myID) {

				Move best = get_best_move(b, a, presentMap, myID);
				moves.insert(best);
			}
		}
	}
}
