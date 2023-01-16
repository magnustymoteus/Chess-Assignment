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
    std::cout << std::endl;
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
    SchaakStuk *koning_zwart = new Koning(zwart), *koning_wit = new Koning(wit);
    setPiece(0, 4, koning_zwart);
    setPiece(7, 4, koning_wit);
    koningen[0] = koning_wit, koningen[1] = koning_zwart;

    setPiece(0, 3, new Koningin(zwart));
    setPiece(7, 3, new Koningin(wit));
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
    Move koningPos = koningen[kleur == zwart]->getPositie();
    for(std::array<SchaakStuk*, 8> currentRow: schaakBord) {
        for(SchaakStuk* currentPiece : currentRow) {
            if(currentPiece != nullptr && currentPiece->getKleur() != kleur &&
               hasMove(koningPos.first, koningPos.second,currentPiece->getValidMoves())) return true;
        }
    }
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
void Game::setPiece(int r, int k, SchaakStuk* s, bool deletePreviousPos)
{
    if(isBinnenGrens(r, k) && s != nullptr) {
        if(deletePreviousPos) removePiece(s->getPositie().first, s->getPositie().second);
        schaakBord[r][k] = s;
        s->setPositie({r, k}, *this);
    }
   else if(!isBinnenGrens(r, k)) throw std::invalid_argument("not inside chess boundary");
   else if(s == nullptr) throw std::invalid_argument("chess piece is a null pointer");
}
void Game::removePiece(int r, int k) {
    schaakBord[r][k] = nullptr;
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
MoveMatrix Game::getDiagonalMoves(std::pair<int, int> pos) const {
   Move current = {pos.first, pos.second};
    MoveVector topLeft, topRight, downLeft, downRight;
    int i = 1;
    while(i < 8) {
        topLeft.push_back({current.first-i, current.second-i});
        topRight.push_back({current.first-i, current.second+i});
        downLeft.push_back({current.first+i, current.second-i});
        downRight.push_back({current.first+i, current.second+i});
        i++;
    }
    return {topLeft, topRight, downLeft, downRight};
}
MoveMatrix Game::getHorizontalMoves(std::pair<int, int> pos) const {
    int currR = pos.first, currK = pos.second;
    MoveVector movesLeft, movesRight;
    int i = 1;
    while(i < 8) {
        if(currK+i < 8) movesRight.push_back({currR, currK+i});
        if(currK-i >= 0) movesLeft.push_back({currR, currK-i});
        i++;
    }
    return {movesLeft, movesRight};
}
std::vector<std::vector<std::pair<int ,int>>> Game::getVerticalMoves(std::pair<int, int> pos) const {
    int currR = pos.first, currK = pos.second;
    MoveVector movesUp, movesDown;
    int i = 1;
    while(i < 8) {
        if(currR+i < 8) movesDown.push_back({currR+i, currK});
        if(currR-i >= 0) movesUp.push_back({currR-i, currK});
        i++;
    }
    return {movesUp, movesDown};
}
MoveVector Game::getRadiusMoves(std::pair<int, int> pos, int radiusFactor) const {
    MoveVector moves;
   Move startingCoord = {pos.first-radiusFactor, pos.second-radiusFactor};
    for(int i=startingCoord.first;i<startingCoord.first+radiusFactor*2+1;i++) {
        for(int j=startingCoord.second;j<startingCoord.second+radiusFactor*2+1;j++) {
            if(i != pos.first || j != pos.second ) moves.push_back({i, j});
        }
    }
    return moves;
}Move Game::getMirrorX(std::pair<int, int> pos) const {
    return {7-pos.first, pos.second};
}Move Game::getMirrorY(std::pair<int, int> pos) const {
    return {pos.first, 7-pos.second};
}
MoveVector Game::filterBlockedMoves(MoveVector zetten, zw kleur) const {
    MoveVector geldige_zetten;
    for(Move currentMove : zetten) {
            int currentRow = currentMove.first, currentColumn = currentMove.second;
            if(!isBinnenGrens(currentRow, currentColumn) || hasPiece(currentRow, currentColumn)) {
                if(isBinnenGrens(currentRow, currentColumn) &&
                hasEnemyPiece(currentRow, currentColumn, kleur))
                    geldige_zetten.push_back({currentRow, currentColumn});
                break;
            }
            else geldige_zetten.push_back({currentRow, currentColumn});
    }
    return geldige_zetten;
}
MoveMatrix Game::filterBlockedMovesMatrix(MoveMatrix zetten, zw kleur) const {
    MoveMatrix geldige_zetten_matrix;
    for(MoveVector currentMovesArr : zetten) {
        geldige_zetten_matrix.push_back(filterBlockedMoves(currentMovesArr, kleur));
    }
    return geldige_zetten_matrix;
}
MoveVector Game::filterIndividualMoves(MoveVector zetten, zw kleur) const {
    MoveVector valid_moves;
    for(std::pair<int, int> currentMove : zetten) {
        if(isBinnenGrens(currentMove.first, currentMove.second) && !hasFriendlyPiece(currentMove.first, currentMove.second, kleur)) {
            valid_moves.push_back(currentMove);
        }
    }
    return valid_moves;
}

MoveVector Game::concatenateMoves(MoveMatrix movesMatrix) const {
    MoveVector moves;
    for(MoveVector currentMoveArray : movesMatrix) {
        moves.insert(moves.end(), currentMoveArray.begin(), currentMoveArray.end());
    }
    return moves;
}
MoveVector Game::dissolveMatrix(MoveMatrix matrix) const {
    MoveVector result;
    for(MoveVector currentVector : matrix) {
        result.insert(result.end(), currentVector.begin(), currentVector.end());
    }
    return result;
}
bool Game::validTurn(SchaakStuk *s) const {
    return currentTurn == s->getKleur();
}
bool Game::hasMove(int r, int k, MoveVector moves) const {
    for(Move currentMove : moves) {
        if(currentMove.first == r && currentMove.second == k) return true;
    }
    return false;
}
void Game::nextTurn() {
    currentTurn = (currentTurn == zwart) ? wit : zwart;
}
