#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

//Cole Bradley
//Student ID: 260891176

//Creates enums for the nunmbers in the cards and for the suit of the card
enum Rank {ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK = 'J', QUEEN = 'Q', KING = 'K'};
enum Type {CLUBS = 'C', DIAMONDS = 'D', HEARTS = 'H', SPADES = 'S'};
    

//Card Class
class Card{
    public:

    //Card Constructor that takes in a Rank and a Type
    Card(Rank num, Type type):value(num), suit(type){}
    //Get value function
    //As jack, queen, king are chars, we make sure to return their numerical value using a case statement
    //We return aces as 1 always as the check for when the should be 11 is in the class hand
    int getValue(){
        switch(value){
            case JACK:
                return 10;
            case QUEEN:
                return 10;
            case KING:
                return 10;
            default:
                return value;
        }
    }
    //Converts the value and the suit to strings and returns the string
    string displayCard(){
        string cardValue;
        switch(value){
            case JACK:
                cardValue = 'J';
                break;
            case QUEEN:
                cardValue = 'Q';
                break;
            case KING:
                cardValue = 'K';
                break;
            default:
                cardValue = to_string(value);
        }
        char cardType = suit;
        string card = cardValue + cardType;
        return card;
    }

    protected:
    Rank value;
    Type suit;
};


//Hand Class
class Hand{
    public:

    //Adds a card to the end of our hand as our hand is a vector
    void add(Card addedCard){
        hand.push_back(addedCard);
    }
    //Empties the hand
    void clear(){
        hand.clear();
    }
    //Gets the total of the cards
    //Counts how many aces are in the hand
    int getTotal(){
        int total = 0;
        int numAces = 0;
        for (Card &element: hand){
            total += element.getValue();
            if (element.getValue() == 1){
                numAces += 1;
            }
        }
        //Makes the necessary amount of aces 1's 
        for (int i = 0; i < numAces; i++){
            if (21 - total >= 10){
                total += 10;
            }
        }
        return total;
    }

    vector<Card> hand;

};

//Deck Class
class Deck{
    public:
    //acts as a constructor for the deck
    //Iterates through the two enums and adds every possible card in a 52 card deck to the deck
    void Populate(){
        int i;
        int j;
        for (int i = 1; i <= 4; i++){
            for (j = ACE; j <= 13; j++){
                Type place;
                Rank value;
                switch (j){
                    case 11:
                        value = Rank('J');
                        break;
                    case 12:
                        value = Rank('Q');
                        break;
                    case 13:
                        value = Rank('K');
                        break;
                    default:
                        value = Rank(j);
                }
                switch (i){
                    case 1:
                        place = CLUBS;
                        break;
                    case 2:
                        place = DIAMONDS;
                        break;
                    case 3:
                        place = HEARTS;
                        break;
                    case 4:
                        place = SPADES;
                        break;
                }
                Card card = Card(value,place);
                deck.push_back(card);
            }
        }
    }

    //Uses the algorithm header to shuffle the vector a random number of times
    void shuffle(){
        int numShuffles = rand() % 100;
        for (int i = 0; i < numShuffles; i ++){
            random_shuffle(deck.begin(), deck.end());
        }
    }

    //Below removes the last element in the deck and adds it to the hand
    Card deal(){
        Card temp = deck.back();
        deck.pop_back();
        return temp;
    }

    vector<Card> deck;
};

//Abstract Player Class to be inherited
class AbstractPlayer{
    public:
    
    //Creates a pure virtual method
    virtual bool isDrawing() = 0;
    
    //Checks if the total is over 21
    bool isBusted(){
        if (hand.getTotal() > 21)
            return true;
        else
            return false;
    }
    
    Hand hand;
};



//Computer Player class
class ComputerPlayer: public AbstractPlayer{
    public:

    //Checks if the computer players hand is less than 16 and draws 
    ComputerPlayer():numWins(1) , numLD(1){}
    
    bool isDrawing(){
        if (hand.getTotal() <= 16)
            return true;
        else 
            return false;
    }

    //To keep track of if who is winning
    float numWins;
    float numLD;
    //Must add logic for if the computer is losing more than 45% of games
};

//Human Player class
class HumanPlayer: public AbstractPlayer{
    public:

    //Asks the player if the want to draw or not
    bool isDrawing() {
        char answer;
        cout << "Do you want to draw? (y/n):";
        cin >> answer;
        return answer == 'y' ? true : false;
    }

