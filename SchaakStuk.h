//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#ifndef SCHAKEN_SCHAAKSTUK_H
#define SCHAKEN_SCHAAKSTUK_H
#include <guicode/chessboard.h>

class Game;

enum zw{zwart,wit};
enum zetType{normaal, vangbaar, speciaal};

class SchaakStuk {
public:
    SchaakStuk(zw kleur): kleur(kleur) {}

    virtual std::vector<std::pair<int, int>> geldige_zetten(Game &game) const;
    virtual bool zet_geldig(int r, int k, Game &game) const;
    virtual bool zet_geldig(std::pair<int, int> zet, Game &game) const;
    virtual std::vector<std::pair<int, int>> filter_ongeldige_zetten(std::vector<std::pair<int, int>> zetten, Game &game) const;
    virtual Piece piece() const=0;      // Verander deze functie niet!
                                        // Deze functie wordt gebruikt door
                                        // setItem(x,y,SchaakStuk*) van
                                        // SchaakGUI

    zw getKleur() const { return kleur; }

    std::pair<int, int> getPositie() const { return positie; }
    void setPositie(std::pair<int, int> newPositie) { positie=newPositie; }
private:
    zw kleur;
    std::pair<int, int> positie;
};

class Pion:public SchaakStuk {
public:
    Pion(zw kleur):SchaakStuk(kleur) {}
    virtual Piece piece() const override {
        return Piece(Piece::Pawn,getKleur()==wit?Piece::White:Piece::Black);
    }
    bool zet_geldig(int r, int k, zetType type, Game &game) const;
    bool zet_geldig(std::pair<int, int> zet,  zetType type, Game &game) const;
    std::vector<std::pair<int, int>> filter_ongeldige_zetten(std::vector<std::pair<int, int>> zetten, zetType type, Game &game) const;
    std::vector<std::pair<int, int>> geldige_zetten(Game &game) const;
    bool heeft_bewogen() const;
};

class Toren:public SchaakStuk {
public:
    Toren(zw kleur):SchaakStuk(kleur) {}
    std::vector<std::pair<int, int>> geldige_zetten(Game &game) const;
    Piece piece() const override {
        return Piece(Piece::Rook,getKleur()==wit?Piece::White:Piece::Black);
    }
};

class Paard:public SchaakStuk {
public:
    Paard(zw kleur):SchaakStuk(kleur) {}

    Piece piece() const override {
        return Piece(Piece::Knight,getKleur()==wit?Piece::White:Piece::Black);
    }
};

class Loper:public SchaakStuk {
public:
    Loper(zw kleur):SchaakStuk(kleur) {}

    Piece piece() const override {
        return Piece(Piece::Bishop,getKleur()==wit?Piece::White:Piece::Black);
    }
};

class Koning:public SchaakStuk {
public:
    Koning(zw kleur):SchaakStuk(kleur) {}

    Piece piece() const override {
        return Piece(Piece::King,getKleur()==wit?Piece::White:Piece::Black);
    }
};

class Koningin:public SchaakStuk {
public:
    Koningin(zw kleur):SchaakStuk(kleur) {}

    Piece piece() const override {
        return Piece(Piece::Queen,getKleur()==wit?Piece::White:Piece::Black);
    }
};

#endif //SCHAKEN_SCHAAKSTUK_H
