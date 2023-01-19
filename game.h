//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#ifndef SCHAKEN_GAME_H
#define SCHAKEN_GAME_H

#include "SchaakStuk.h"

class Game {
// variabelen om de status van het spel/bord te bewaren
public:
    Game();
    Game(zw turn) {currentTurn=turn;}
    Game Clone() const; //copy constructor
    ~Game();

    bool move(SchaakStuk* s, const int &r, const int &k); // Verplaats stuk s naar rij r en kolom k

    bool schaak(const zw &kleur);
    bool schaakmat(const zw &kleur);
    bool pat(const zw &kleur);
    void setStartBord();
    void setStukkenVector();

    std::vector<SchaakStuk*> getStukken() const {return stukken;}
    SchaakStuk * getPiece(const int &r, const int &k) const;
    void printBord() const;
    bool isBinnenGrens(const int &r, const int &k) const;
    void setPiece(const int &r, const int &k, SchaakStuk* s, const bool &deletePreviousPos=false);
    void removePiece(const int &r, const int &k);
    bool hasPiece(const int &r, const int &k) const;
    bool hasFriendlyPiece(const int &r, const int &k, const zw &kleur) const;
    bool hasEnemyPiece(const int &r, const int &k, const zw &kleur) const;
    void updateAllPieces(const bool &filterCheckMoves=true);
    void updateMoveThreats();
    MoveVector getRadiusMoves(Move pos, const int &radiusFactor) const;
    MoveMatrix getDiagonalMoves(Move pos) const;
    MoveMatrix getHorizontalMoves(Move pos) const;
    std::vector<std::vector<std::pair<int ,int>>> getVerticalMoves(Move pos) const;
    MoveVector filterBlockedMoves(MoveVector zetten, const zw &kleur) const;
    MoveMatrix filterBlockedMovesMatrix(MoveMatrix zetten, const zw &kleur) const;
    MoveVector dissolveMatrix(MoveMatrix matrix) const;
    MoveVector filterIndividualMoves(MoveVector zetten, const zw &kleur) const;
    MoveVector filterSelfCheckMoves(MoveVector zetten, Move position) const;
    bool hasMove(const int &r, const int &k, MoveVector moves) const;
    bool validTurn(SchaakStuk *s) const;
    void nextTurn();
    zw getCurrentTurn() const {return currentTurn;}
    std::array<SchaakStuk*, 2> getKoningen() {return koningen;}
    void setKoningen(std::array<SchaakStuk*, 2> newKoningen) {koningen=newKoningen;}
    void setCurrentTurn(zw newTurn) {currentTurn=newTurn;}
    void pushToStukken(SchaakStuk* stuk) {stukken.push_back(stuk);}
    void setSchaakBord(const int &r, const int &k, SchaakStuk* stuk) {schaakBord[r][k] = stuk;}
    MoveVector getMoveIntersection(MoveVector zetten1, MoveVector zetten2) const;
private:
    std::array<std::array<SchaakStuk*, 8>, 8> schaakBord {nullptr};
    std::array<SchaakStuk*, 2> koningen {nullptr};
    std::vector<SchaakStuk*> stukken;
    zw currentTurn;
    // Hier zet jij jouw datastructuur neer om het bord te bewaren ...
};


#endif //SCHAKEN_GAME_H
