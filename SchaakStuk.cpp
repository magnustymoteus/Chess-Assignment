//  Student: Patryk Pilichowski
//  Rolnummer: 20220383
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#include "SchaakStuk.h"
#include "game.h"
#include <iostream>

SchaakStuk* Pion::Clone() {
    SchaakStuk* newS = new Pion(getKleur());
    newS->setPositie(getPositie());
    newS->setValidMoves(getValidMoves());
    newS->setCanBeTaken(getCanBeTaken());
    return newS;
}
SchaakStuk* Toren::Clone() {
    SchaakStuk* newS = new Toren(getKleur());
    newS->setPositie(getPositie());
    newS->setValidMoves(getValidMoves());
    newS->setCanBeTaken(getCanBeTaken());
    return newS;
}
SchaakStuk* Paard::Clone() {
    SchaakStuk* newS = new Paard(getKleur());
    newS->setPositie(getPositie());
    newS->setValidMoves(getValidMoves());
    newS->setCanBeTaken(getCanBeTaken());
    return newS;
}
SchaakStuk* Loper::Clone() {
    SchaakStuk* newS = new Loper(getKleur());
    newS->setPositie(getPositie());
    newS->setValidMoves(getValidMoves());
    newS->setCanBeTaken(getCanBeTaken());
    return newS;
}
SchaakStuk* Koning::Clone() {
    SchaakStuk* newS = new Koning(getKleur());
    newS->setPositie(getPositie());
    newS->setValidMoves(getValidMoves());
    newS->setCanBeTaken(getCanBeTaken());
    return newS;
}
SchaakStuk* Koningin::Clone() {
    SchaakStuk* newS = new Koningin(getKleur());
    newS->setPositie(getPositie());
    newS->setValidMoves(getValidMoves());
    newS->setCanBeTaken(getCanBeTaken());
    return newS;
}
void SchaakStuk::setPositie(std::pair<int, int> newPositie) {
    positie=newPositie;
}
void SchaakStuk::updateValidMoves(Game &game, bool filterCheckMoves) {
    validMoves = geldige_zetten(game, filterCheckMoves);
}
bool SchaakStuk::isZetGeldig(int r, int k) const {
    for(std::pair<int, int> currentMove : validMoves) {
        if(currentMove.first == r && currentMove.second == k) return true;
    }
    return false;
}
bool Pion::zet_geldig(std::pair<int, int> zet, zetType type, Game &game) const {
    int r = zet.first, k = zet.second;
    if(!game.isBinnenGrens(r,k)) return false;
    bool isGeldig;
    switch(type) {
        case vangbaar:
            isGeldig = game.hasEnemyPiece(r, k, getKleur());
            break;
        case speciaal:
            isGeldig = !heeft_bewogen() && !game.hasPiece(r, k) && !game.hasPiece(((getKleur()==wit)?r+1:r-1), k);
            break;
        default:
            isGeldig = !game.hasPiece(r,k);
            break;
    }
    return isGeldig;
}
MoveVector Pion::filter_ongeldige_zetten(MoveVector zetten, zetType type, Game &game) const {
    //de pion is een speciale stuk waardoor hij zijn eigen methoden heeft van geldige zetten.
    MoveVector geldige_zetten;
    //alle zetten moeten van hetzelfde type zijn
    for(Move zet : zetten) {
        if(zet_geldig(zet, type, game) && game.isBinnenGrens(zet.first, zet.second)) geldige_zetten.push_back(zet);
    }
    return geldige_zetten;
}

bool Pion::heeft_bewogen() const {
    switch(getKleur()) {
        case wit:
            return (getPositie().first) != 6;
        default:
            return (getPositie().first) != 1;
    }
}

