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
    ~Game();

    bool move(SchaakStuk* s,int r, int k); // Verplaats stuk s naar rij r en kolom k

    bool schaak(zw kleur);
    bool schaakmat(zw kleur);
    bool pat(zw kleur);
    void setStartBord();

    SchaakStuk * getPiece(int r, int k) const;
    void printBord() const;
    bool isBinnenGrens(int r, int k) const;
    void setPiece(int r, int k, SchaakStuk* s, bool deletePreviousPos=false);
    void removePiece(int r, int k);
    bool hasPiece(int r, int k) const;
    bool hasFriendlyPiece(int r, int k, zw kleur) const;
    bool hasEnemyPiece(int r, int k, zw kleur) const;
    void updateAllValidMoves();
    std::vector<std::pair<int, int>> getRadiusMoves(std::pair<int, int> pos, int radiusFactor) const;
    std::vector<std::pair<int, int>> getDiagonalMoves(std::pair<int, int> pos) const;
    std::vector<std::pair<int, int>> getHorizontalMoves(std::pair<int, int> pos) const;
    std::vector<std::pair<int ,int>> getVerticalMoves(std::pair<int, int> pos) const;
    std::vector<std::pair<int, int>> getVerticalAndHorizontalMoves(std::pair<int, int> pos) const;
    std::vector<std::pair<int, int>> filterBlockedMoves(std::vector<std::pair<int, int>> zetten, int aantalSpiegelZetten, zw kleur) const;
    std::vector<std::pair<int, int>> filterIndividualMoves(std::vector<std::pair<int, int>> zetten, zw kleur) const;
    std::pair<int, int> getMirrorX(std::pair<int, int> pos) const;
    std::pair<int, int> getMirrorY(std::pair<int, int> pos) const;
    std::pair<int, int> getMirrorXY(std::pair<int, int> pos) const;
    bool allTrue(std::vector<bool> bools) const;
    bool validTurn(SchaakStuk *s) const;
    void nextTurn();
    std::vector<std::pair<int, int>> concatenateMoves(std::vector<std::vector<std::pair<int, int>>> movesMatrix) const;

private:
    std::array<std::array<SchaakStuk*, 8>, 8> schaakBord {nullptr};
    zw currentTurn;
    // Hier zet jij jouw datastructuur neer om het bord te bewaren ...
};


#endif //SCHAKEN_GAME_H
