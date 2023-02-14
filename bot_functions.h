#ifndef BOTFN_H
#define BOTFN_H

#include "hlt.hpp"
#include "networking.hpp"

#define MAX_STRENGTH 255
#define MINIMUM_VAL -99999999

using namespace hlt;

// Adding coordinates for every site
void set_coords(GameMap& presentMap);

// Sets scores for all sites around the bot
void set_border_scores(GameMap presentMap, unsigned char myID, std::vector<Site> &scoreQueue);

// Comparator function used to sort Site vector by score
bool compare_by_score(Site &lhs, Site &rhs);

// Verifica daca un site e la margine
bool on_border(unsigned short int x, unsigned short int y, GameMap &presentMap, int myID);

// Obtains best move for a cell at the current time
Move get_best_move(unsigned short int x, unsigned short int y, GameMap &presentMap, int myID);

// Uses de getMove function to populate move set
void populate_move_set(GameMap presentMap, int myID, std::set<Move> &moves, int height, int width);

#endif
