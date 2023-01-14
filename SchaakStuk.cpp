//  Student: Patryk Pilichowski
//  Rolnummer: 20220383
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#include "SchaakStuk.h"
#include "game.h"
#include <iostream>

void SchaakStuk::setPositie(std::pair<int, int> newPositie, Game &game) {
    positie=newPositie;
    //validMoves = geldige_zetten(game);
}
bool SchaakStuk::isZetGeldig(int r, int k) const {
    for(std::pair<int, int> currentMove : validMoves) {
        if(currentMove.first == r && currentMove.second == k) return true;
    }
    return false;
}
bool Pion::zet_geldig(std::pair<int, int> zet, zetType type, Game &game) const {
    int r = zet.first, k = zet.second;
    bool isGeldig;
    switch(type) {
        case vangbaar:
            isGeldig = game.hasEnemyPiece(r, k, getKleur());
            break;
        case speciaal:
            isGeldig = !heeft_bewogen() && !game.hasPiece(r, k);
            break;
        default:
            isGeldig = !game.hasPiece(r,k);
            break;
    }
    return isGeldig;
}
std::vector<std::pair<int, int>> Pion::filter_ongeldige_zetten(std::vector<std::pair<int, int>> zetten, zetType type, Game &game) const {
    //de pion is een speciale stuk waardoor hij zijn eigen methoden heeft van geldige zetten.
    std::vector<std::pair<int, int>> geldige_zetten;
    //alle zetten moeten van hetzelfde type zijn
    for(auto zet : zetten) {
        if(zet_geldig(zet, type, game)) geldige_zetten.push_back(zet);
    }
    return geldige_zetten;
}

bool Pion::heeft_bewogen() const {
    return !((getPositie().first == 1 && getKleur() == zwart) || (getPositie().second == 6 && getKleur() == wit));
}

std::vector<std::pair<int, int>> Pion::geldige_zetten(Game &game) const {
    std::vector<std::pair<int, int>> geldige_zetten;
    std::vector<std::pair<int, int>> vangbare_zetten;

    int direction = (getKleur() == wit) ? 1 : -1;
    std::pair<int, int> normaleZet = {getPositie().first+direction,getPositie().second};
    std::pair<int, int> specialeZet = {getPositie().first+(direction*2), getPositie().second};

    vangbare_zetten.push_back({getPositie().first+direction, getPositie().second+1});
    vangbare_zetten.push_back({getPositie().first+direction, getPositie().second-1});
    vangbare_zetten = filter_ongeldige_zetten(vangbare_zetten, vangbaar, game);
    geldige_zetten = game.concatenateMoves({vangbare_zetten});
    if(zet_geldig(normaleZet, normaal, game)) geldige_zetten.push_back(normaleZet);
    if(zet_geldig(specialeZet, speciaal, game)) geldige_zetten.push_back(specialeZet);
    return geldige_zetten;
}
std::vector<std::pair<int, int>> Toren::geldige_zetten(Game &game) const {
    std::cout << "toren" << std::endl;
    std::vector<std::pair<int, int>> geldige_zetten;
    std::vector<std::pair<int, int>> verticale_zetten = game.getVerticalMoves(getPositie());
    std::vector<std::pair<int, int>> horizontale_zetten = game.getHorizontalMoves(getPositie());
    verticale_zetten = game.filterBlockedMoves(verticale_zetten, 2, getKleur());
    horizontale_zetten = game.filterBlockedMoves(horizontale_zetten, 2, getKleur());
    geldige_zetten = game.concatenateMoves({verticale_zetten, horizontale_zetten});
    return geldige_zetten;
}
std::vector<std::pair<int, int>> Loper::geldige_zetten(Game &game) const {
    std::cout << "loper" << std::endl;
    std::vector<std::pair<int, int>> geldige_zetten;
    std::vector<std::pair<int, int>> diagonale_zetten = game.getDiagonalMoves(getPositie());
    diagonale_zetten = game.filterBlockedMoves(diagonale_zetten, 4, getKleur());
    geldige_zetten = game.concatenateMoves({diagonale_zetten});
    return geldige_zetten;
}
std::vector<std::pair<int, int>> Koningin::geldige_zetten(Game &game) const {
    std::cout << "koningin" << std::endl;
    std::vector<std::pair<int, int>> geldige_zetten;
    std::vector<std::pair<int, int>> verticale_zetten = game.getVerticalMoves(getPositie());
    std::cout << "koningin 1" << std::endl;
    std::vector<std::pair<int, int>> horizontale_zetten = game.getHorizontalMoves(getPositie());
    std::cout << "koningin 2" << std::endl;
    std::vector<std::pair<int, int>> diagonale_zetten = game.getDiagonalMoves(getPositie());
    std::cout << "koningin 3" << std::endl;
    verticale_zetten = game.filterBlockedMoves(verticale_zetten, 2, getKleur());
    std::cout << "koningin 4" << std::endl;
    horizontale_zetten = game.filterBlockedMoves(horizontale_zetten, 2, getKleur());
    std::cout << "koningin 5" << std::endl;
    diagonale_zetten = game.filterBlockedMoves(diagonale_zetten, 4, getKleur());
    std::cout << "koningin 6" << std::endl;
    geldige_zetten = game.concatenateMoves({verticale_zetten, horizontale_zetten, diagonale_zetten});
    return geldige_zetten;
}
std::vector<std::pair<int, int>> Koning::geldige_zetten(Game &game) const {
    std::cout << "koning" << std::endl;
    std::vector<std::pair<int, int>> geldige_zetten = game.getRadiusMoves(getPositie(), 1);
    geldige_zetten = game.filterIndividualMoves(geldige_zetten, getKleur());
    return geldige_zetten;
}
std::vector<std::pair<int, int>> Paard::geldige_zetten(Game &game) const {
    std::cout << "paard" << std::endl;
    std::vector<std::pair<int, int>> geldige_zetten;
    std::vector<std::pair<int, int>> moves1 = {{getPositie().first-1, getPositie().second-2}};
    std::vector<std::pair<int, int>> moves2 = {{getPositie().first-2, getPositie().second-1}};
    moves1 = {moves1[0], game.getMirrorX(moves1[0]), game.getMirrorY(moves1[0]), game.getMirrorXY(moves1[0])};
    moves2 = {moves2[0], game.getMirrorX(moves2[0]), game.getMirrorY(moves2[0]), game.getMirrorXY(moves2[0])};
    geldige_zetten = game.concatenateMoves({game.filterIndividualMoves(moves1, getKleur()), game.filterIndividualMoves(moves2, getKleur())});
    return geldige_zetten;
}