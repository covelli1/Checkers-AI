from random import randint
from BoardClasses import Move
from BoardClasses import Board
from BoardClasses import InvalidMoveError

from copy import deepcopy
#The following part should be completed by students.
#Students can modify anything except the class name and exisiting functions and varibles.
class StudentAI():

    def __init__(self,col,row,p):
        self.col = col
        self.row = row
        self.p = p
        self.board = Board(col,row,p)
        self.board.initialize_game()
        self.color = ''
        self.opponent = {1:2,2:1}
        self.color = 2

        # self.temp_board = self.board
        # self.move_list = []

    #funtion that returns the 
    def evaluate(self, board2Eval):
        return board2Eval.white_count - board2Eval.black_count

    #given a color/player, return a list of all its possible moves
    def set_move_list(self, color):
        move_list = []
        moves = self.board.get_all_possible_moves(color)

        print("_____________________________")
        print(moves)
        print("_____________________________")


        index = 0
        inner_index = 0

        while index < len(moves):
            while(inner_index < len(moves[index])):
                move_list.append(moves[index][inner_index])
                inner_index += 1
            index += 1
            inner_index = 0

        return move_list

    #with depth implementation (curBoard = current board state, maxPlayer = who is currently moving)
    def minimax_depth(self, curBoard, depth, maxPlayer):

        #BASE CASE
        #if the depth is zero or there is a winner in the current board state,
        #we want to return (the difference in points, random move)
        #i'm returning a random move here in order to keep all the return types the same
        if depth == 0 or curBoard.is_win(maxPlayer):
            return self.evaluate(curBoard), None

        # if it's the current player's turn, they want to maximize the amount of points they can accrue
        if maxPlayer == self.color:
            maxEval = float('-inf')
            best_move = None

            # puts all the possible moves of the current player/color into a var all_moves
            all_moves = self.set_move_list(maxPlayer)

            # interate through all the possible moves of the current player/color
            for move in all_moves:

                # creates a deep copy of the board 
                # (this means anything changes made to this deepcopy
                # will not affect the original it was copied from)
                temp_board = deepcopy(curBoard)

                # this try loop filters out all impossible moves
                try:
                    temp_board.make_move(move, maxPlayer)
                    score = self.minimax_depth(temp_board, depth - 1, self.opponent[self.color])[0]
                    
                    maxEval = max(maxEval, score)

                    if maxEval == score:
                        best_move = move

                except InvalidMoveError:
                    pass
            return maxEval, best_move
           
        else:

            minEval = float('inf')
            best_move = None

            # puts all the possible moves of the current player/color into a var all_moves
            all_moves = self.set_move_list(maxPlayer)

            # interate through all the possible moves of the current player/color
            for move in all_moves:

                # creates a deep copy of the board 
                # (this means anything changes made to this deepcopy
                # will not affect the original it was copied from)
                temp_board = deepcopy(self.board)


                # this try loop filters out all impossible moves
                try: 
                    temp_board.make_move(move, maxPlayer)
                    score = self.minimax_depth(temp_board, depth - 1, self.opponent[self.color])[0]

                    minEval = min(minEval, score)

                    if minEval == score:
                        best_move = move

                except InvalidMoveError:
                    pass
            return minEval, best_move

    




    # #implement depth later
    # def minimax(self, colorNum):
    #     #for depth implementation later
    #     # if depth == 0 or self.temp_board.is_win(colorNum):
    #     #     return self.board.white_count - self.board.black_count


    #     # get all possible moves and put it into self.move_list
    #     moves = self.board.get_all_possible_moves(colorNum)


    #     index = 0
    #     inner_index = 0

    #     while index < len(moves):
    #         while(inner_index < len(moves[index])):
    #             self.move_list.append(moves[index][inner_index])
    #             inner_index += 1
    #         index += 1
    #         inner_index = 0   


    #     if colorNum == 1:
    #         # self.color = self.opponent[self.color]
    #         maxEval = float('-inf')
    #         best_move = None

    #         for move in self.move_list:
    #             temp_board = deepcopy(self.board)
    #             try:
    #                 temp_board.make_move(move, self.color)
    #                 score = temp_board.black_count - temp_board.white_count
                    
    #                 print("move " + str(move))
    #                 print("score " + str(score))
                    
    #                 maxEval = max(maxEval, score)

    #                 if maxEval == score:
    #                     best_move = move

    #             except InvalidMoveError:
    #                 pass
    #         print(best_move)
    #         return best_move
           
    #     else:

    #         Eval = float('-inf')
    #         best_move = None
    #         print(self.move_list)
    #         for move in self.move_list:
    #             temp_board = deepcopy(self.board)
    #             try: 
    #                 temp_board.make_move(move, self.color)
    #                 score = temp_board.white_count - temp_board.black_count

    #                 print("move " + str(move))
    #                 print("score " + str(score))

    #                 Eval = max(Eval, score)

    #                 if Eval == score:
    #                     best_move = move

    #             except InvalidMoveError:
    #                 pass
    #         print(best_move)
    #         return best_move

    
    # function that performs the most optimal(accourding to our 
    # evaluate function) move a returns said move
    def get_move(self,move):
        # if there is already a move in the var "move", we make that move for the 
        # opponent, otherwise set self.color to 1
        if len(move) != 0:
            self.board.make_move(move,self.opponent[self.color])
        else:
            self.color = 1

        # move = self.minimax(self.color)
        move = self.minimax_depth(self.board, 2, self.color)[1]
        self.board.make_move(move,self.color)
        return move
