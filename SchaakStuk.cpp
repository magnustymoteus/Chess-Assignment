//  Student: Patryk Pilichowski
//  Rolnummer: 20220383
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#include "SchaakStuk.h"
#include "game.h"

std::vector<std::pair<int, int>> SchaakStuk::geldige_zetten(Game &game) const {
    return {};
}

bool SchaakStuk::zet_geldig(int r, int k, Game &game) const {
    return !game.hasFriendlyPiece(r, k, getKleur());
    //de "normale" regels die voor de meerderheid van de stukken moeten gelden
}
bool SchaakStuk::zet_geldig(std::pair<int, int> zet, Game &game) const {
    int r=zet.first, k=zet.second;
    return !game.hasFriendlyPiece(r, k, getKleur());
    //de "normale" regels die voor de meerderheid van de stukken moeten gelden
}
std::vector<std::pair<int, int>> SchaakStuk::filter_ongeldige_zetten(std::vector<std::pair<int, int>> zetten, Game &game) const {
    std::vector<std::pair<int, int>> geldige_zetten;
    for(auto zet : zetten) {
        if(zet_geldig(zet, game)) geldige_zetten.push_back(zet);
    }
    return geldige_zetten;
}


bool Pion::zet_geldig(int r, int k, zetType type, Game &game) const {
    bool isGeldig;
    switch(type) {
        case normaal:
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
    geldige_zetten.insert(geldige_zetten.end(), vangbare_zetten.begin(), vangbare_zetten.end());
    if(zet_geldig(normaleZet, normaal, game)) geldige_zetten.push_back(normaleZet);
    if(zet_geldig(specialeZet, speciaal, game)) geldige_zetten.push_back(specialeZet);
    return geldige_zetten;
}
