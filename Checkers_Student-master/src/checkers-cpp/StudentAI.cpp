#include "StudentAI.h"
#include <random>


//The following part should be completed by students.
//The students can modify anything except the class name and exisiting functions and varibles.
StudentAI::StudentAI(int col,int row,int p)
	:AI(col, row, p)
{
    board = Board(col,row,p);
    board.initializeGame();
    player = 2;
}

Move StudentAI::GetMove(Move move)
{
    if (move.seq.empty())
    {
        player = 1;
    } else{
        board.makeMove(move,player == 1?2:1);
    }

    //if only one possible move
    if(board.getAllPossibleMoves(player).size()==1)
    {
        Move onlyMove = board.getAllPossibleMoves(player)[0][0];
        board.makeMove(onlyMove, player);
        return onlyMove;
    }

    MCTSTreeNode* startNode = new MCTSTreeNode(board,player);

    mctsTree =  MCTSTree(startNode,sqrt(2.0));

    // MCTSTreeNode* firstNode = mctsTree.selectNode();

    // ///testing individual functions
    // MCTSTreeNode* chosenChild = mctsTree.expandTree(firstNode);

//    double score = simulateNode(chosenChild);
//    backPropogate(chosenChild,score);
//
//    vector<MCTSTreeNode*> possibleNodes = root->getChildren();
//    MCTSTreeNode* bestNode = possibleNodes[0];
//
//    for(int i=1; i<possibleNodes.size();++i)
//    {
//        if( bestNode->getWinRatio() < possibleNodes[i]->getWinRatio())
//        {
//            bestNode=possibleNodes[i];
//        }
//    }
//
  ////Should be final code
//    currNode = bestNode;
//    return bestNode->getLastMove();
    // //cout << "before mctsSearch" << endl;
    Move result = mctsTree.MCTSsearch();
    // //cout << "after mctsSearch" << endl;
    board.makeMove(result,player);
    return result;



    // //////// chooses random move
    // vector<vector<Move>> moves = board.getAllPossibleMoves(player);
    // int i = rand() % (moves.size());
    // vector<Move> checker_moves = moves[i];
    // int j = rand() % (checker_moves.size());
    // Move res = checker_moves[j];
    // board.makeMove(res,player);

    // return res;

}

//Add better heuristic later for one look ahead
//int StudentAI::evaluate(Board board)
//{
//    return board.blackCount - board.whiteCount;
//}

////MCTSTree functions//////

//constructors
MCTSTree::MCTSTree()
        :root(nullptr) , currNode(nullptr)
{

}

MCTSTree::MCTSTree(MCTSTreeNode* startNode, double constVal)
        :root(startNode) , currNode(startNode), ucbConst(constVal)
{

}

///getter functions
MCTSTreeNode* MCTSTree::getRoot()
{
    return root;
}

MCTSTreeNode* MCTSTree::getCurrentNode()
{
    return currNode;
}

////
double MCTSTree::computeUCB(MCTSTreeNode *node)
{
    return (node->getNumOfWins() / (double)node->getNumOfVisits()) + ucbConst * sqrt( log( double(node->getParent()->getNumOfVisits())) /node->getNumOfVisits());
}

MCTSTreeNode* MCTSTree::findBestChild(MCTSTreeNode *parent)
{
        double temp = 0.0;
        vector < MCTSTreeNode * > children = parent->getChildren();
        MCTSTreeNode* bestChild = children[0];
        double maxUCT = computeUCB(children[0]);
        for (int i = 1; i < children.size(); ++i) {
            temp = computeUCB(children[i]);
            if(temp> maxUCT)
            {
                bestChild = children[i];
                maxUCT = temp;
            }
        }
        return bestChild;
}


///Selection
////checks at the current node if all possible moves are explored yet, if so pick best child till reaches leaf or gets to a node where unexplored moves available
MCTSTreeNode* MCTSTree::selectNode()
{
    MCTSTreeNode* chosenNode = currNode;

    while ( chosenNode->noExplorableMoves() && chosenNode->hasChildren() )
    {
        chosenNode = findBestChild(chosenNode);
    }
    return chosenNode;
}




