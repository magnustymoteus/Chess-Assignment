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
    void resetStukkenVector() {stukken.clear();}
    void resetSchaakBord();

    std::vector<SchaakStuk*> getStukken() const {return stukken;}
    SchaakStuk * getPiece(const int &r, const int &k) const;
    static bool isBinnenGrens(const int &r, const int &k) ;
    void setPiece(const int &r, const int &k, SchaakStuk* s, const bool &deletePreviousPos=false);
    void removePiece(const int &r, const int &k);
    bool hasPiece(const int &r, const int &k) const;
    bool hasFriendlyPiece(const int &r, const int &k, const zw &kleur) const;
    bool hasEnemyPiece(const int &r, const int &k, const zw &kleur) const;
    void updateAllPieces(const bool &filterCheckMoves=true);
    static MoveVector getRadiusMoves(Move pos, const int &radiusFactor) ;
    static MoveMatrix getDiagonalMoves(Move pos) ;
    static MoveMatrix getHorizontalMoves(Move pos) ;
    static std::vector<std::vector<std::pair<int ,int>>> getVerticalMoves(Move pos) ;
    MoveVector filterBlockedMoves(const MoveVector& zetten, const zw &kleur) const;
    MoveVector filterBlockedMovesMatrix(const MoveMatrix& zetten, const zw &kleur) const;
    static MoveVector dissolveMatrix(const MoveMatrix& matrix) ;
    MoveVector filterIndividualMoves(const MoveVector& zetten, const zw &kleur) const;
    MoveVector filterSelfCheckMoves(const MoveVector& zetten, Move position) const;
    static bool hasMove(const int &r, const int &k, const MoveVector& moves) ;
    void nextTurn();
    zw getCurrentTurn() const {return currentTurn;}
    void setKoningen(std::array<SchaakStuk*, 2> newKoningen) {koningen=newKoningen;}
    void setCurrentTurn(zw newTurn) {currentTurn=newTurn;}
    void pushToStukken(SchaakStuk* stuk) {stukken.push_back(stuk);}
    void setSchaakBord(const int &r, const int &k, SchaakStuk* stuk) {schaakBord[r][k] = stuk;}
    static MoveVector getMoveIntersection(const MoveVector& zetten1, const MoveVector& zetten2) ;
    static std::pair<SchaakStuk*, Move> getRandomMoveOfPiece(std::pair<SchaakStuk*, MoveVector> moves) ;
    static std::pair<SchaakStuk*, MoveVector> getRandomPieceOfVector(std::vector<std::pair<SchaakStuk*, MoveVector>> pieces) ;
    std::tuple<MoveVector, MoveVector, MoveVector, MoveVector> getPrioritizedMovesOfPiece(SchaakStuk *s) const;
    void takeRandomPrioritizedMove(const zw &kleur);
private:
    std::array<std::array<SchaakStuk*, 8>, 8> schaakBord {nullptr};
    std::array<SchaakStuk*, 2> koningen {nullptr};
    std::vector<SchaakStuk*> stukken;
    zw currentTurn;

    // Hier zet jij jouw datastructuur neer om het bord te bewaren ...
};


#endif //SCHAKEN_GAME_H
