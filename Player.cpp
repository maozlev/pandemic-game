#include "Player.hpp"


#include <map>
#include <vector>
#include <iostream>
#include <stdexcept>


using namespace std;

namespace pandemic{

    Player::Player(Board board, City city, string roles){
        string st = roles;
        std::vector <City> cards_of_playerr;
        this->current_board = board;
        this->currennt_city = city;
        this->cards_of_player = cards_of_playerr;
        // this->role = roles;
    }

    Player& Player::drive(City city){
        string st = ToString(city);
        if(this->current_board.is_neighbors(this->currennt_city, city)){
            this->currennt_city = city;
            cout<<"moving to: "+st<<endl;
            return *this;
        }
        throw std::logic_error{st+" is not connected"};
    }

    Player& Player::fly_direct(City city){
        string st = ToString(city);
        if(this->cards_of_player.size() == 0){
            throw std::invalid_argument{"you dont have any cards"};
        }
        unsigned int t = 0;
        while(t < this->cards_of_player.size()){
            if(this->cards_of_player.at(t) == city){
                this->currennt_city = city;
                this->cards_of_player.erase(this->cards_of_player.begin() + t);
                this->current_board.cards.at(city) = false;
                cout<<"fly to: "+st<<endl;
                cout<<"drop: "+st<<endl;
                return *this;
            }
            t++;
        } 
        throw std::invalid_argument{"you dont have card of this city: "+st};
    }

    Player& Player::fly_charter(City city){
        City need_to_drop = this->currennt_city;
        string st = ToString(city);
        string st1 = ToString(need_to_drop);
        unsigned int t = 0;
        if(this->cards_of_player.size() == 0){
            throw std::invalid_argument{"you dont have any cards"};
        }
        while(t < this->cards_of_player.size()){
            if(this->cards_of_player.at(t) == need_to_drop){
                this->currennt_city = city;
                this->cards_of_player.erase(this->cards_of_player.begin() + t);
                this->current_board.cards.at(need_to_drop) = false;
                cout<<"fly to: "+st<<endl;
                cout<<"drop: "+st1<<endl;
                return *this;
            }
            t++;
        } 
        throw std::invalid_argument{"you dont have card of this city"};
    }

    Player& Player::fly_shuttle(City city){
        string st = ToString(city);
        string st1 = ToString(this->currennt_city);
        if(this->current_board.research_lab.at(city) &&
           this->current_board.research_lab.at(this->currennt_city)){
               cout<<"fly from: "<<st1<<"to "<<st<<endl;
               this->currennt_city = city;
               return *this;
        }
        if(!(this->current_board.research_lab.at(city))){
            throw std::invalid_argument{st+" dont have research lab"};     
        }
        if(!(this->current_board.research_lab.at(this->currennt_city))){
            throw std::invalid_argument{st1+" dont have research lab"};     
        }
        return *this;
    }

    Player& Player::build(){
        City city = this->currennt_city;
        string st = ToString(city);
        if(this->current_board.research_lab.at(city) == true){
            cout<<"you already have a lab in the city: "<<st<<endl;
            return *this;
        }
        if(this->cards_of_player.size() == 0){
            throw std::invalid_argument{"you dont have any cards"};
            return *this;
        }
        unsigned int t = 0;
        while(t < this->cards_of_player.size()){
            if(this->cards_of_player.at(t) == city){
                this->current_board.research_lab.at(city) = true;
                this->current_board.cards.at(city) = false;
                this->cards_of_player.erase(this->cards_of_player.begin() + t);
                cout<<"built lab at: "+st<<endl;
                cout<<"drop: "+st<<endl;
                return *this;
            }
            t++;
        } 
        throw std::invalid_argument{"you dont have card of this city: "+st};
    }

    Player& Player::discover_cure(Color color){
        string st = ToString(this->currennt_city);
        string c = ToString(color);
        if(this->current_board.cure.at(color) == false){
            if(this->current_board.research_lab.at(this->currennt_city) == false){
                throw logic_error{st+" have no lab!"};
            }
            int num_of_color_card = get_num_of_color(color);
            if(num_of_color_card<5){
                throw logic_error{"you not have enoght "+c+" cards"};
            }
            int sum=0;
            for (unsigned int i = 0; i < this->cards_of_player.size(); i++){
                if(sum == 5){
                    break;
                }
                if(this->current_board.colors.at(this->cards_of_player.at(i)) == color){
                    this->cards_of_player.erase(this->cards_of_player.begin() + i);
                    sum++;
                }
            }
            this->current_board.cure.at(color) = true;
            cout<<"drop: "<<sum<<" cards"<<endl;
        }
        cout<<"discoverd: "<<c<<" cure!"<<endl;
        return *this;
    }

    Player& Player::treat(City city){
        string st = ToString(city);
        if(this->currennt_city == city){
            if(this->current_board.infection_level.at(city) == 0){
                throw logic_error{st+" is already clear"};
            }
            if(this->current_board.research_lab.at(city) == true){
                this->current_board.infection_level.at(city) = 0;
                cout<<"update infection level: "<<
                this->current_board.infection_level.at(city)<<
                " at: "<<st<<endl;
                cout<<st<<" is clear"<<endl;
                return *this;
            }
            this->current_board.infection_level.at(city) -= 1;
            cout<<"update infection level: "<<
            this->current_board.infection_level.at(city)<<
            " at: "<<st<<endl;
            return *this;
        }
        cout<<"go to the city: "<<st<<endl;
        return *this;
    }


    Player& Player::take_card(City city){
        string st = ToString(city);
        if(this->current_board.card_in_game(city)){
            cout<<st+" in the game already"<<endl;
            return *this;
        }
        this->cards_of_player.insert(cards_of_player.begin(), 1, city);
        this->current_board.cards.at(city) = true;
        cout<<"take card: "<<st<<endl;
        return *this;
    }

    int Player::get_num_of_color(Color color){
        int sum = 0;
        for (size_t i = 0; i < this->cards_of_player.size(); i++){
            if(this->current_board.colors.at(this->cards_of_player.at(i)) == color){
                sum++;
            }
        }
            return sum;
    }
};
       