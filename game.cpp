//  Student: Patryk Pilichowski
//  Rolnummer: 20220383
//  Opmerkingen: (bvb aanpassingen van de opgave)
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#include "game.h"
#include <iostream>

Game::Game() {
    setStartBord();
    currentTurn = wit;
}

Game::~Game() {}

void Game::printBord() const {
    for(int i=0;i<schaakBord.size();i++) {
        for(int j=0;j<schaakBord[i].size();j++) {
            if(schaakBord[i][j] != nullptr) std::cout << "y ";
            else std::cout << "n ";
        }
        std::cout << std::endl;
    }
}
void Game::updateAllValidMoves() {
    for(int i=0;i<schaakBord.size();i++) {
        for(int j=0;j<schaakBord[i].size();j++) {
            if(schaakBord[i][j] != nullptr) schaakBord[i][j]->updateValidMoves(*this);
        }
    }
}
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
    updateAllValidMoves();
}

// Verplaats stuk s naar positie (r,k)
// Als deze move niet mogelijk is, wordt false teruggegeven
// en verandert er niets aan het schaakbord.
// Anders wordt de move uitgevoerd en wordt true teruggegeven
bool Game::move(SchaakStuk* s, int r, int k) {
    if(s->isZetGeldig(r, k)) {
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
       schaakBord[s->getPositie().first][s->getPositie().second] = nullptr;
        s->setPositie({r, k}, *this);
    }
   else if(!isBinnenGrens(r, k)) throw std::invalid_argument("not inside chess boundary");
   else if(s == nullptr) throw std::invalid_argument("chess piece is a null pointer");
}
bool Game::isBinnenGrens(int r, int k) const {
    return ((0<=r && r<=7) && (0<=k && k<=7));
}
bool Game::hasFriendlyPiece(int r, int k, zw kleur) const {
    if(getPiece(r, k ) != nullptr) {
        SchaakStuk *s2 = getPiece(r, k);
        return (s2->getKleur() == kleur);
    }
    return false;
}
bool Game::hasEnemyPiece(int r, int k, zw kleur) const {
    if(getPiece(r, k ) != nullptr) {
        SchaakStuk *s2 = getPiece(r, k);
        return (s2->getKleur() != kleur);
    }
}
bool Game::hasPiece(int r, int k) const {
    SchaakStuk *s = getPiece(r, k);
    return (s != nullptr);
}
std::vector<std::pair<int, int>> Game::getDiagonalMoves(std::pair<int, int> pos) const {
    std::pair<int, int> current = {pos.first+1, pos.second+1};
    std::vector<std::pair<int, int>> moves;
    for(int i=0;i<8;i++, current.first++, current.second++) {
        std::pair<int, int> currentMoves[4] = {current, getMirrorX(current), getMirrorY(current),
                                               getMirrorXY(current)};
        for(std::pair<int, int> currentMove : currentMoves) {
            if(isBinnenGrens(currentMove.first, currentMove.second)) moves.push_back(currentMove);
        }
    }
    return moves;
}
std::vector<std::pair<int, int>> Game::getHorizontalMoves(std::pair<int, int> pos) const {
    int currR = pos.first, currK = pos.second+1;
    std::vector<std::pair<int, int>> moves;
    int i=currR;
    while(i >= 0 && i <= 7) {
        moves.push_back({i, currK});
        moves.push_back(getMirrorY({i, currK}));
        i++;
    }
    return moves;
}
std::vector<std::pair<int ,int>> Game::getVerticalMoves(std::pair<int, int> pos) const {
    int currR = pos.first+1, currK = pos.second;
    std::vector<std::pair<int, int>> moves;
    int i = currK;
    while(i >= 0 && i <= 7) {
        moves.push_back({currR, i});
        moves.push_back(getMirrorX({currR, i}));
        i++;
    }
    return moves;
}
std::vector<std::pair<int, int>> Game::getVerticalAndHorizontalMoves(std::pair<int, int> pos) const {
    std::vector<std::pair<int, int>> horizontal = getHorizontalMoves(pos);
    std::vector<std::pair<int, int>> vertical = getVerticalMoves(pos);
    horizontal.insert(horizontal.end(), vertical.begin(), vertical.end());
    return horizontal;
}
std::vector<std::pair<int, int>> Game::getRadiusMoves(std::pair<int, int> pos, int radiusFactor) const {
    std::vector<std::pair<int, int>> moves;
    std::pair<int, int> startingCoord = {pos.first-radiusFactor, pos.second-radiusFactor};
    for(int i=startingCoord.first;i<startingCoord.first+radiusFactor*2+1;i++) {
        for(int j=startingCoord.second;j<startingCoord.second+radiusFactor*2+1;j++) {
            if(i != pos.first || j != pos.second ) moves.push_back({i, j});
        }
    }
    return moves;
}
std::pair<int, int> Game::getMirrorX(std::pair<int, int> pos) const {
    return {7-pos.first, pos.second};
}
std::pair<int, int> Game::getMirrorY(std::pair<int, int> pos) const {
    return {pos.first, 7-pos.second};
}
std::pair<int, int> Game::getMirrorXY(std::pair<int, int> pos) const {
    return getMirrorX(getMirrorY(pos));
}
std::vector<std::pair<int, int>> Game::filterBlockedMoves(std::vector<std::pair<int, int>> zetten, int aantalSpiegelZetten, zw kleur) const {
    /*aantalSpiegelZetten:
     * diagonaal: 4 'spiegelzetten'
     * horizontaal & verticaal: 2 'spiegelzetten'
     */
    std::vector<bool> stop(aantalSpiegelZetten, false);
    std::vector<std::pair<int, int>> geldige_zetten;
    for(int i=0;(i<zetten.size()) && (!allTrue(stop));i++) {
        if(!stop[i%stop.size()]) {
            int currentRow = zetten[i].first, currentColumn = zetten[i].second;
            if(!isBinnenGrens(currentRow, currentColumn) || hasPiece(currentRow, currentColumn)) {
                stop[i%stop.size()] = true;
                if(isBinnenGrens(currentRow, currentColumn) &&
                hasEnemyPiece(currentRow, currentColumn, kleur))
                    geldige_zetten.push_back({currentRow, currentColumn});
            }
            else geldige_zetten.push_back({currentRow, currentColumn});
        }
    }
    return geldige_zetten;
}

std::vector<std::pair<int, int>> Game::filterIndividualMoves(std::vector<std::pair<int, int>> zetten, zw kleur) const {
    std::vector<std::pair<int, int>> valid_moves;
    for(std::pair<int, int> currentMove : zetten) {
        if(isBinnenGrens(currentMove.first, currentMove.second) && !hasFriendlyPiece(currentMove.first, currentMove.second, kleur)) {
            valid_moves.push_back(currentMove);
        }
    }
    return valid_moves;
}


bool Game::allTrue(std::vector<bool> bools) const {
    for(bool currentBool : bools) {
        if(!currentBool) return false;
    }
    return true;
}
std::vector<std::pair<int, int>> Game::concatenateMoves(std::vector<std::vector<std::pair<int, int>>> movesMatrix) const {
    std::vector<std::pair<int, int>> moves;
    for(std::vector<std::pair<int, int>> currentMoveArray : movesMatrix) {
        moves.insert(moves.end(), currentMoveArray.begin(), currentMoveArray.end());
    }
    return moves;
}

bool Game::validTurn(SchaakStuk *s) const {
    return currentTurn == s->getKleur();
}
void Game::nextTurn() {
    currentTurn = (currentTurn == zwart) ? wit : zwart;
}