//  Student: Patryk Pilichowski
//  Rolnummer: 20220383
//  Opmerkingen: (bvb aanpassingen van de opgave)
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#include "game.h"

Game::Game() {
    setStartBord();
}

Game::~Game() {}

// Zet het bord klaar; voeg de stukken op de jusite plaats toe
void Game::setStartBord() {
    setPiece(0, 3, new Koningin(zwart));
    setPiece(7, 3, new Koningin(wit));
    setPiece(0, 4, new Koning(zwart));
    setPiece(7, 4, new Koning(wit));
    for(int i=0;i<8;i++) {
        setPiece(1, i, new Pion(zwart));
        setPiece(6, i, new Pion(wit));
        if(i==0 || 7-i==7) {
            setPiece(7, i, new Toren(wit)), setPiece(0,i,new Toren(zwart));
            setPiece(7, 7-i, new Toren(wit)), setPiece(0,7-i,new Toren(zwart));
        }
        else if(i==1 || 7-i==6) {
            setPiece(7, i, new Paard(wit)), setPiece(0,i,new Paard(zwart));
            setPiece(7, 7-i, new Paard(wit)), setPiece(0,7-i,new Paard(zwart));
        }
        else if(i==2 || 7-i==5) {
            setPiece(7, i, new Loper(wit)), setPiece(0,i,new Loper(zwart));
            setPiece(7, 7-i, new Loper(wit)), setPiece(0,7-i,new Loper(zwart));
        }
    }
    for(int i=2;i<6;i++) {
        for(int j=0;j<8;j++) {
            setPiece(i, j, nullptr);
        }
    }
}

// Verplaats stuk s naar positie (r,k)
// Als deze move niet mogelijk is, wordt false teruggegeven
// en verandert er niets aan het schaakbord.
// Anders wordt de move uitgevoerd en wordt true teruggegeven
bool Game::move(SchaakStuk* s, int r, int k) {
    if(s->zet_geldig(r,k,*this)) {
        setPiece(r,k,s);
        return true;
    }
    return false;
}

// Geeft true als kleur schaak staat
bool Game::schaak(zw kleur) {
    return false;
}

// Geeft true als kleur schaakmat staat
bool Game::schaakmat(zw kleur) {
    return false;
}

// Geeft true als kleur pat staat
// (pat = geen geldige zet mogelijk, maar kleur staat niet schaak;
// dit resulteert in een gelijkspel)
bool Game::pat(zw kleur) {
    return false;
}

// Geeft een pointer naar het schaakstuk dat op rij r, kolom k staat
// Als er geen schaakstuk staat op deze positie, geef nullptr terug
SchaakStuk * Game::getPiece(int r, int k) const {
    return schaakBord[r][k];
}

// Zet het schaakstuk waar s naar verwijst neer op rij r, kolom k.
// Als er al een schaakstuk staat, wordt het overschreven.
// Bewaar in jouw datastructuur de *pointer* naar het schaakstuk,
// niet het schaakstuk zelf.
void Game::setPiece(int r, int k, SchaakStuk* s)
{
    if(isBinnenGrens(r, k) && s != nullptr) {
       schaakBord[r][k] = s;
        s->setPositie({r, k});
    }
   else if(!isBinnenGrens(r, k)) throw std::invalid_argument("not inside chess boundary");
}
bool Game::isBinnenGrens(int r, int k) const {
    return ((0<=r && r<=7) && (0<=k && k<=7));
}
bool Game::hasFriendlyPiece(int r, int k, zw kleur) const {
    SchaakStuk *s2 = getPiece(r, k);
    return (s2 != nullptr && s2->getKleur() == kleur);
}
bool Game::hasEnemyPiece(int r, int k, zw kleur) const {
    SchaakStuk *s2 = getPiece(r, k);
    return (s2 != nullptr && s2->getKleur() != kleur);
}
bool Game::hasPiece(int r, int k) const {
    SchaakStuk *s = getPiece(r, k);
    return (s != nullptr);
}
