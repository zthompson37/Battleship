// Program for the guessing game Battleship

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <unistd.h> // sleep(seconds)

const int WIDTH = 10, LENGTH = 10, MISSES = 83, HITS = 17, MAX_MOVES = 100,
          CARRIER_LENGTH = 5, BATTLESHIP_LENGTH = 4, CRUISER_LENGTH = 3,
          SUBMARINE_LENGTH = 3, DESTROYER_LENGTH = 2;

enum orientation_t { HORIZONTAL = 0, VERTICAL = 1 };
enum ship_symbol_t { CARRIER_SYMBOL = '!', BATTLESHIP_SYMBOL = '@', CRUISER_SYMBOL = '#',
SUBMARINE_SYMBOL = '$', DESTROYER_SYMBOL = '%' }; // facilitate debugging

using namespace std;

class PlayerBoard
{
public:
     void retrieve_layout(string ship_name, int ship_length);
     void print_player_layout( );
     //void print_AI_layout( );
     //void update_AI_layout( );
     //void update_player_layout( );
     void setup_fleet( );
     int* get_attack_coordinates( );
     int attack_and_update(char position, int row, int column);
     void print_player_attacks( );
     void set_carrier(char orientation_par, char symbol_par, int fixed_position_par, int spread_position_par[])
     {
          this->carrier.orientation = orientation_par;
          this->carrier.symbol = symbol_par;
          this->carrier.fixed_position = fixed_position_par;
          this->carrier.spread_position = spread_position_par;
     }
     char get_carrier_orientation( ) { return this->carrier.orientation; }
     char get_carrier_symbol( ) { return this->carrier.symbol; }
     int get_carrier_fixed_position( ) { return this->carrier.fixed_position; }
     int* get_carrier_spread_position( ) { return this->carrier.spread_position; }
     void free_carrier_spread_position( ) { delete [] this->carrier.spread_position; }
     void set_battleship(char orientation_par, char symbol_par, int fixed_position_par, int spread_position_par[])
     {
          this->battleship.orientation = orientation_par;
          this->battleship.symbol = symbol_par;
          this->battleship.fixed_position = fixed_position_par;
          this->battleship.spread_position = spread_position_par;
     }
     char get_battleship_orientation( ) { return this->battleship.orientation; }
     char get_battleship_symbol( ) { return this->battleship.symbol; }
     int get_battleship_fixed_position( ) { return this->battleship.fixed_position; }
     int* get_battleship_spread_position( ) { return this->battleship.spread_position; }
     void free_battleship_spread_position( ) { delete [] this->battleship.spread_position; }
     void set_cruiser(char orientation_par, char symbol_par, int fixed_position_par, int spread_position_par[])
     {
          this->cruiser.orientation = orientation_par;
          this->cruiser.symbol = symbol_par;
          this->cruiser.fixed_position = fixed_position_par;
          this->cruiser.spread_position = spread_position_par;
     }
     char get_cruiser_orientation( ) { return this->cruiser.orientation; }
     char get_cruiser_symbol( ) { return this->cruiser.symbol; }
     int get_cruiser_fixed_position( ) { return this->cruiser.fixed_position; }
     int* get_cruiser_spread_position( ) { return this->cruiser.spread_position; }
     void free_cruiser_spread_position( ) { delete [] this->cruiser.spread_position; }
     void set_submarine(char orientation_par, char symbol_par, int fixed_position_par, int spread_position_par[])
     {
          this->submarine.orientation = orientation_par;
          this->submarine.symbol = symbol_par;
          this->submarine.fixed_position = fixed_position_par;
          this->submarine.spread_position = spread_position_par;
     }
     char get_submarine_orientation( ) { return this->submarine.orientation; }
     char get_submarine_symbol( ) { return this->submarine.symbol; }
     int get_submarine_fixed_position( ) { return this->submarine.fixed_position; }
     int* get_submarine_spread_position( ) { return this->submarine.spread_position; }
     void free_submarine_spread_position( ) { delete [] this->submarine.spread_position; }
     void set_destroyer(char orientation_par, char symbol_par, int fixed_position_par, int spread_position_par[])
     {
          this->destroyer.orientation = orientation_par;
          this->submarine.symbol = symbol_par;
          this->destroyer.fixed_position = fixed_position_par;
          this->destroyer.spread_position = spread_position_par;
     }
     char get_destroyer_orientation( ) { return this->destroyer.orientation; }
     char get_destroyer_symbol( ) { return this->destroyer.symbol; }
     int get_destroyer_fixed_position( ) { return this->destroyer.fixed_position; }
     int* get_destroyer_spread_position( ) { return this->destroyer.spread_position; }
     void free_destroyer_spread_position( ) { delete [] this->destroyer.spread_position; }
     PlayerBoard( );

private:
     char player_layout[WIDTH][LENGTH];
     char AI_layout[WIDTH][LENGTH];
     char player_attacks[WIDTH][LENGTH];
     struct Carrier
     {
          char orientation;
          char symbol;
          int fixed_position;
          int *spread_position;
          // fixed_position refers to row/column on which ship is located
          // spread_position refers to the numbers corresponding to the length
          // of the ship (e.g., vertical Carrier at column 5, rows 1-5)
     } carrier;
     struct Battleship
     {
          char orientation;
          char symbol;
          int fixed_position;
          int *spread_position;
     } battleship;
     struct Cruiser
     {
          char orientation;
          char symbol;
          int fixed_position;
          int *spread_position;
     } cruiser;
     struct Submarine
     {
          char orientation;
          char symbol;
          int fixed_position;
          int *spread_position;
     } submarine;
     struct Destroyer
     {
          char orientation;
          char symbol;
          int fixed_position;
          int *spread_position;
     } destroyer;
};

