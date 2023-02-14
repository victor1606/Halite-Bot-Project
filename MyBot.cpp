#include <stdlib.h>
#include <time.h>
#include <cstdlib>
#include <ctime>
#include <time.h>
#include <set>
#include <fstream>
#include <climits>
#include <queue>
#include <fstream>

#include "hlt.hpp"
#include "networking.hpp"
#include "bot_functions.h"

using namespace std;
using namespace hlt;


int main() {
	srand(time(NULL));

	cout.sync_with_stdio(0);

	unsigned char myID;
	GameMap presentMap;
	getInit(myID, presentMap);

	// Team name
	sendInit("skel");

	set<Move> moves;
	while (true) {
		moves.clear();

		getFrame(presentMap);

		// Vector used to keep a queue of scores
		vector<Site> scoreQueue;
		
		// Adding coordinates for every site
		set_coords(presentMap);

		// Sets scores for all sites around the bot
		set_border_scores(presentMap, myID, scoreQueue);

		// Matrix that keeps track of sites that have been scored
		vector<vector<bool>> scored =
			vector<vector<bool>>(presentMap.height, vector<bool>(presentMap.width, false));

		// Keeps scores sorted in queue
		sort(scoreQueue.begin(), scoreQueue.end(), compare_by_score);


		while (!scoreQueue.empty()) {
			// Pops current site
			scoreQueue.pop_back();

			for (unsigned char d : CARDINALS) {
				// Extracts all neighbours of site

				Site &aux_site = scoreQueue.back();
				unsigned short int aux_x = aux_site.x;
				unsigned short int aux_y = aux_site.y;
				Site &neighbour = presentMap.getSite({aux_x, aux_y}, d);

				// Checks if the neighbouring site is owned by the player and is not scored yet
				
				if (neighbour.owner == myID && !scored[neighbour.y][neighbour.x]) {
					// Marks site as scored 
					scored[neighbour.y][neighbour.x] = true;
					// Scoring formula
					neighbour.score = scoreQueue.back().score - neighbour.production - 2;

					unsigned short reverse;

					if (d == NORTH) {
						reverse = SOUTH;
					} else if (d == SOUTH) {
						reverse = NORTH;
					} else if (d == EAST) {
						reverse = WEST;
					} else if (d == WEST) {
						reverse = EAST;
					}
					neighbour.direction = reverse;

					// Adds neighbour to queue
					scoreQueue.push_back(neighbour);

					// Keep vector sorted
					sort(scoreQueue.begin(), scoreQueue.end(), compare_by_score);
				}
			}
		}

		// Get best move and insert to moves set
		int height = presentMap.height;
		int width = presentMap.width;

		populate_move_set(presentMap, myID, moves, height, width);

		// Send moves
		sendFrame(moves);
	}

	return 0;
}