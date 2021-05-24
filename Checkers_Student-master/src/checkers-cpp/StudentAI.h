#ifndef STUDENTAI_H
#define STUDENTAI_H

#include "AI.h"
#include "Board.h"
#include "Checker.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>


#pragma once
using namespace std;

//The following part should be completed by students.
//Students can modify anything except the class name and exisiting functions and varibles.


namespace helper{
    bool movesEqual(  Move m1,    Move m2);
    Board cloneBoard(const Board& originalBoard);

}

class MCTSTreeNode {
public:
    MCTSTreeNode();
    MCTSTreeNode(const Board& board, int player);
    ~MCTSTreeNode();
    MCTSTreeNode* addChildren( Move move,int player);
    bool noExplorableMoves();
    bool hasChildren();
    void updateNode(double value); //updates the nodes visits and wins
    void deleteNode();

    //getter methods
    MCTSTreeNode* getParent() const;
    vector<MCTSTreeNode*> getChildren() const;
    vector<vector<Move>> getUnexploredMoves() const;
    Board getBoardState() const;
    int getNumOfVisits() const;
    int getPlayer() const;
    double getNumOfWins() const;
    double getWinRatio() const;
    Move getLastMove() const;

private:
    MCTSTreeNode* parent;
    vector <MCTSTreeNode*> children;
    vector<vector<Move>> unexploredMoves; //get from allPossibleMoves of board, and erase a move everytime a move is chosen
    Board boardState;
    int numOfVisits;
    int player;
    double numOfWins;
    Move lastMove; //move that leads to current board

};

class MCTSTree {
public:
    MCTSTree();
    MCTSTree(MCTSTreeNode* startNode, double constVal);
    MCTSTreeNode* findBestChild( MCTSTreeNode* parent);
    MCTSTreeNode* selectNode(); // returns best child of node passed in
    MCTSTreeNode* expandTree(MCTSTreeNode* node);
    double simulateNode(MCTSTreeNode* node); // simulates node and returns values based on simulation
    void backPropogate(MCTSTreeNode* node, double score); // goes back the tree and updates the values
    double computeUCB(MCTSTreeNode* node); //computes ucb value of node
    MCTSTreeNode* getRoot();
    MCTSTreeNode* getCurrentNode();
    Move MCTSsearch();

private:
    MCTSTreeNode* root; //root for tree
    MCTSTreeNode* currNode; // current node we are looking at in tree
    double ucbConst = sqrt(2.0);
};



class StudentAI :public AI
{
public:
    MCTSTree mctsTree;
    Board board;
    StudentAI(int col, int row, int p);
    virtual Move GetMove(Move board);
    //int evaluate( Board board);
};




#endif //STUDENTAI_H