    //Checks if the casino or the player had a draw or who won
    void announce(Hand computerHand){
        int casinoTotal = computerHand.getTotal();
        int playerTotal = hand.getTotal();
        if (playerTotal > casinoTotal)
            cout << "Player wins.\n\n";
        
        else if (playerTotal == casinoTotal)
            cout << "Push: No one wins.\n\n";
        
        else
            cout << "Casino wins.\n\n";
    }

};

//Blackjack game class, main class that play the game
class BlackJackGame{

    public:
    
    void play(){
        //Initializes our player, populates and shuffles a deck
        HumanPlayer player;
        m_deck.Populate();
        m_deck.shuffle();
        m_casino.hand.add(m_deck.deal());
        //Booleans to make sure the game ends immediately when someone busts
        
        //Displays the first card dealt to the casino
        cout << "Casino: ";
        for (Card & card: m_casino.hand.hand){
            cout << card.displayCard() << " ";
        }
        cout << "[" << m_casino.hand.getTotal() << "]\n";

        //Deals the first two cards to the player
        for (int i = 0; i < 2; i ++){
            player.hand.add(m_deck.deal());
        }

        //Displays the players card data
        cout << "Player: ";
        for (Card & card: player.hand.hand){
            cout << card.displayCard() << " ";
        }
        cout << "[" << player.hand.getTotal() << "]\n";

        //Continues to ask and deals card to the player if they want more cards dealt
        while(player.isDrawing()){
            player.hand.add(m_deck.deal());
            cout << "Player: ";
            for (Card & card: player.hand.hand){
                cout << card.displayCard() << " ";
            }
            cout << "[" << player.hand.getTotal() << "]\n";

            //If the player gets 21, we break out of the loop
            if (player.hand.getTotal() == 21){
                break;
            }
            //Lets the player know if they have busted
            if (player.isBusted()){
                cout << "Player busts.\n";
                cout << "Casino wins.\n\n";
                break;
            }
        }


        //Continues to draw cards for the computer depending on if the casino must win or the total is <= 16
        while(!player.isBusted() && m_casino.isDrawing() || m_casino.numWins / (m_casino.numLD + m_casino.numWins) < 0.55 && m_casino.hand.getTotal() < player.hand.getTotal() && m_casino.hand.getTotal() <= 16 && !player.isBusted()){
            //This if statement is if the casino has won less than 55% of games
            if (m_casino.numWins / (m_casino.numLD + m_casino.numWins) < 0.55 ){
                bool searching = true;
                while (searching){
                    Card testCard = m_deck.deal();

                    //Checks to make sure the added card won't bust the casino so we can guarantee a win
                    if (m_casino.hand.getTotal() + testCard.getValue() <= 21 ){
                        //Checks to make sure that if the total goes above 16, it beats or ties the player.
                        if ((m_casino.hand.getTotal() + testCard.getValue() > 16) && (m_casino.hand.getTotal() + testCard.getValue() < player.hand.getTotal())){
                            
                        }
                        else{
                            m_casino.hand.add(testCard);
                            searching = false;
                        }
                    }
                    
                }
            }
            //This is if the casino has won more the 55% of the games and has <=16 total points
            else{
                m_casino.hand.add(m_deck.deal());
            }

            //Displays the casinos cards
            cout << "Casino: ";
            for (Card & card: m_casino.hand.hand){
                cout << card.displayCard() << " ";
            }
            cout << "[" << m_casino.hand.getTotal() << "]\n";
            
            //Checks if the casino has busted
            if (m_casino.isBusted()){
                cout << "Casino busts.\n";
                cout << "Player wins.\n\n";
                m_casino.numLD += 1;
                break;
            }
        }

        //Adds a loss/draw to the casino 
        if (player.hand.getTotal() >= m_casino.hand.getTotal() && !player.isBusted())
            m_casino.numLD += 1;
        //Adds the win to the casino if the casino wins
        else
            m_casino.numWins += 1;
        //FIf there was no bust, announces the winner
        if(!player.isBusted() && !m_casino.isBusted()){
            player.announce(m_casino.hand);
        }
        //clears the hand and the deck of the casino for the next round
        m_casino.hand.clear();
        m_deck.deck.clear();
    }

    protected:
    Deck m_deck;
    ComputerPlayer m_casino;
};





//Main Function to run the program
int main()
{
cout << "\tWelcome to the Comp322 Blackjack game!" << endl << endl;
BlackJackGame game;
// The main loop of the game
bool playAgain = true;
char answer = 'y';
while (playAgain)
{
game.play();
// Check whether the player would like to play another round
cout << "Would you like another round? (y/n): ";
cin >> answer;
cout << endl << endl;
playAgain = (answer == 'y' ? true : false);
}
cout <<"Game over!";
return 0;
}
