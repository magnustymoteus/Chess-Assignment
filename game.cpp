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
Game Game::Clone() const {
    // clones the instance and returns it
    Game newGame = Game(currentTurn);
    newGame.setCurrentTurn(currentTurn);
    newGame.setKoningen(koningen);
    SchaakStuk* koning_wit = koningen[0]->Clone();
    SchaakStuk* koning_zwart = koningen[1]->Clone();
    newGame.setKoningen({koning_wit, koning_zwart});
    for(SchaakStuk* currentPiece : getStukken()) {
        int i = currentPiece->getPositie().first, j = currentPiece->getPositie().second;
        if(i == koningen[0]->getPositie().first && j == koningen[0]->getPositie().second) {
            newGame.pushToStukken(koning_wit);
            newGame.setSchaakBord(i, j, koning_wit);
        }
        else if(i == koningen[1]->getPositie().first && j == koningen[1]->getPositie().second) {
            newGame.pushToStukken(koning_zwart);
            newGame.setSchaakBord(i, j, koning_zwart);
        }
        else {
            SchaakStuk *sCopy = currentPiece->Clone();
            newGame.pushToStukken(sCopy);
            newGame.setSchaakBord(i, j, sCopy);
        }
    }
    return newGame;
}
Game::~Game() {}

void Game::printBord() const {
    //prints the current chess board for debugging purposes
    for(int i=0;i<schaakBord.size();i++) {
        for(int j=0;j<schaakBord[i].size();j++) {
            if(schaakBord[i][j] != nullptr) std::cout << "y ";
            else std::cout << "n ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
void Game::updateMoveThreats() {
    //updates all "red moves/squares" for all pieces
    for(SchaakStuk *currentPiece : stukken) {
        currentPiece->setThreatenedMoves(*this);
    }
}
void Game::updateAllPieces(const bool &filterCheckMoves) {
    //updates the game each turn by updating valid moves and threatened pieces ("red pieces")
    for(SchaakStuk *currentPiece : stukken) {
        currentPiece->setCanBeTaken(false);
        currentPiece->updateValidMoves(*this, filterCheckMoves);
    }
    updateMoveThreats();
    for(SchaakStuk *currentPiece : stukken) {
        for(Move currentMove : currentPiece->getValidMoves()) {
            if(hasEnemyPiece(currentMove.first, currentMove.second, currentPiece->getKleur()))
                getPiece(currentMove.first, currentMove.second)->setCanBeTaken(true);
        }
    }
}
MoveVector Game::getMoveIntersection(MoveVector zetten1, MoveVector zetten2) const {
    //gets the intersection of two vectors
    MoveVector intersection;
    for(Move currentMove1 : zetten1) {
        for(Move currentMove2 : zetten2) {
            if(currentMove1.first == currentMove2.first && currentMove1.second == currentMove2.second)
                intersection.push_back(currentMove1);
        }
    }
    return intersection;
}
void Game::setStukkenVector() {
    for(int i=0;i<8;i++) {
        for(int j=0;j<8;j++) {
            if(hasPiece(i, j)) stukken.push_back(getPiece(i, j));
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
    setStukkenVector();
    updateAllPieces();
}

// Verplaats stuk s naar positie (r,k)
// Als deze move niet mogelijk is, wordt false teruggegeven
// en verandert er niets aan het schaakbord.
// Anders wordt de move uitgevoerd en wordt true teruggegeven
bool Game::move(SchaakStuk* s, const int &r, const int &k) {
    //moves the piece if the move is valid
    if(s->isZetGeldig(r, k)) {
        setPiece(r,k,s, true);
        return true;
    }
    return false;
}

// Geeft true als kleur schaak staat
bool Game::schaak(zw kleur) {
    //checks if there is a check
    Move koningPos = koningen[kleur == zwart]->getPositie();
        for(SchaakStuk* currentPiece : stukken) {
            if(currentPiece->getKleur() != kleur &&
               hasMove(koningPos.first, koningPos.second,currentPiece->getValidMoves())) return true;
        }
    return false;
}

// Geeft true als kleur schaakmat staat
bool Game::schaakmat(zw kleur) {
    //checks if there is a checkmate
    bool isSchaak = schaak(kleur);
    for(SchaakStuk* currentPiece : stukken) {
        if(currentPiece->getKleur() == kleur && !currentPiece->getValidMoves().empty()) return false;
    }
    return isSchaak;
}

// Geeft true als kleur pat staat
// (pat = geen geldige zet mogelijk, maar kleur staat niet schaak;
// dit resulteert in een gelijkspel)
bool Game::pat(zw kleur) {
    //checks if there is a stalemate
    bool isSchaak = schaak(kleur);
    for(SchaakStuk* currentPiece : stukken) {
        if(currentPiece->getKleur() == kleur && !currentPiece->getValidMoves().empty()) return false;
    }
    return !isSchaak;
}

// Geeft een pointer naar het schaakstuk dat op rij r, kolom k staat
// Als er geen schaakstuk staat op deze positie, geef nullptr terug
SchaakStuk * Game::getPiece(const int &r, const int &k) const {
    //returns the piece on the chess board matrix.
    return schaakBord[r][k];
}

// Zet het schaakstuk waar s naar verwijst neer op rij r, kolom k.
// Als er al een schaakstuk staat, wordt het overschreven.
// Bewaar in jouw datastructuur de *pointer* naar het schaakstuk,
// niet het schaakstuk zelf.
void Game::setPiece(const int &r, const int &k, SchaakStuk* s, const bool &deletePreviousPos)
//sets the piece to a given coordinate, this is similar to move() but doesn't explicitly check if the move is valid.
{
    if(isBinnenGrens(r, k) && s != nullptr) {
        if(hasPiece(r, k)) {
        for(auto iter = stukken.begin();iter!=stukken.end();iter++) {
            if(((*iter)->getPositie().first == r) && ((*iter)->getPositie().second == k)) stukken.erase(iter);
        }}
        if(deletePreviousPos) removePiece(s->getPositie().first, s->getPositie().second);
        schaakBord[r][k] = s;
        s->setPositie({r, k});
    }
   else if(!isBinnenGrens(r, k)) throw std::invalid_argument("not inside chess boundary");
   else if(s == nullptr) throw std::invalid_argument("chess piece is a null pointer");
}
void Game::removePiece(const int &r, const int &k) {
    //removes a piece from the chess board matrix
    schaakBord[r][k] = nullptr;
}
bool Game::isBinnenGrens(const int &r, const int &k) const {
    //checks if the coordinate is within the chess board coordinates
    return ((0<=r && r<=7) && (0<=k && k<=7));
}
bool Game::hasFriendlyPiece(const int &r, const int &k, zw kleur) const {
    //checks if on the given coordinate there is a piece of the same color as kleur
    if(getPiece(r, k ) != nullptr) {
        SchaakStuk *s2 = getPiece(r, k);
        return (s2->getKleur() == kleur);
    }
    return false;
}
bool Game::hasEnemyPiece(const int &r, const int &k, zw kleur) const {
    //checks if on the given coordinate there is a piece of the opposite color as kleur
    if(getPiece(r, k ) != nullptr) {
        SchaakStuk *s2 = getPiece(r, k);
        return (s2->getKleur() != kleur);
    }
}
bool Game::hasPiece(const int &r, const int &k) const {
    //checks if the item on the given coordinate in the chess board matrix is not a nullptr
    SchaakStuk *s = getPiece(r, k);
    return (s != nullptr);
}
MoveMatrix Game::getDiagonalMoves(Move pos) const {
    //generates all possible diagonal moves (not necessarily valid, that would be the job of filter functions)
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
MoveMatrix Game::getHorizontalMoves(Move pos) const {
    //generates all possible horizontal moves (not necessarily valid, that would be the job of filter functions)
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
MoveMatrix Game::getVerticalMoves(Move pos) const {
    //generates all possible vertical moves (not necessarily valid, that would be the job of filter functions)
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
MoveVector Game::getRadiusMoves(Move pos, const int &radiusFactor) const {
    //generates all possible radius moves (not necessarily valid, that would be the job of filter functions)
    MoveVector moves;
   Move startingCoord = {pos.first-radiusFactor, pos.second-radiusFactor};
    for(int i=startingCoord.first;i<startingCoord.first+radiusFactor*2+1;i++) {
        for(int j=startingCoord.second;j<startingCoord.second+radiusFactor*2+1;j++) {
            if(i != pos.first || j != pos.second ) moves.push_back({i, j});
        }
    }
    return moves;
}
MoveVector Game::filterBlockedMoves(MoveVector zetten, zw kleur) const {
    //pushes moves to the vector until there is a piece in the way (includes that move if it is an enemy piece)
    //also removes moves that happen to be outside of boundary
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
    //uses filterBlockedMoves() on all vectors in the vector of those vectors
    MoveMatrix geldige_zetten_matrix;
    for(MoveVector currentMovesArr : zetten) {
        geldige_zetten_matrix.push_back(filterBlockedMoves(currentMovesArr, kleur));
    }
    return geldige_zetten_matrix;
}
MoveVector Game::filterIndividualMoves(MoveVector zetten, zw kleur) const {
    //pushes moves that are within boundary and have no friendly pieces (enemy or empty)
    MoveVector valid_moves;
    for(std::pair<int, int> currentMove : zetten) {
        if(isBinnenGrens(currentMove.first, currentMove.second) && !hasFriendlyPiece(currentMove.first, currentMove.second, kleur)) {
            valid_moves.push_back(currentMove);
        }
    }
    return valid_moves;
}

MoveVector Game::filterSelfCheckMoves(MoveVector zetten, Move position) const {
    /*clones a temporary copy of the current game instance (to retain const correctness) and filters the moves where
     * the king would be in check in that move*/
    MoveVector geldige_zetten;
    for(Move currentMove : zetten) {
        Game gameCpy = Clone();
        SchaakStuk* sCpy = gameCpy.getPiece(position.first, position.second);
        gameCpy.setPiece(currentMove.first, currentMove.second, sCpy, true);
        gameCpy.updateAllPieces(false);
        if(!gameCpy.schaak(sCpy->getKleur())) geldige_zetten.push_back(currentMove);
    }
    return geldige_zetten;
}

MoveVector Game::dissolveMatrix(MoveMatrix matrix) const {
    //dissolves a matrix of moves into a vector of moves
    MoveVector result;
    for(MoveVector currentVector : matrix) {
        result.insert(result.end(), currentVector.begin(), currentVector.end());
    }
    return result;
}
bool Game::validTurn(SchaakStuk *s) const {
    //returns whether it's the turn of the color of the given piece
    return currentTurn == s->getKleur();
}
bool Game::hasMove(const int &r, const int &k, MoveVector moves) const {
    //returns if the given vector of moves includes a certain move
    for(Move currentMove : moves) {
        if(currentMove.first == r && currentMove.second == k) return true;
    }
    return false;
}
void Game::nextTurn() {
    //switches turn
    currentTurn = (currentTurn == zwart) ? wit : zwart;
}