MoveVector Pion::geldige_zetten(Game &game, bool filterCheckMoves) const {
    MoveVector geldige_zetten;
    MoveVector vangbare_zetten;
    int direction = (getKleur() == wit) ? -1 : 1;
   Move normaleZet = {getPositie().first+direction,getPositie().second};
   Move specialeZet = {getPositie().first+(direction*2), getPositie().second};

    vangbare_zetten.push_back({getPositie().first+direction, getPositie().second+1});
    vangbare_zetten.push_back({getPositie().first+direction, getPositie().second-1});
    vangbare_zetten = filter_ongeldige_zetten(vangbare_zetten, vangbaar, game);
    geldige_zetten = vangbare_zetten;
    if(zet_geldig(normaleZet, normaal, game)) geldige_zetten.push_back(normaleZet);
    if(zet_geldig(specialeZet, speciaal, game)) geldige_zetten.push_back(specialeZet);
    if(filterCheckMoves) geldige_zetten = game.filterSelfCheckMoves(geldige_zetten, getPositie());
    return geldige_zetten;
}
MoveVector Toren::geldige_zetten(Game &game, bool filterCheckMoves) const {
    MoveVector geldige_zetten, verticale_zetten, horizontale_zetten;
    MoveMatrix verticale_zetten_matrix = game.getVerticalMoves(getPositie());
    MoveMatrix horizontale_zetten_matrix = game.getHorizontalMoves(getPositie());
    verticale_zetten = game.dissolveMatrix(game.filterBlockedMovesMatrix(verticale_zetten_matrix, getKleur()));
    horizontale_zetten = game.dissolveMatrix(game.filterBlockedMovesMatrix(horizontale_zetten_matrix, getKleur()));
    geldige_zetten = game.concatenateMoves({verticale_zetten, horizontale_zetten});
    if(filterCheckMoves) geldige_zetten = game.filterSelfCheckMoves(geldige_zetten, getPositie());
    return geldige_zetten;
}
MoveVector Loper::geldige_zetten(Game &game, bool filterCheckMoves) const {
    MoveVector geldige_zetten;
    MoveMatrix diagonale_zetten = game.getDiagonalMoves(getPositie());
    diagonale_zetten = game.filterBlockedMovesMatrix(diagonale_zetten, getKleur());
    geldige_zetten = game.dissolveMatrix(diagonale_zetten);
    if(filterCheckMoves) geldige_zetten = game.filterSelfCheckMoves(geldige_zetten, getPositie());
    return geldige_zetten;
}
MoveVector Koningin::geldige_zetten(Game &game, bool filterCheckMoves) const {
    MoveVector geldige_zetten;
    MoveMatrix verticale_zetten = game.getVerticalMoves(getPositie());
    MoveMatrix horizontale_zetten = game.getHorizontalMoves(getPositie());
    MoveMatrix diagonale_zetten = game.getDiagonalMoves(getPositie());
    verticale_zetten = game.filterBlockedMovesMatrix(verticale_zetten, getKleur());
    horizontale_zetten = game.filterBlockedMovesMatrix(horizontale_zetten, getKleur());
    diagonale_zetten = game.filterBlockedMovesMatrix(diagonale_zetten, getKleur());
    geldige_zetten = game.concatenateMoves({game.dissolveMatrix(verticale_zetten),
                                            game.dissolveMatrix(horizontale_zetten),
                                            game.dissolveMatrix(diagonale_zetten)});
    if(filterCheckMoves) geldige_zetten = game.filterSelfCheckMoves(geldige_zetten, getPositie());
    return geldige_zetten;
}
MoveVector Koning::geldige_zetten(Game &game, bool filterCheckMoves) const {
    MoveVector geldige_zetten;
    MoveVector radius = game.getRadiusMoves(getPositie(), 1);
    geldige_zetten = game.filterIndividualMoves(radius, getKleur());
    if(filterCheckMoves) geldige_zetten = game.filterSelfCheckMoves(geldige_zetten, getPositie());
    return geldige_zetten;
}
MoveVector Paard::geldige_zetten(Game &game, bool filterCheckMoves) const {
    MoveVector geldige_zetten;
    MoveVector moves1 = {{getPositie().first-1, getPositie().second-2},
                                               {getPositie().first-1, getPositie().second+2},
                                               {getPositie().first+1, getPositie().second-2},
                                               {getPositie().first+1, getPositie().second+2}
    };
    MoveVector moves2 = {{getPositie().first-2, getPositie().second-1},
                                               {getPositie().first-2, getPositie().second+1},
                                               {getPositie().first+2, getPositie().second-1},
                                               {getPositie().first+2, getPositie().second+1},
    };
    geldige_zetten = game.concatenateMoves({game.filterIndividualMoves(moves1, getKleur()), game.filterIndividualMoves(moves2, getKleur())});
    if(filterCheckMoves) geldige_zetten = game.filterSelfCheckMoves(geldige_zetten, getPositie());
    return geldige_zetten;
}