#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <set>
#include "crypt.h"

const int MAX_PLAYERS = 11;
using namespace std;
using lli = long long int;
struct card {
    int number = -1;
    int suit = -1;
    int name = -1;
    string suitStr;
    string nameStr;
};
struct player {
    int number;
    lli C, D;//Keys
    lli hand[2];
};

player playerInit(lli P) {
    player temp{};
    temp.C = genMutPrime(P - 1);
    temp.D = invEv(temp.C, P - 1);

    return temp;
}

card readCard(lli input) {
    card temp;
    if (input <= 0) {
        return {};
    }
    input -= 2;
    temp.number = input;
    temp.suit = input % 4;
    switch (temp.suit) {
        case 0:
            temp.suitStr = "Diamonds";
            break;
        case 1:
            temp.suitStr = "Hearts";
            break;
        case 2:
            temp.suitStr = "Clubs";
            break;
        case 3:
            temp.suitStr = "Spades";
            break;
    }
    temp.name = input % 13;
    switch (temp.name) {
        case 0:
            temp.nameStr = "Ace";
            break;
        case 1:
            temp.nameStr = "2";
            break;
        case 2:
            temp.nameStr = "3";
            break;
        case 3:
            temp.nameStr = "4";
            break;
        case 4:
            temp.nameStr = "5";
            break;
        case 5:
            temp.nameStr = "6";
            break;
        case 6:
            temp.nameStr = "7";
            break;
        case 7:
            temp.nameStr = "8";
            break;
        case 8:
            temp.nameStr = "9";
            break;
        case 9:
            temp.nameStr = "10";
            break;
        case 10:
            temp.nameStr = "Jack";
            break;
        case 11:
            temp.nameStr = "Queen";
            break;
        case 12:
            temp.nameStr = "King";
            break;

    }
    return temp;
}

vector<lli> shuffleCards(player pl, vector<lli> cards, lli P) {    auto rng = default_random_engine {};
    shuffle(begin(cards), std::end(cards), rng);
    vector<lli> shCards(cards.size());
    for (int i = 0; i < cards.size(); i++) {
        shCards.at(i) = modPow(cards.at(i), pl.C, P);

    }
    return shCards;
}

vector<lli> deshuffleCards(player pl, vector<lli> cards, lli P) {

    vector<lli> shCards(cards.size());
    for (int i = 0; i < cards.size(); i++) {
        shCards.at(i) = modPow(cards.at(i), pl.D, P);

    }
    return shCards;
}

int playerCount() {
    int t = 90;

    while (t >= MAX_PLAYERS || t <= 0) {
        cout << "Enter number of players(0<count<11)\n:";
        cin >> t;
    }
    return t;
}

card readEncCard(vector<player> deck, player pl, lli encCard, lli P) {
    lli temp = encCard;
    for (auto &i: deck) {
        if (i.number != pl.number) {
            temp = modPow(temp, i.D, P);

        }
    }
    temp = modPow(temp, pl.D, P);
    return readCard(temp);
}

int main() {

    vector<lli> duplCheck={};
    srand(time(NULL));

    vector<lli> cards(52);
    for (int i = 0; i < 52; i++) {
        cards.at(i) = i + 2;
        //1   cout<<readCard(i).suitStr<<" "<<readCard(i).nameStr<<"\n";
    }
    lli P = genPrime(INT16_MAX, INT32_MAX);
    nlPrint(P);
    int count = playerCount();
    cout << count;
    vector<player> deck(count);
    for (int i = 0; i < count; i++) {
        deck.at(i) = (playerInit(P));
        deck.at(i).number = i;
    }
    for (auto &i: deck) {
        cards = shuffleCards(i, cards, P);
    }
    nlPrint("");
    for (auto &i: cards) {
        cout << i << " ";

    }
    for (auto &i: deck) {
        for (int j = 0; j < 2; j++) {
            bool grabbed = false;
            while (!grabbed) {
                int grab = rand() % cards.size();
                if (cards.at(grab) > 0) {
                    duplCheck.push_back(cards.at(grab));
                    i.hand[j] = cards.at(grab);
                    cards.at(grab) *= -1;
                    grabbed = true;
                }
            }

        }


    }
    nlPrint("Cards" + to_string(cards.size()));
    nlPrint("Players");
    for (auto &i: deck) {
        nlPrint(readEncCard(deck, i, i.hand[0], P).nameStr + " " + readEncCard(deck, i, i.hand[0], P).suitStr+"    "+readEncCard(deck, i, i.hand[1], P).nameStr + " " + readEncCard(deck, i, i.hand[1], P).suitStr);
        cout<<"______________________________________________________________________________________";
        //nlPrint();
        // nlPrint(readEncCard(deck,i,i.hand[1],P).number);
        //   nlPrint(readEncCard(deck,i,i.hand[0],P).number);

    }
    nlPrint("On Deck Cards________________________________________");
    vector<lli> onDeckCards={};

        for (int j = 0; j < 5; j++) {
            bool grabbed = false;
            while (!grabbed) {
                int grab = rand() % cards.size();
                if (cards.at(grab) > 0) {
                    duplCheck.push_back(cards.at(grab));
                     onDeckCards.push_back(cards.at(grab));
                    cards.at(grab) *= -1;
                    grabbed = true;
                }
            }



    }

        onDeckCards= deshuffleCards(deck.at(0),onDeckCards,P);
    for (auto &i: onDeckCards) {
            cout<<"|"<<readCard(i).nameStr + " " + readCard(i).suitStr<<"|  ";
    }
    ////ПРОВЕРКА НА КОРРЕКТНОСТЬ ВЫБОРКИ И УДАЛЕНИЯ
    duplCheck.insert(duplCheck.end(),cards.begin(),cards.end());
    duplCheck= deshuffleCards(deck.at(0),duplCheck,P);
//    nlPrint("Remaining cards"); q
    duplCheck.erase(std::remove_if(
            duplCheck.begin(),duplCheck.end(),
            [](const lli& x) {
                return x < 0; // put your condition here
            }), duplCheck.end());
//    for (auto &i: duplCheck) {
//        if (i > 0)
//            nlPrint(readCard(i).nameStr + " " + readCard(i).suitStr);
//    }
    set<int> s(duplCheck.begin(), duplCheck.end());
    nlPrint("Cards in use "+ to_string(duplCheck.size()));
    nlPrint("Unique cards on deck  "+ to_string(duplCheck.size()));
    return 0;
}