class AIBoard
{
public:
     char get_AI_position(int row, int column) { return AI_layout[row - 1][column - 1]; }
     void generate_fleet(int ship_length, ship_symbol_t symbol);
     bool availability(int row, int column); // space availability
     int availability(int row, int column, orientation_t orientation, int ship_length); // file availability
     int randomly_assign( ) { return floor(static_cast<double>(rand( )) * 9/2000000000); } // generate a random number in the range 0-9
     void print_AI_layout( );
     AIBoard( );

private:
     char AI_layout[WIDTH][LENGTH];
     char player_layout[WIDTH][LENGTH];
};
// trivial functions
void print_empty_board( );
void print_game_info( );
bool symbol_check(char symbol_ref);

int main( )
{
     PlayerBoard player;
     AIBoard AI;
     int counter1 = 0, counter2 = 0;

     print_game_info( );
     print_empty_board( );

     /*player.retrieve_layout("carrier", CARRIER_LENGTH);
     player.retrieve_layout("battleship", BATTLESHIP_LENGTH);
     player.retrieve_layout("cruiser", CRUISER_LENGTH);
     player.retrieve_layout("submarine", SUBMARINE_LENGTH);
     player.retrieve_layout("destroyer", DESTROYER_LENGTH);
     player.setup_fleet( );
     player.print_player_layout( );*/

     srand(time(NULL));
     AI.generate_fleet(CARRIER_LENGTH, CARRIER_SYMBOL);
     AI.generate_fleet(BATTLESHIP_LENGTH, BATTLESHIP_SYMBOL);
     AI.generate_fleet(CRUISER_LENGTH, CRUISER_SYMBOL);
     AI.generate_fleet(SUBMARINE_LENGTH, SUBMARINE_SYMBOL);
     AI.generate_fleet(DESTROYER_LENGTH, DESTROYER_SYMBOL);
     AI.print_AI_layout( );

     do {
          int *coordinates = player.get_attack_coordinates( );
          counter1 = player.attack_and_update(AI.get_AI_position(*coordinates, *(coordinates + 1)), *coordinates, *(coordinates + 1));
          player.print_player_attacks( );
          // print player board with AI attacks
     } while (counter1 < HITS && counter2 < MAX_MOVES);

     player.free_carrier_spread_position( ); // return allocated memory to freestore
     player.free_battleship_spread_position( );
     player.free_cruiser_spread_position( );
     player.free_submarine_spread_position( );
     player.free_destroyer_spread_position( );

     return 0;
}
void print_game_info( )
{
     cout << endl
          << "**********************************************************************\n"
          << "* Welcome to Battleship, a classic two-player guessing game in which *\n"
          << "* the objective is to destroy your opponent's fleet. Because you     *\n"
          << "* are using this program, instead of another human being, to play    *\n"
          << "* Battleship, it most likely follows that you are a loner. Try to    *\n"
          << "* change that. Anyways, if you would like to see the instructions,   *\n"
          << "* press 'I.' If you are familiar with the game, proceed by pressing  *\n"
          << "* 'P.'                                                               *\n"
          << "**********************************************************************"
          << endl;
}
/*bool symbol_check(char symbol_ref)
{
     static int symbols[5], counter = 0;
     symbols[counter] = symbol_ref;

     for (int i = 0; i <= counter; i++)
     {
          if (i != counter && symbols[i] == symbol_ref)
          {
               cout << "\nSorry, that symbol has already been chosen for a previous ship.\n\n"
                    << "Please try another symbol: ";

               return true;
          }
     }

     counter++;
     return false;
}*/
void PlayerBoard::retrieve_layout(string ship_name, int ship_length)
{
     char symbol, orientation, fixed_position;
     int spread_position[ship_length];


          cout << "Choose the symbol that will mark the positions occupied by your " << ship_name << ": "; // retrieve ship symbol
          cin >> symbol;


     cout << endl << "Enter the orientation (H - horizontal, V - vertical) of your " << ship_name << ": "; // retrieve ship orientation
     cin >> orientation;

     if (orientation == 'H') // retrieve fixed position
          cout << endl << "Enter the number corresponding to the row along which your " << ship_name << " will lie: ";
     else
          cout << endl << "Enter the letter corresponding to the column along which your " << ship_name << " will lie: ";
     cin >> fixed_position;
// CHANGE THE BELOW TO PROMPT FOR A RANGE INSTEAD OF INDIVIDUAL NUMBERS
     if (orientation == 'H') // retrieve spread position
          cout << endl << "Enter the letters corresponding to the horizontally aligned positions on row " << fixed_position
               << "\nwhich your " << ship_name << " will take up: ";
     else
          cout << endl << "Enter the numbers corresponding to the vertically aligned positions on column " << fixed_position
               << "\nwhich your " << ship_name << " will take up: ";
     for (int i = 0; i < ship_length; i++) { cin >> spread_position[i]; }

     if (ship_name == "carrier") // read in obtained information
          set_carrier(orientation, symbol, fixed_position, spread_position);
     else if (ship_name == "battleship")
          set_battleship(orientation, symbol, fixed_position, spread_position);
     else if (ship_name == "cruiser")
          set_cruiser(orientation, symbol, fixed_position, spread_position);
     else if (ship_name == "submarine")
          set_submarine(orientation, symbol, fixed_position, spread_position);
     else
          set_destroyer(orientation, symbol, fixed_position, spread_position);
}
void PlayerBoard::setup_fleet( )
{
     if (this->carrier.orientation == 'H')
          for (int i = 0; i < CARRIER_LENGTH; i++)
               player_layout[this->carrier.fixed_position - 1][this->carrier.spread_position[i] - 1] = this->carrier.symbol;
     else
          for (int i = 0; i < CARRIER_LENGTH; i++)
               player_layout[this->carrier.spread_position[i] - 1][this->carrier.fixed_position - 1] = this->carrier.symbol;

     if (this->battleship.orientation == 'H')
          for (int i = 0; i < BATTLESHIP_LENGTH; i++)
               player_layout[this->battleship.fixed_position - 1][this->battleship.spread_position[i] - 1] = this->battleship.symbol;
     else
          for (int i = 0; i < BATTLESHIP_LENGTH; i++)
               player_layout[this->battleship.spread_position[i] - 1][this->battleship.fixed_position - 1] = this->battleship.symbol;

     if (this->cruiser.orientation == 'H')
          for (int i = 0; i < CRUISER_LENGTH; i++)
               player_layout[this->cruiser.fixed_position - 1][this->cruiser.spread_position[i] - 1] = this->cruiser.symbol;
     else
          for (int i = 0; i < CRUISER_LENGTH; i++)
               player_layout[this->cruiser.spread_position[i] - 1][this->cruiser.fixed_position - 1] = this->cruiser.symbol;

     if (this->submarine.orientation == 'H')
          for (int i = 0; i < SUBMARINE_LENGTH; i++)
               player_layout[this->submarine.fixed_position - 1][this->submarine.spread_position[i] - 1] = this->submarine.symbol;
     else
          for (int i = 0; i < SUBMARINE_LENGTH; i++)
               player_layout[this->submarine.spread_position[i] - 1][this->submarine.fixed_position - 1] = this->submarine.symbol;

     if (this->destroyer.orientation == 'H')
          for (int i = 0; i < DESTROYER_LENGTH; i++)
               player_layout[this->destroyer.fixed_position - 1][this->destroyer.spread_position[i] - 1] = this->destroyer.symbol;
     else
          for (int i = 0; i < DESTROYER_LENGTH; i++)
               player_layout[this->destroyer.spread_position[i] - 1][this->destroyer.fixed_position - 1] = this->destroyer.symbol;

}
void PlayerBoard::print_player_layout( )
{
     cout << endl << "    1  2  3  4  5  6  7  8  9  10" << endl;
     for (int i = 0; i < WIDTH; i++)
     {
          cout << " " << static_cast<char>(i + 65) << "  ";
          for (int j = 0; j < LENGTH; j++)
               cout << player_layout[i][j] << "  ";
          cout << endl;
     }
     cout << endl << endl;
}
/*void PlayerBoard::print_AI_layout( )
{
     cout << endl << "    A  B  C  D  E  F  G  H  I  J" << endl;
     for (int i = 0; i < WIDTH; i++)
     {
          if (i == 9)
               cout << (i + 1) << "  ";
          else
               cout << " " << (i + 1) << "  ";
          for (int j = 0; j < LENGTH; j++)
               cout << AI_layout[i][j] << "  ";
          cout << endl;
     }
     cout << endl << endl;
}*/
int* PlayerBoard::get_attack_coordinates( )
{
     // Prompt the user to enter a position on AI board that will be attacked
     // Determine success of attack, and report back to user. If attack is successful, update counter
     // This loop will be executed until all ships are sunk (17 hits)
     static int coordinates[2];

     /*cout << endl << "To determine who goes first, let's flip a coin.\n"
          << "Choose heads (H) or tails (T): ";*/
     // read in character and compare to randomly produced result of coin flip

     cout << endl << "Please choose a position to attack (e.g. C4): ";
     cin >> coordinates[0] >> coordinates[1];

     return coordinates;
}
int PlayerBoard::attack_and_update(char position, int row, int column)
{
     static int hits = 0;
     cout << endl << "Determining attack success...";
     sleep(2);
     if (position == CARRIER_SYMBOL || position == BATTLESHIP_SYMBOL
     || position == CRUISER_SYMBOL || position == SUBMARINE_SYMBOL
     || position == DESTROYER_SYMBOL)
     {
          cout << endl << "Status: HIT ON " << row << column << "!";
          player_attacks[row - 1][column - 1] = 'H';
          return ++hits;
     }
     else
     {
          cout << endl << "Status: MISS ON " << row << column << ".";
          player_attacks[row - 1][column - 1] = 'M';
          return hits;
     }
}
void PlayerBoard::print_player_attacks( )
{
     cout << endl << "Updating board...";
     sleep(2);
     cout << endl << "    1  2  3  4  5  6  7  8  9  10" << endl;
     for (int i = 0; i < WIDTH; i++)
     {
          cout << " " << static_cast<char>(i + 65) << "  ";
          for (int j = 0; j < LENGTH; j++)
               cout << player_attacks[i][j] << "  ";
          cout << endl;
     }
     cout << endl << endl;
}
void print_empty_board( )
{
     cout << endl << "    1  2  3  4  5  6  7  8  9  10" << endl;
     for (int i = 0; i < WIDTH; i++)
     {
          cout << " " << static_cast<char>(i + 65) << "  ";
          for (int j = 0; j < LENGTH; j++)
               cout << "_  ";
          cout << endl;
     }
     cout << endl << endl
          << "Above is the board on which you will place your fleet. You will now\n"
          << "establish the positions of your individual ships.\n\n"
          << "**********************************************************************"
          << endl << endl;
}
PlayerBoard::PlayerBoard( )
{
     for (int i = 0; i < WIDTH; i++)
          for (int j = 0; j < LENGTH; j++)
          {
               player_layout[i][j] = '_';
               AI_layout[i][j] = '_';
               player_attacks[i][j] = '_';
          }

     this->carrier.spread_position = new int [CARRIER_LENGTH];
     for (int i = 0; i < CARRIER_LENGTH; i++) { this->carrier.spread_position[i] = 0; }

     this->battleship.spread_position = new int [BATTLESHIP_LENGTH];
     for (int i = 0; i < BATTLESHIP_LENGTH; i++) { this->battleship.spread_position[i] = 0; }

     this->cruiser.spread_position = new int [CRUISER_LENGTH];
     for (int i = 0; i < CRUISER_LENGTH; i++) { this->cruiser.spread_position[i] = 0; }

     this->submarine.spread_position = new int [SUBMARINE_LENGTH];
     for (int i = 0; i < SUBMARINE_LENGTH; i++) { this->submarine.spread_position[i] = 0; }

     this->destroyer.spread_position = new int [DESTROYER_LENGTH];
     for (int i = 0; i < DESTROYER_LENGTH; i++) { this->destroyer.spread_position[i] = 0; }
}
void AIBoard::generate_fleet(int ship_length, ship_symbol_t symbol)
{
     int temp_row, temp_column;
     bool invalid_spots = true;
     char potential_spots[WIDTH][LENGTH];
     temp_row = randomly_assign( ); // randomly assign a placement position
     temp_column = randomly_assign( );

     if (static_cast<double>(rand( )) < static_cast<double>(RAND_MAX)/2) // randomly determine whether to place horizontal or vertical
     { // vertical
          if (temp_row + ship_length > LENGTH) // count downwards if starting row is too high
          {
               if (availability(temp_row, temp_column, VERTICAL, ship_length) >= ship_length) // determine whether or not file is available
               {
                    for (int i = 0; i < ship_length; i++) // go through ship_length positions
                    {
                         if (availability(temp_row - i, temp_column)) // if spot is available, mark it
                              potential_spots[temp_row - i][temp_column] = symbol;
                         else // if there are insufficiently many contiguous spots, terminate loop
                         {
                              invalid_spots = false;
                              break;
                         }
                    }
                    if (invalid_spots)
                         for (int i = 0; i < ship_length; i++)
                              AI_layout[temp_row - i][temp_column] = potential_spots[temp_row - i][temp_column];
                    else // call function to find suitable placement positions
                    {
                         //cout << endl << "TEST1" << endl;
                         generate_fleet(ship_length, symbol);
                    }
               }
               else
               {
                    generate_fleet(ship_length, symbol); //cout << endl << "TEST2" << endl;
               }
          }
          else // count upwards if starting row is too low
          {
               if (availability(temp_row, temp_column, HORIZONTAL, ship_length) >= ship_length)
               {
                    for (int i = 0; i < ship_length; i++)
                    {
                         if (availability(temp_row + i, temp_column))
                              potential_spots[temp_row + i][temp_column] = symbol;
                         else
                         {
                              invalid_spots = false;
                              break;
                         }
                    }
                    if (invalid_spots)
                         for (int i = 0; i < ship_length; i++)
                              AI_layout[temp_row + i][temp_column] = potential_spots[temp_row + i][temp_column];
                    else // call function to find suitable placement positions
                    {
                         //cout << endl << "TEST3" << endl;
                         generate_fleet(ship_length, symbol);
                    }
               }
               else
               {
                         generate_fleet(ship_length, symbol); //cout << endl << "TEST2" << endl;
               }
          }
     }
     else // horizontal
     {
          if (temp_column + ship_length > LENGTH) // count left if starting column is too high
          {
               if (availability(temp_row, temp_column, VERTICAL, ship_length) >= ship_length)
               {
                    for (int i = 0; i < ship_length; i++)
                    {
                         if (availability(temp_row, temp_column - i))
                              potential_spots[temp_row][temp_column - i] = symbol;
                         else
                         {
                              invalid_spots = false;
                              break;
                         }
                    }
                    if (invalid_spots)
                         for (int i = 0; i < ship_length; i++)
                              AI_layout[temp_row][temp_column - i] = potential_spots[temp_row][temp_column - i];
                    else // call function to find suitable placement positions
                    {
                         //cout << endl << "TEST5" << endl;
                         generate_fleet(ship_length, symbol);
                    }
               }
               else
               {
                    generate_fleet(ship_length, symbol); //cout << endl << "TEST2" << endl;
               }
          }
          else // count right if starting column is too low
          {
               if (availability(temp_row, temp_column, HORIZONTAL, ship_length) >= ship_length)
               {
                    for (int i = 0; i < ship_length; i++)
                    {
                         if (availability(temp_row, temp_column + i))
                              potential_spots[temp_row][temp_column + i] = symbol;
                         else
                         {
                              invalid_spots = false;
                              break;
                         }
                    }
                    if (invalid_spots)
                         for (int i = 0; i < ship_length; i++)
                              AI_layout[temp_row][temp_column + i] = potential_spots[temp_row][temp_column + i];
                    else // call function to find suitable placement positions
                    {
                         //cout << endl << "TEST7" << endl;
                         generate_fleet(ship_length, symbol);
                    }
               }
               else
               {
                    generate_fleet(ship_length, symbol); //cout << endl << "TEST2" << endl;
               }
          }
     }
}
bool AIBoard::availability(int row, int column)
{
     if (AI_layout[row][column] == CARRIER_SYMBOL || AI_layout[row][column] == BATTLESHIP_SYMBOL
     || AI_layout[row][column] == CRUISER_SYMBOL || AI_layout[row][column] == SUBMARINE_SYMBOL ||
     AI_layout[row][column] == DESTROYER_SYMBOL)
          return false;
     else
          return true;
}
int AIBoard::availability(int row, int column, orientation_t orientation, int ship_length)
{
     int counter = 0;
     if (orientation == VERTICAL) // move upwards for vertical and sideways for horizontal
     {//vertical
          for (int i = 0; i < LENGTH; i++)
          {
               if (i == 0)
               {
                    if (availability(i, column))
                    {
                         counter++;
                         continue;
                    }
                    else
                         continue;
               }
               else
               {
                    if (availability(i, column) && availability(i - 1, column))
                    {
                         counter++;;
                         continue;
                    }
                    else
                         continue;
               }
          }
     }
     else // orientation = HORIZONTAL
     {
          for (int i = 0; i < WIDTH; i++)
          {
               if (i == 0)
               {
                    if (availability(row, i))
                    {
                         counter++;
                         continue;
                    }
                    else
                         continue;
               }
               else
               {
                    if (availability(row, i) && availability(row, i - 1))
                    {
                         counter++;
                         continue;
                    }
                    else
                         continue;
               }
          }
     }

     return counter;
}
AIBoard::AIBoard( )
{
     for (int i = 0; i < WIDTH; i++)
          for (int j = 0; j < LENGTH; j++)
          {
               player_layout[i][j] = '_';
               AI_layout[i][j] = '_';
          }
}
void AIBoard::print_AI_layout( )
{
     cout << endl << "    1  2  3  4  5  6  7  8  9  10" << endl;
     for (int i = 0; i < WIDTH; i++)
     {
          cout << " " << static_cast<char>(i + 65) << "  ";
          for (int j = 0; j < LENGTH; j++)
               cout << AI_layout[i][j] << "  ";
          cout << endl;
     }
     cout << endl << endl;
}
