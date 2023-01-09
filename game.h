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
    bool isBinnenGrens(int r, int k) const;
    void setPiece(int r, int k, SchaakStuk* s);
    bool hasPiece(int r, int k) const;
    bool hasFriendlyPiece(int r, int k, zw kleur) const;
    bool hasEnemyPiece(int r, int k, zw kleur) const;

private:
    std::array<std::array<SchaakStuk*, 8>, 8> schaakBord {nullptr};
    // Hier zet jij jouw datastructuur neer om het bord te bewaren ...
};


#endif //SCHAKEN_GAME_H
