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

typedef std::pair<int, int> Move;
typedef std::vector<Move> MoveVector;
typedef std::vector<MoveVector> MoveMatrix;

class SchaakStuk {
public:
    SchaakStuk(zw kleur): kleur(kleur) {}

    virtual MoveVector geldige_zetten(Game &game) const = 0;
    bool isZetGeldig(int r, int k) const;
    virtual Piece piece() const=0;      // Verander deze functie niet!
                                        // Deze functie wordt gebruikt door
                                        // setItem(x,y,SchaakStuk*) van
                                        // SchaakGUI

    zw getKleur() const { return kleur; }

    Move getPositie() const { return positie; }
    void setPositie(std::pair<int, int> newPositie, Game &game);
    MoveVector getValidMoves() const {return validMoves;}
    void updateValidMoves(Game &game);
private:
    zw kleur;
   Move positie;
    MoveVector validMoves;
};

class Pion:public SchaakStuk {
public:
    Pion(zw kleur):SchaakStuk(kleur) {}
    virtual Piece piece() const override {
        return Piece(Piece::Pawn,getKleur()==wit?Piece::White:Piece::Black);
    }
    bool zet_geldig(std::pair<int, int> zet,  zetType type, Game &game) const;
    MoveVector filter_ongeldige_zetten(MoveVector zetten, zetType type, Game &game) const;
    MoveVector geldige_zetten(Game &game) const;
    bool heeft_bewogen() const;
};

class Toren:public SchaakStuk {
public:
    Toren(zw kleur):SchaakStuk(kleur) {}
    MoveVector geldige_zetten(Game &game) const;
    Piece piece() const override {
        return Piece(Piece::Rook,getKleur()==wit?Piece::White:Piece::Black);
    }
};

class Paard:public SchaakStuk {
public:
    Paard(zw kleur):SchaakStuk(kleur) {}
    MoveVector geldige_zetten(Game &game) const;
    Piece piece() const override {
        return Piece(Piece::Knight,getKleur()==wit?Piece::White:Piece::Black);
    }
};

class Loper:public SchaakStuk {
public:
    Loper(zw kleur):SchaakStuk(kleur) {}
    MoveVector geldige_zetten(Game &game) const;
    Piece piece() const override {
        return Piece(Piece::Bishop,getKleur()==wit?Piece::White:Piece::Black);
    }
};

class Koning:public SchaakStuk {
public:
    Koning(zw kleur):SchaakStuk(kleur) {}
    MoveVector geldige_zetten(Game &game) const;
    Piece piece() const override {
        return Piece(Piece::King,getKleur()==wit?Piece::White:Piece::Black);
    }
};

class Koningin:public SchaakStuk {
public:
    Koningin(zw kleur):SchaakStuk(kleur) {}
    MoveVector geldige_zetten(Game &game) const;
    Piece piece() const override {
        return Piece(Piece::Queen,getKleur()==wit?Piece::White:Piece::Black);
    }
};

#endif //SCHAKEN_SCHAAKSTUK_H