////Expansion
MCTSTreeNode* MCTSTree::expandTree(MCTSTreeNode *node) {
    //cout<<"Expansion start \n";
    if (!node->noExplorableMoves()) //// if node has still unexplored moves pick one of them randomly
    {
//        //cout<<"Expansion unexplored moves available \n";
        vector<vector<Move>> moves = node->getUnexploredMoves();
        int i = rand() % (moves.size());
        vector<Move> checker_moves = moves[i];
        int j = rand() % (checker_moves.size());
        
        Move randomMove = checker_moves[j];
        
        int player = node->getPlayer();
        
    //        printf("moves.size(): %i \n", moves.size());
    //        printf("checker_moves.size(): %i \n", checker_moves.size());
    //        //cout<<"Expansion end \n";
        if(player == 1) {
            player = 2;
        }
        else {
            player = 1;
        }
        //cout<<"Expansion end \n";
        return node->addChildren(randomMove,player);
    }

    else if( node->hasChildren()) //// no unexplored nodes and has children, choose one to expand randomly
    {
        //cout<<"Expansion no unexplored moves available \n";
        vector<MCTSTreeNode*> allChildren = node->getChildren();
        int i = rand() % (allChildren.size());
        //cout<<"Expansion end \n";
        return allChildren[i];
    }
    else{
        //cout<<"First Expansion \n";
        //cout<<"First Expansion end \n";
        return node;
    }
}

////Rollout
double MCTSTree::simulateNode(MCTSTreeNode *node)
{
    double result=0.0;
    int winner=0;
    int ourPlayer = root->getPlayer();
    int currentPlayer = node->getPlayer();
    Board boardState = node->getBoardState();
    //int currMoveCount = boardState.saved_move_list.size();
    int currCount = 0;
    //cout << "simulateNode begin simulation" << endl;
    while(boardState.getAllPossibleMoves(currentPlayer).size() != 0 )
    {
        vector <vector<Move>> moves = boardState.getAllPossibleMoves(currentPlayer);
        int i = rand() % (moves.size());
        vector <Move> checker_moves = moves[i];
        int j = rand() % (checker_moves.size());
        Move randomMove = checker_moves[j];
        boardState.makeMove(randomMove,currentPlayer);
        ++currCount;
        currentPlayer = (currentPlayer == 1 ? 2:1);
    }
    //cout << "simulateNode end simulation" << endl;
    winner = boardState.isWin(node->getPlayer());

    //cout << "simulateNode begin simulation" << endl;
    while(currCount!=0) //undoes the moves made till reaches same move count as original
    {
        boardState.Undo();
        currCount--;
    }

    return (ourPlayer == winner || winner == 0 ? 1.0 : 0.0 ); // includes ties as wins
}

////Backpropogate
void MCTSTree:: backPropogate(MCTSTreeNode* node, double score)
{
    MCTSTreeNode* temp = node->getParent();
    node->updateNode(score);
    while(temp!= nullptr) //ends when reaches the root
    {
        temp->updateNode(score);
        temp = temp->getParent();
    }
}


//add for loop with timer
Move MCTSTree:: MCTSsearch()
{
    //ADD timer
    
    // builds MCTS Tree

    //begins clock timer
    unsigned long begTime;
    begTime = clock();

    unsigned long moveTimerReached = 1000000000000000000;

    for(int numSimulation = 0; numSimulation < 200; numSimulation++) {
        if((unsigned long) clock() - begTime >= moveTimerReached) {
            //cout << "entered timer if statement" << endl;
            break;
        }
        else {
            //cout << "Before selectNode" << endl;
            MCTSTreeNode* startNode = selectNode();
            //cout << "After selectNode" << endl;
            //
            MCTSTreeNode* chosenChild = expandTree(startNode);
           //cout << "Before simulateNode" << endl;
            double score = simulateNode(chosenChild);
           //cout << "After simulateNode" << endl;
           //cout << "Before backProp" << endl;
            backPropogate(chosenChild,score);
           //cout << "After backProp" << endl;
            // if(startNode->noExplorableMoves()) {
                //cout<<"Expansion no unexplored moves available \n";
            //     vector<MCTSTreeNode*> allChildren = startNode->getChildren();
            //     int i = rand() % (allChildren.size());
                //cout<<"Expansion end \n";
            //     currNode = allChildren[i];
            // }
        }
    }

    vector<MCTSTreeNode*> possibleNodes = root->getChildren();
    MCTSTreeNode* bestNode = possibleNodes[0];

    


    for(int i=1; i<possibleNodes.size();++i)
    {
        if( bestNode->getWinRatio() < possibleNodes[i]->getWinRatio())
        {
                bestNode=possibleNodes[i];
        }
        
    }

    currNode = bestNode;
    
    return bestNode->getLastMove();
   // return root->getBoardState().getAllPossibleMoves(root->getPlayer())[0][0];
}


