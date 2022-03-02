#include "Character.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

//Function for ordering a Character vector.

vector<Character> alphabeth(vector<Character> old){
    vector<string> nameOrdered;
    for(int i = 0; i < 5; i++){
        nameOrdered.push_back(old.at(i).name);
    }
    sort(nameOrdered.begin(), nameOrdered.end());
    vector<Character> ordered;

    for(int i = 0; i < 5; i++){
        for(int k = 0; k < 5; k++){
            if(old.at(k).name == nameOrdered.at(i)){
                ordered.push_back(old.at(k));
                break;
            }
        }
    }

    return ordered;
}

//Function for processing an attack.

void attackProcess(Character &attacker, Character &defender, bool ifSpecial){
    int damage = attacker.attack - defender.defense;
    if(ifSpecial && attacker.type == "Dwarfs" && attacker.nRoundsSinceSpecial > 20){
        damage *= 2;
        attacker.nRoundsSinceSpecial = 0;
    }
    if(damage > 0){
        defender.remainingHealth -= damage;
        if(defender.remainingHealth <= 0){
            defender.remainingHealth = 0;
            defender.isAlive = false;
        }
    }
}

//Main function

int main(int argc, char* argv[]) {

    ifstream input (argv[1]);
    ofstream output (argv[2]);

    vector<Character> old1;
    vector<Character> old2;

    vector<Character> com1;
    vector<Character> com2;

    if (input.is_open())
    {
        string r;
        input >> r;
        int round = stoi(r);

        //Creating the first communities vector.

        for(int i = 0; i < 5; i++){
            string name, type, a, d, initial;
            input >> name >> type >> a >> d >> initial;
            int attack = stoi(a);
            int defense = stoi(d);
            int ihp = stoi(initial);

            old1.push_back(Character(name, type, attack, defense, ihp, round));
        }

        com1 = alphabeth(old1);

        //Creating the second communities vector

        for(int i = 0; i < 5; i++){
            string name, type, a, d, initial;
            input >> name >> type >> a >> d >> initial;
            int attack = stoi(a);
            int defense = stoi(d);
            int ihp = stoi(initial);

            old2.push_back(Character(name, type, attack, defense, ihp, round));
        }

        com2 = alphabeth(old2);

        //Some variables for deciding and keeping some situations during rounds

        bool warEnded = false;
        int roundLast;
        int numOfCasuality = 0;
        string winner = "Draw";
        string lastDead1 = " ";
        string lastDead2 = " ";

        //Round operations

        for(int i = 1; i <= round; i++){
            if(warEnded){
                break;
            }

            //Some variables for deciding and keeping some situations during attacks

            bool ifSpecial = false;
            string aName, dName, skill;
            input >> aName >> dName >> skill;

            if(skill == "SPECIAL"){
                ifSpecial = true;
            }

            //Attack operations


            if(i % 2 == 1){

                //If odd rounds means community 1 attacks

                for(int a = 0; a < 5; a++){
                    if(com1.at(a).name == aName){

                        //Deciding and checking who attacker is

                        if(!com1.at(a).isAlive){
                            bool ch = false;
                            for(int x = a + 1; x < 5; x++){
                                if(com1.at(x).isAlive){
                                    a = x;
                                    ch = true;
                                    break;
                                }
                            }
                            if(!ch){
                                for(int x = a - 1; x >=0; x--){
                                    if(com1.at(x).isAlive){
                                        a = x;
                                        ch = true;
                                        break;
                                    }
                                }
                            }
                        }

                        //End of deciding attacker

                        for(int d = 0; d < 5; d++){
                            if(com2.at(d).name == dName){

                                //Deciding and checking who defender is

                                if(!com2.at(d).isAlive){
                                    bool ch = false;
                                    for(int x = d + 1; x < 5; x++){
                                        if(com2.at(x).isAlive){
                                            d = x;
                                            ch = true;
                                            break;
                                        }
                                    }
                                    if(!ch){
                                        for(int x = d - 1; x >=0; x--){
                                            if(com2.at(x).isAlive){
                                                d = x;
                                                ch = true;
                                                break;
                                            }
                                        }
                                    }
                                }

                                //End of deciding defender

                                attackProcess(com1.at(a), com2.at(d), ifSpecial);

                                //Checking wizard special

                                if(ifSpecial && com1.at(a).type == "Wizards" && com1.at(a).nRoundsSinceSpecial > 50 && lastDead1 != " "){
                                    for(int ld = 0; ld < 5; ld++){
                                        if(com1.at(ld).name == lastDead1){
                                            if(!com1.at(ld).isAlive){
                                                com1.at(ld).isAlive = true;
                                                com1.at(ld).remainingHealth = com1.at(ld).healthHistory[0];
                                                com1.at(ld).healthHistory[i] = com1.at(ld).remainingHealth;
                                                com1.at(ld).nRoundsSinceSpecial = 0;
                                                numOfCasuality--;
                                            } else {
                                                com1.at(ld).remainingHealth = com1.at(ld).healthHistory[0];
                                                com1.at(ld).healthHistory[i] = com1.at(ld).remainingHealth;
                                            }
                                            com1.at(a).nRoundsSinceSpecial = 0;
                                            break;
                                        }
                                    }
                                }

                                //Checking Elvin special

                                if(ifSpecial && com1.at(a).type == "Elves" && com1.at(a).nRoundsSinceSpecial > 10){
                                    for(int hob = 0; hob < 5; hob++){
                                        if(com1.at(hob).type == "Hobbit"){
                                            int transferred = com1.at(a).remainingHealth / 2;
                                            com1.at(a).remainingHealth -= transferred;
                                            com1.at(hob).remainingHealth += transferred;
                                            com1.at(a).nRoundsSinceSpecial = 0;
                                        }
                                    }
                                }

                                //If defender dies

                                if(!com2.at(d).isAlive){
                                    lastDead2 = com2.at(d).name;
                                    numOfCasuality++;
                                }
                                break;
                            }
                        }
                        break;
                    }
                }
            } else {

                //even rounds mean community 2 attacks

                for(int a = 0; a < 5; a++){
                    if(com2.at(a).name == aName){

                        //Deciding and checking who attacker is

                        if(!com2.at(a).isAlive){
                            bool ch = false;
                            for(int x = a + 1; x < 5; x++){
                                if(com2.at(x).isAlive){
                                    a = x;
                                    ch = true;
                                    break;
                                }
                            }
                            if(!ch){
                                for(int x = a - 1; x >=0; x--){
                                    if(com2.at(x).isAlive){
                                        a = x;
                                        ch = true;
                                        break;
                                    }
                                }
                            }
                        }

                        //End of defender

                        for(int d = 0; d < 5; d++){
                            if(com1.at(d).name == dName){

                                //Deciding and checking who defender is

                                if(!com1.at(d).isAlive){
                                    bool ch = false;
                                    for(int x = d + 1; x < 5; x++){
                                        if(com1.at(x).isAlive){
                                            d = x;
                                            ch = true;
                                            break;
                                        }
                                    }
                                    if(!ch){
                                        for(int x = d - 1; x >=0; x--){
                                            if(com1.at(x).isAlive){
                                                d = x;
                                                ch = true;
                                                break;
                                            }
                                        }
                                    }
                                }

                                //End of deciding

                                attackProcess(com2.at(a), com1.at(d), ifSpecial);

                                //Checking wizard special

                                if(ifSpecial && com2.at(a).type == "Wizards" && com2.at(a).nRoundsSinceSpecial > 50 && lastDead2 != " "){
                                    for(int ld = 0; ld < 5; ld++){
                                        if(com2.at(ld).name == lastDead2){
                                            if(!com2.at(ld).isAlive){
                                                com2.at(ld).isAlive = true;
                                                com2.at(ld).remainingHealth = com2.at(ld).healthHistory[0];
                                                com2.at(ld).healthHistory[i] = com2.at(ld).remainingHealth;
                                                com2.at(ld).nRoundsSinceSpecial = 0;
                                                numOfCasuality--;
                                            } else {
                                                com2.at(ld).remainingHealth = com2.at(ld).healthHistory[0];
                                                com2.at(ld).healthHistory[i] = com2.at(ld).remainingHealth;
                                            }
                                            com2.at(a).nRoundsSinceSpecial = 0;
                                            break;
                                        }
                                    }
                                }

                                //Checking Elvin special

                                if(ifSpecial && com2.at(a).type == "Elves" && com2.at(a).nRoundsSinceSpecial > 10){
                                    for(int hob = 0; hob < 5; hob++){
                                        if(com2.at(hob).type == "Hobbit"){
                                            int transferred = com2.at(a).remainingHealth / 2;
                                            com2.at(a).remainingHealth -= transferred;
                                            com2.at(hob).remainingHealth += transferred;
                                            com2.at(a).nRoundsSinceSpecial = 0;
                                        }
                                    }
                                }

                                //Checking if defender dies

                                if(!com1.at(d).isAlive){
                                    lastDead1 = com1.at(d).name;
                                    numOfCasuality++;
                                }
                                break;
                            }
                        }
                        break;
                    }
                }
            }

            roundLast = i;

            //Checking some continuity conditions and updating characters for community 1

            for(int k = 0; k < 5; k++){
                com1.at(k).healthHistory[i] = com1.at(k).remainingHealth;

                if(com1.at(k).type == "Hobbit" && com1.at(k).isAlive == false){
                    warEnded = true;
                    winner = "Community-2";
                }
                 com1.at(k).nRoundsSinceSpecial++;
            }

            bool allDead1 = true;
            for(int k = 0; k < 5; k++){
                if(com1.at(k).type != "Hobbit" && com1.at(k).isAlive == true){
                    allDead1 = false;
                }
            }
            if(allDead1){
                warEnded = true;
                winner = "Community-2";
            }

            //Checking some continuity conditions and updating characters for community 2

            for(int k = 0; k < 5; k++){
                com2.at(k).healthHistory[i] = com2.at(k).remainingHealth;

                if(com2.at(k).type == "Hobbit" && com2.at(k).isAlive == false){
                    warEnded = true;
                    winner = "Community-1";
                }
                com2.at(k).nRoundsSinceSpecial++;
            }

            bool allDead2 = true;
            for(int k = 0; k < 5; k++){
                if(com2.at(k).type != "Hobbit" && com2.at(k).isAlive == true){
                    allDead2 = false;
                }
            }
            if(allDead2){
                warEnded = true;
                winner = "Community-1";
            }

            //If conditions are not met then finish the war

            if(warEnded){
                break;
            }

        }

        //Printing the results

        output << winner << endl << roundLast << endl << numOfCasuality << endl;

        //Printing the health records of community 1

        for(int i = 0; i < 5; i++){
            for(int k = 0; k < 5; k++){
                if(old1.at(i).name == com1.at(k).name){
                    output << com1.at(k).name << " ";
                    for(int m = 0; m < roundLast + 1; m++){
                        output << com1.at(k).healthHistory[m] << " ";
                    }
                    output << endl;
                }
            }
        }

        //Printing the health records of community 2

        for(int i = 0; i < 5; i++){
            for(int k = 0; k < 5; k++){
                if(old2.at(i).name == com2.at(k).name){
                    output << com2.at(k).name << " ";
                    for(int m = 0; m < roundLast + 1; m++){
                        output << com2.at(k).healthHistory[m] << " ";
                    }
                    output << endl;
                }
            }
        }
    }
    else cout << "Unable to open file";

    input.close();
    output.close();

    return 0;
}