/////MCTSTreeNode functions/////

MCTSTreeNode:: ~MCTSTreeNode()
{
//    //cout<<"Destructor node \n";
   deleteNode();
}

void MCTSTreeNode:: deleteNode()
{
    if( parent!= nullptr)
    {
        for(auto it = parent->children.begin(); it!= parent->children.end() ;++it ) {
            if (*it == this) {
                parent->children.erase(it);
                break;
            }
        }
    }

    for(int i=0; i<children.size(); ++i)
    {
      delete children[i];
    }
}


MCTSTreeNode:: MCTSTreeNode()
:parent(nullptr) , boardState(), numOfVisits(0),numOfWins(0.0),player(2),lastMove()
{
}

MCTSTreeNode::MCTSTreeNode(const Board& board, int player)
:parent(nullptr) , boardState(board), numOfVisits(0),numOfWins(0.0), player(player),lastMove()
{
    //boardState = helper::cloneBoard(board);
    unexploredMoves = boardState.getAllPossibleMoves(player);
}

double MCTSTreeNode:: getWinRatio() const
{
//    if(numOfVisits == 0)
//        return 0.0;
    return numOfWins/numOfVisits;
}

Move MCTSTreeNode:: getLastMove() const
{
    return lastMove;
}

///called by node to add a child
MCTSTreeNode* MCTSTreeNode:: addChildren( Move move, int player)
{
   //cout << "started running add children" << endl;
    Board childBoard = helper::cloneBoard(boardState);
    childBoard.makeMove(move,this->player);

    MCTSTreeNode* child = new MCTSTreeNode(childBoard,player);
    // if(player == 1) {
    //     player = 2;
    // }
    // else {
    //     player = 1;
    // }
    //child->boardState.makeMove(move,this->player);
    child->parent = this;
    child->lastMove = move;
    children.emplace_back(child);

    
    
    bool run = true;
// removes the move from unexplored moves
    for(auto checkersIt =unexploredMoves.begin(); checkersIt !=unexploredMoves.end() ; ++checkersIt  )
    {
        
        for(auto movesIt = checkersIt->begin(); movesIt!= checkersIt->end() ; ++movesIt  ) {
           
            if (helper::movesEqual(move,*movesIt)) {
                
                checkersIt->erase(movesIt);
                if(checkersIt->size() == 0) {
                    unexploredMoves.erase(checkersIt);
                    run = false;
                }
                break;
            }
        }
        if(!run) {
            break;
        }
    }
    
   //cout << "end running add children" << endl;
    return child;
}

void MCTSTreeNode:: updateNode(double value)
{
    numOfWins+=value;
    ++numOfVisits;
}

//returns true if no more explorable moves
bool MCTSTreeNode:: noExplorableMoves()
{
    return unexploredMoves.empty();
}

bool MCTSTreeNode:: hasChildren()
{
    return (!children.empty());
}


//getter methods

MCTSTreeNode* MCTSTreeNode:: getParent() const
{
    return parent;
}
vector<MCTSTreeNode*> MCTSTreeNode:: getChildren() const
{
    return children;
}
vector<vector<Move>> MCTSTreeNode:: getUnexploredMoves() const
{
    return unexploredMoves;
}

Board MCTSTreeNode:: getBoardState() const
{
    return boardState;
}

int MCTSTreeNode:: getNumOfVisits() const
{
    return numOfVisits;
}

int MCTSTreeNode:: getPlayer() const
{
    return player;
}

double MCTSTreeNode:: getNumOfWins() const
{
    return numOfWins;
}


///helper functions

bool helper::movesEqual( Move m1, Move m2)
{
    return m1.toString() == m2.toString();
}

Board helper:: cloneBoard(const Board& originalBoard)
{
 Board newBoard = Board(originalBoard.col,originalBoard.row,originalBoard.p);
 newBoard.blackCount = originalBoard.blackCount;
 newBoard.whiteCount = originalBoard.whiteCount;
 newBoard.tieCount = originalBoard.tieCount;
 newBoard.tieMax = originalBoard.tieMax;


 auto originalIt = originalBoard.board.begin();
 for(auto it = newBoard.board.begin()  ; it != newBoard.board.end() && originalIt != originalBoard.board.end(); ++it , ++originalIt)
 {
     it->assign(originalIt->begin(),originalIt->end());
 }

 newBoard.saved_move_list.insert(newBoard.saved_move_list.end(),originalBoard.saved_move_list.begin(),originalBoard.saved_move_list.end());

 return newBoard;
}