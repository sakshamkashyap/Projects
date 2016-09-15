import sys
import copy
from decimal import Decimal
pos_inf = Decimal('Infinity')
neg_inf = Decimal('-Infinity')

Grid = []
p1 = ""
p2 = ""
validGrid = []

text_file = open("next_state.txt", "w")



def win_func(i,j,p1,p2,Grid):
    count = 0
    if isValidMoves(i,j,Grid):
        if j-4 >= 0:
            if Grid[i][j-1] == p1 and Grid[i][j-2] == p1 and Grid[i][j-3] == p1 and Grid[i][j-4] == p1:
                count += 1
        if j-3 >=0 and j+1<=14:
            if Grid[i][j+1] == p1 and Grid[i][j-1] == p1 and Grid[i][j-2] == p1 and Grid[i][j-3] == p1:
                count += 1
        if j+2 <=14 and j-2 >= 0:
            if Grid[i][j+1] == p1 and Grid[i][j+2] == p1 and Grid[i][j-1] == p1 and Grid[i][j-2] == p1:
                count += 1
        if j+3 <= 14 and j-1 >= 0:
            if Grid[i][j + 1] == p1 and Grid[i][j + 2] == p1 and Grid[i][j + 3] == p1 and Grid[i][j -1] == p1:
                count += 1
        if j+4 <= 14:
            if Grid[i][j + 1] == p1 and Grid[i][j + 2] == p1 and Grid[i][j + 3] == p1 and Grid[i][j + 4] == p1:
                count += 1

        if i-4>=0:
            if Grid[i-1][j] == p1 and Grid[i-2][j] == p1 and Grid[i-3][j] == p1 and Grid[i-4][j] == p1:
                count += 1
        if i - 3 >= 0 and i + 1 <= 14:
            if Grid[i+1][j] == p1 and Grid[i-1][j] == p1 and Grid[i-2][j] == p1 and Grid[i-3][j] == p1:
                count += 1
        if i + 2 <= 14 and i - 2 >= 0:
            if Grid[i+1][j] == p1 and Grid[i+2][j] == p1 and Grid[i-1][j] == p1 and Grid[i-2][j] == p1:
                count += 1
        if i + 3 <= 14 and i - 1 >= 0:
            if Grid[i+1][j] == p1 and Grid[i+2][j] == p1 and Grid[i+3][j] == p1 and Grid[i-1][j] == p1:
                count += 1
        if i + 4 <= 14:
            if Grid[i+1][j] == p1 and Grid[i+2][j] == p1 and Grid[i+3][j] == p1 and Grid[i+4][j] == p1:
                count += 1

        if i-4 >= 0 and j-4>=0:
            if Grid[i-1][j - 1] == p1 and Grid[i-2][j - 2] == p1 and Grid[i-3][j - 3] == p1 and Grid[i-4][j - 4] == p1:
                count += 1
        if i+1 <= 14 and j+1 <= 14 and i-3>=0 and j-3>=0:
            if Grid[i+1][j + 1] == p1 and Grid[i-1][j - 1] == p1 and Grid[i-2][j - 2] == p1 and Grid[i-3][j - 3] == p1:
                count += 1
        if i+2<=14 and j+2<=14 and i-2>=0 and j-2>=0:
            if Grid[i+1][j + 1] == p1 and Grid[i+2][j + 2] == p1 and Grid[i-1][j - 1] == p1 and Grid[i-2][j - 2] == p1:
                count += 1
        if i+3 <= 14 and j+3<=14 and i-1>=0 and j-1>=0:
            if Grid[i+1][j + 1] == p1 and Grid[i+2][j + 2] == p1 and Grid[i+3][j + 3] == p1 and Grid[i-1][j - 1] == p1:
                count += 1
        if j+4 <=14 and i+4<=14:
            if Grid[i+1][j + 1] == p1 and Grid[i+2][j + 2] == p1 and Grid[i+3][j + 3] == p1 and Grid[i+4][j + 4] == p1:
                count += 1
        if i+4 <=14 and j-4 >=0:
            if Grid[i + 1][j - 1] == p1 and Grid[i + 2][j - 2] == p1 and Grid[i + 3][j - 3] == p1 and Grid[i + 4][j - 4] == p1:
                count += 1
        if i+3 <=14 and i-1>=0 and j-3 >= 0 and j+1 <=14:
            if Grid[i - 1][j + 1] == p1 and Grid[i + 1][j - 1] == p1 and Grid[i + 2][j - 2] == p1 and Grid[i + 3][j - 3] == p1:
                count += 1
        if i+2<=14 and i-2>=0 and j+2<=14 and j-2>=0:
            if Grid[i - 1][j + 1] == p1 and Grid[i - 2][j + 2] == p1 and Grid[i + 1][j - 1] == p1 and Grid[i + 2][j - 2] == p1:
                count += 1
        if i-3 >=0 and i+3 <= 14 and j-1 >= 0 and j+3 <=14:
            if Grid[i - 1][j + 1] == p1 and Grid[i - 2][j + 2] == p1 and Grid[i - 3][j + 3] == p1 and Grid[i + 1][j - 1] == p1:
                count += 1
        if i-4 >=0 and j+4 <=14:
            if Grid[i - 1][j + 1] == p1 and Grid[i - 2][j + 2] == p1 and Grid[i - 3][j + 3] == p1 and Grid[i - 4][j + 4] == p1:
                count += 1

    return count

def blockClosedFour_func(i,j,p1,p2,Grid):
    count = 0
    if Grid[i][j] == ".":
        if j - 5 >= 0:
            if Grid[i][j - 1] == p2 and Grid[i][j - 2] == p2 and Grid[i][j - 3] == p2 and Grid[i][j - 4] == p2 and Grid[i][j-5] == p1:
                count += 1
        if j+5 <= 14:
            if Grid[i][j + 1] == p2 and Grid[i][j + 2] == p2 and Grid[i][j + 3] == p2 and Grid[i][j + 4] == p2 and Grid[i][j+5] == p1:
                count += 1
        if i-5 >=0:
            if Grid[i-1][j] == p2 and Grid[i-2][j] == p2 and Grid[i-3][j] == p2 and Grid[i-4][j] == p2 and Grid[i-5][j] == p1:
                count += 1
        if i+5 <=14:
            if Grid[i+1][j] == p2 and Grid[i+2][j] == p2 and Grid[i+3][j] == p2 and Grid[i+4][j] == p2 and Grid[i+5][j] == p1:
                count += 1
        if i+5 <= 14 and j+5 <=14:
            if Grid[i+1][j + 1] == p2 and Grid[i+2][j + 2] == p2 and Grid[i+3][j + 3] == p2 and Grid[i+4][j + 4] == p2 and Grid[i+5][j+5] == p1:
                count += 1
        if i-5 >=0 and j-5 >=0:
            if Grid[i - 1][j - 1] == p2 and Grid[i - 2][j - 2] == p2 and Grid[i - 3][j - 3] == p2 and Grid[i - 4][j - 4] == p2 and Grid[i - 5][j - 5] == p1:
                count += 1
        if i+5 <= 14 and j-5>=0:
            if Grid[i + 1][j - 1] == p2 and Grid[i + 2][j - 2] == p2 and Grid[i + 3][j - 3] == p2 and Grid[i + 4][j - 4] == p2 and Grid[i + 5][j - 5] == p1:
                count += 1
        if j+5 <= 14 and i-5 >=0:
            if Grid[i - 1][j + 1] == p2 and Grid[i - 2][j + 2] == p2 and Grid[i - 3][j + 3] == p2 and Grid[i - 4][j + 4] == p2 and Grid[i - 5][j + 5] == p1:
                count += 1

    return count

def createOpenFour_func(i,j,p1,p2,Grid):
    count = 0
    if Grid[i][j] == ".":
        if j-4 >=0 and j+1 <=14:
            if Grid[i][j-1] == p1 and Grid[i][j-2] == p1 and Grid[i][j-3] == p1 and Grid[i][j-4] != p2 and Grid[i][j+1] != p2:
                count += 1
        if j-3 >=0 and j+2 <= 14:
            if Grid[i][j - 1] == p1 and Grid[i][j - 2] == p1 and Grid[i][j + 1] == p1 and Grid[i][j - 3] != p2 and Grid[i][j+2] != p2:
                count += 1
        if j-2 >=0 and j+3<=14:
            if Grid[i][j - 1] == p1 and Grid[i][j + 1] == p1 and Grid[i][j + 2] == p1 and Grid[i][j - 2] != p2 and Grid[i][j + 3] != p2:
                count += 1
        if j+4 <= 14 and j-1 >= 0:
            if Grid[i][j + 1] == p1 and Grid[i][j + 2] == p1 and Grid[i][j + 3] == p1 and Grid[i][j + 4] != p2 and Grid[i][j - 1] != p2:
                count += 1
        if i-4 >= 0 and i+1<=14:
            if Grid[i-1][j] == p1 and Grid[i-2][j] == p1 and Grid[i-3][j] == p1 and Grid[i-4][j] != p2 and Grid[i+1][j] != p2:
                count += 1
        if i-3 >=0 and i+2<=14:
            if Grid[i-1][j] == p1 and Grid[i-2][j] == p1 and Grid[i+1][j] == p1 and Grid[i-3][j] != p2 and Grid[i+2][j] != p2:
                count += 1
        if i-2 >=0 and i+3 <=14:
            if Grid[i-1][j] == p1 and Grid[i+1][j] == p1 and Grid[i+2][j] == p1 and Grid[i-2][j] != p2 and Grid[i+3][j] != p2:
                count += 1
        if i+4<=14 and i-1>=0:
            if Grid[i+1][j] == p1 and Grid[i+2][j] == p1 and Grid[i+3][j] == p1 and Grid[i+4][j] != p2 and Grid[i-1][j] != p2:
                count += 1

        if i-4 >=0 and j-4>=0 and i+1<=14 and j+1<=14:
            if Grid[i - 1][j-1] == p1 and Grid[i - 2][j-2] == p1 and Grid[i - 3][j-3] == p1 and Grid[i - 4][j-4] != p2 and Grid[i + 1][j+1] != p2:
                count += 1
        if i-3>=0 and j-3>=0 and i+2 <=14 and j+2<=14:
            if Grid[i - 1][j-1] == p1 and Grid[i - 2][j-2] == p1 and Grid[i + 1][j+1] == p1 and Grid[i - 3][j-3] != p2 and Grid[i + 2][j+2] != p2:
                count += 1
        if i-2 >=0 and j-2>=0 and i+3 <=14 and j+3<=14:
            if Grid[i - 1][j-1] == p1 and Grid[i + 1][j+1] == p1 and Grid[i + 2][j+2] == p1 and Grid[i - 2][j-2] != p2 and Grid[i + 3][j+3] != p2:
                count += 1
        if i+4<=14 and j+4<=14 and i-1 >=0and j-1>=0:
            if Grid[i + 1][j+1] == p1 and Grid[i + 2][j+2] == p1 and Grid[i + 3][j+3] == p1 and Grid[i + 4][j+4] != p2 and Grid[i - 1][j-1] != p2:
                count += 1
        if i+4<=14 and j-4 >=0 and i-1>=0 and j+1 <=14:
            if Grid[i + 1][j - 1] == p1 and Grid[i + 2][j - 2] == p1 and Grid[i + 3][j - 3] == p1 and Grid[i + 4][j - 4] != p2 and Grid[i - 1][j + 1] != p2:
                count += 1
        if i+3 <= 14 and i-2 >=0 and j-3>=0 and j+2<=14:
            if Grid[i + 1][j - 1] == p1 and Grid[i + 2][j - 2] == p1 and Grid[i - 1][j + 1] == p1 and Grid[i + 3][j - 3] != p2 and Grid[i - 2][j + 2] != p2:
                count += 1
        if i-3>=0 and i+2<=14 and j+3 <=14 and j-2>=0:
            if Grid[i + 1][j - 1] == p1 and Grid[i - 1][j + 1] == p1 and Grid[i - 2][j + 2] == p1 and Grid[i + 2][j - 2] != p2 and Grid[i - 3][j + 3] != p2:
                count += 1
        if i-4 >= 0 and i+1 <= 14 and j+4 <=14 and j-1>=0:
            if Grid[i - 1][j + 1] == p1 and Grid[i - 2][j + 2] == p1 and Grid[i - 3][j + 3] == p1 and Grid[i - 4][j + 4] != p2 and Grid[i + 1][j - 1] != p2:
                count += 1
    return count

def createClosedFour_func(i,j,p1,p2,Grid):
    count = 0
    if Grid[i][j] == ".":
        if j-4 >=0 and j+1 <=14:
            if Grid[i][j-1] == p1 and Grid[i][j-2] == p1 and Grid[i][j-3] == p1 and Grid[i][j-4] == p2 and Grid[i][j+1] == ".":
                count += 1
        if j-3 >=0 and j+2<=14:
            if Grid[i][j - 1] == p1 and Grid[i][j - 2] == p1 and Grid[i][j + 1] == p1 and Grid[i][j - 3] == p2 and Grid[i][j + 2] == ".":
                count += 1
        if j-2 >=0 and j+3 <=14:
            if Grid[i][j - 1] == p1 and Grid[i][j + 1] == p1 and Grid[i][j + 2] == p1 and Grid[i][j - 2] == p2 and Grid[i][j + 3] == ".":
                count += 1
        if j+4<=14 and j-1>=0:
            if Grid[i][j + 1] == p1 and Grid[i][j + 2] == p1 and Grid[i][j + 3] == p1 and Grid[i][j - 1] == p2 and Grid[i][j + 4] == ".":
                count += 1
        if i-4>=0 and i+1<=14:
            if Grid[i-1][j] == p1 and Grid[i-2][j] == p1 and Grid[i-3][j] == p1 and Grid[i-4][j] == p2 and Grid[i+1][j] == ".":
                count += 1
        if i-3>=0 and i+2 <=14:
            if Grid[i-1][j] == p1 and Grid[i-2][j] == p1 and Grid[i+1][j] == p1 and Grid[i-3][j] == p2 and Grid[i+2][j] == ".":
                count += 1
        if i-2 >=0 and i+3<=14:
            if Grid[i-1][j] == p1 and Grid[i+1][j] == p1 and Grid[i+2][j] == p1 and Grid[i-2][j] == p2 and Grid[i+3][j] == ".":
                count += 1
        if i+4 <=14 and i-1>=0:
            if Grid[i+1][j] == p1 and Grid[i+2][j] == p1 and Grid[i+3][j] == p1 and Grid[i-1][j] == p2 and Grid[i+4][j] == ".":
                count += 1

        if i-4>=0 and j-4>=0 and i+1<=14 and j+1<=14:
            if Grid[i-1][j - 1] == p1 and Grid[i-2][j - 2] == p1 and Grid[i-3][j - 3] == p1 and Grid[i-4][j - 4] == p2 and Grid[i+1][j + 1] == ".":
                count += 1
        if i-3 >=0 and j-3>=0 and i+2 <=14 and j+2<=14:
            if Grid[i-1][j - 1] == p1 and Grid[i-2][j - 2] == p1 and Grid[i+1][j + 1] == p1 and Grid[i-3][j - 3] == p2 and Grid[i+2][j + 2] == ".":
                count += 1
        if i-2 >=0 and j-2 >=0 and i+3 <=14 and j+3<=14:
            if Grid[i-1][j - 1] == p1 and Grid[i+1][j + 1] == p1 and Grid[i+2][j + 2] == p1 and Grid[i-2][j - 2] == p2 and Grid[i+3][j + 3] == ".":
                count += 1
        if i+4 <=14 and j+4<=14 and i-1>=0 and j-1>=0:
            if Grid[i+1][j + 1] == p1 and Grid[i+2][j + 2] == p1 and Grid[i+3][j + 3] == p1 and Grid[i-1][j - 1] == p2 and Grid[i+4][j + 4] == ".":
                count += 1
        if i+4 <=14 and i-1>= 0 and j+1<=14 and j-4>=0:
            if Grid[i + 1][j - 1] == p1 and Grid[i + 2][j - 2] == p1 and Grid[i + 3][j - 3] == p1 and Grid[i + 4][j - 4] == p2 and Grid[i - 1][j + 1] == ".":
                count += 1
        if i+3 <=14 and i-2 >=0 and j-3>=0 and j+2 <=14:
            if Grid[i + 1][j - 1] == p1 and Grid[i + 2][j - 2] == p1 and Grid[i - 1][j + 1] == p1 and Grid[i + 3][j - 3] == p2 and Grid[i - 2][j + 2] == ".":
                count += 1
        if i+2<=14 and i-3>=0 and j+3 <=14 and j-2>=0:
            if Grid[i + 1][j - 1] == p1 and Grid[i - 1][j + 1] == p1 and Grid[i - 2][j + 2] == p1 and Grid[i + 2][j - 2] == p2 and Grid[i - 3][j + 3] == ".":
                count += 1
        if i-4 >=0 and j+4 <=14 and i+1 <=14 and j-1>=0:
            if Grid[i -1][j + 1] == p1 and Grid[i-2][j + 2] == p1 and Grid[i-3][j + 3] == p1 and Grid[i+1][j - 1] == p2 and Grid[i-4][j + 4] == ".":
                count += 1

    return count

def blockOpenThree_func(i,j,p1,p2,Grid):
    count = 0
    if Grid[i][j] == ".":
        if j-4>=0:
            if Grid[i][j-1] == p2 and Grid[i][j-2] == p2 and Grid[i][j-3] == p2 and Grid[i][j-4] == ".":
                count += 1
        if j+4<=14:
            if Grid[i][j + 1] == p2 and Grid[i][j + 2] == p2 and Grid[i][j + 3] == p2 and Grid[i][j + 4] == ".":
                count += 1
        if i-4 >=0:
            if Grid[i-1][j] == p2 and Grid[i-2][j] == p2 and Grid[i-3][j] == p2 and Grid[i-4][j] == ".":
                count += 1
        if i+4 <=14:
            if Grid[i + 1][j] == p2 and Grid[i + 2][j] == p2 and Grid[i + 3][j] == p2 and Grid[i + 4][j] == ".":
                count += 1
        if i-4 >=0 and j-4 >=0:
            if Grid[i - 1][j-1] == p2 and Grid[i - 2][j-2] == p2 and Grid[i - 3][j-3] == p2 and Grid[i - 4][j-4] == ".":
                count += 1
        if i+4 <=14 and j+4 <=14:
            if Grid[i + 1][j + 1] == p2 and Grid[i + 2][j + 2] == p2 and Grid[i + 3][j + 3] == p2 and Grid[i + 4][j + 4] == ".":
                count += 1
        if i+4 <=14 and j-4 >=0:
            if Grid[i + 1][j - 1] == p2 and Grid[i + 2][j - 2] == p2 and Grid[i + 3][j - 3] == p2 and Grid[i + 4][j - 4] == ".":
                count += 1
        if i-4 >=0 and j+4 <=14:
            if Grid[i - 1][j + 1] == p2 and Grid[i - 2][j + 2] == p2 and Grid[i - 3][j + 3] == p2 and Grid[i - 4][j + 4] == ".":
                count += 1
    return count


def blockClosedThree_func(i, j,p1,p2,Grid):
    count = 0
    if Grid[i][j] == ".":
        if j-4 >=0:
            if Grid[i][j - 1] == p2 and Grid[i][j - 2] == p2 and Grid[i][j - 3] == p2 and Grid[i][j - 4] == p1:
                count += 1
        if i-4 >=0:
            if Grid[i - 1][j] ==p2 and Grid[i - 2][j] ==p2 and Grid[i - 3][j] == p2 and Grid[i - 4][j] == p1:
                count += 1
        if j+4<=14:
            if Grid[i][j + 1] == p2 and Grid[i][j + 2] == p2 and Grid[i][j + 3] == p2 and Grid[i][j + 4] == p1:
                count += 1
        if i+4 <=14:
            if Grid[i + 1][j] == p2 and Grid[i + 2][j] == p2 and Grid[i + 3][j] == p2 and Grid[i + 4][j] == p1:
                count += 1

        if i-4 >=0 and j-4>=0:
            if Grid[i - 1][j - 1] == p2 and Grid[i - 2][j - 2] == p2 and Grid[i - 3][j - 3] == p2 and Grid[i - 4][j - 4] == p1:
                count += 1
        if i-4 >=0 and j+4<=0:
            if Grid[i - 1][j + 1] == p2 and Grid[i - 2][j + 2] == p2 and Grid[i - 3][j + 3] == p2 and Grid[i - 4][j + 4] == p1:
                count += 1
        if i+4 <=14 and j+4 <=14:
            if Grid[i + 1][j + 1] == p2 and Grid[i + 2][j + 2] == p2 and Grid[i + 3][j + 3] == p2 and Grid[i + 4][j + 4] == p1:
                count += 1
        if i+4 <=14 and j-4>=0:
            if Grid[i + 1][j - 1] == p2 and Grid[i + 2][j - 2] == p2 and Grid[i + 3][j - 3] == p2 and Grid[i + 4][j - 4] == p1:
                count += 1

    return count



def createOpenThree_func(i,j,p1,p2,Grid):
    count = 0
    if Grid[i][j] == ".":
        if j-3 >=0 and j+1 <=14:
            if Grid[i][j-1] == p1 and Grid[i][j-2] == p1 and Grid[i][j-3] == "." and Grid[i][j+1] == ".":
                count += 1
        if j-2>=0 and j+2<=14:
            if Grid[i][j-1] == p1 and Grid[i][j+1] == p1 and Grid[i][j-2] == "." and Grid[i][j+2] == ".":
                count += 1
        if j+3<=14 and j-1 >=0:
            if Grid[i][j+1] == p1 and Grid[i][j+2] == p1 and Grid[i][j-1] == "." and Grid[i][j+3] == ".":
                count += 1
        if i-3 >=0 and i+1<=14:
            if Grid[i-1][j] == p1 and Grid[i-2][j] == p1 and Grid[i-3][j] == "." and Grid[i+1][j] == ".":
                count += 1
        if i-2 >=0 and i+2<=14:
            if Grid[i-1][j] == p1 and Grid[i+1][j] == p1 and Grid[i-2][j] == "." and Grid[i+2][j] == ".":
                count += 1
        if i+3 <=14 and i-1 >=0:
            if Grid[i+1][j] == p1 and Grid[i+2][j] == p1 and Grid[i-1][j] == "." and Grid[i+3][j] == ".":
                count += 1
        if i-3 >=0 and i+1 <=14 and j-3 >=0 and j+1<=14:
            if Grid[i - 1][j-1] == p1 and Grid[i - 2][j-2] == p1 and Grid[i - 3][j-3] == "." and Grid[i + 1][j+1] == ".":
                count += 1
        if i-2>=0 and j-2 >=0 and i+2 <=14 and j+2<=14:
            if Grid[i - 1][j-1] == p1 and Grid[i + 1][j+1] == p1 and Grid[i - 2][j-2] == "." and Grid[i + 2][j+2] == ".":
                count += 1
        if i+3<=14 and j+3<=14 and i-1>=0 and j-1>=0:
            if Grid[i + 1][j+1] == p1 and Grid[i + 2][j+2] == p1 and Grid[i - 1][j-1] == "." and Grid[i + 3][j+3] == ".":
                count += 1
        if i-3>=0 and i+1<=14 and j+3<=14 and j-1>=0:
            if Grid[i - 1][j+1] == p1 and Grid[i - 2][j+2] == p1 and Grid[i - 3][j+3] == "." and Grid[i + 1][j-1] == ".":
                count += 1
        if i-1 >=0 and j+2 <=14 and i+2 <=14 and j-2 >=0:
            if Grid[i - 1][j+1] == p1 and Grid[i + 1][j-1] == p1 and Grid[i - 2][j+2] == "." and Grid[i + 2][j-2] == ".":
                count += 1
        if i+3 <=14 and j-3 >=0 and i-1 >=0 and j+1<=14:
            if Grid[i + 1][j-1] == p1 and Grid[i + 2][j-2] == p1 and Grid[i - 1][j+1] == "." and Grid[i + 3][j-3] == ".":
                count += 1
    return count

def createClosedThree_func(i,j,p1,p2,Grid):
    count = 0
    if Grid[i][j] ==".":
        if j - 3 >= 0 and j + 1 <= 14:
            if Grid[i][j-1] == p1 and Grid[i][j-2] == p1 and Grid[i][j-3] == p2 and Grid[i][j+1] == ".":
                count += 1
            if Grid[i][j - 1] == p1 and Grid[i][j - 2] == p1 and Grid[i][j - 3] == "." and Grid[i][j + 1] == p2:
                count += 1
        if j - 2 >= 0 and j + 2 <= 14:
            if Grid[i][j - 1] == p1 and Grid[i][j + 1] == p1 and Grid[i][j - 2] == p2 and Grid[i][j + 2] == ".":
                count += 1
            if Grid[i][j - 1] == p1 and Grid[i][j + 1] == p1 and Grid[i][j - 2] == "." and Grid[i][j + 2] == p2:
                count += 1
        if j - 1 >= 0 and j + 3 <= 14:
            if Grid[i][j + 1] == p1 and Grid[i][j + 2] == p1 and Grid[i][j - 1] == p2 and Grid[i][j + 3] == ".":
                count += 1
            if Grid[i][j + 1] == p1 and Grid[i][j + 2] == p1 and Grid[i][j - 1] == "." and Grid[i][j + 3] == p2:
                count += 1
        if i - 3 >= 0 and i + 1 <= 14:
            if Grid[i-1][j] == p1 and Grid[i-2][j] == p1 and Grid[i-3][j] == p2 and Grid[i+1][j] == ".":
                count += 1
            if Grid[i - 1][j] == p1 and Grid[i - 2][j] == p1 and Grid[i - 3][j] == "." and Grid[i + 1][j] == p2:
                count += 1
        if i - 2 >= 0 and i + 2 <= 14:
            if Grid[i-1][j] == p1 and Grid[i+1][j] == p1 and Grid[i-2][j] == p2 and Grid[i+2][j] == ".":
                count += 1
            if Grid[i - 1][j] == p1 and Grid[i + 1][j] == p1 and Grid[i - 2][j] == "." and Grid[i + 2][j] == p2:
                count += 1
        if i - 1 >= 0 and i + 3 <= 14:
            if Grid[i+1][j] == p1 and Grid[i+2][j] == p1 and Grid[i-1][j] == p2 and Grid[i+3][j] == ".":
                count += 1
            if Grid[i+1][j] == p1 and Grid[i+2][j] == p1 and Grid[i-1][j] == "." and Grid[i+3][j] == p2:
                count += 1
        if i - 3 >= 0 and j - 3 >= 0 and i + 1 <= 14 and j + 1 <= 14:
            if Grid[i - 1][j-1] == p1 and Grid[i - 2][j-2] == p1 and Grid[i - 3][j-3] == p2 and Grid[i + 1][j+1] == ".":
                count += 1
            if Grid[i - 1][j - 1] == p1 and Grid[i - 2][j - 2] == p1 and Grid[i - 3][j - 3] == "." and Grid[i + 1][
                        j + 1] == p2:
                count += 1
        if i - 2 >= 0 and j - 2 >= 0 and i + 2 <= 14 and j + 2 <= 14:
            if Grid[i - 1][j-1] == p1 and Grid[i + 1][j+1] == p1 and Grid[i - 2][j-2] == p2 and Grid[i + 2][j+2] == ".":
                count += 1
            if Grid[i - 1][j - 1] == p1 and Grid[i + 1][j + 1] == p1 and Grid[i - 2][j - 2] == "." and Grid[i + 2][
                        j + 2] == p2:
                count += 1
            if Grid[i - 1][j + 1] == p1 and Grid[i + 1][j - 1] == p1 and Grid[i - 2][j + 2] == p2 and Grid[i + 2][
                        j - 2] == ".":
                count += 1
            if Grid[i - 1][j + 1] == p1 and Grid[i + 1][j - 1] == p1 and Grid[i - 2][j + 2] == "." and Grid[i + 2][
                        j - 2] == p2:
                count += 1
        if i - 1 >= 0 and j - 1 >= 0 and i + 3 <= 14 and j + 3 <= 14:
            if Grid[i + 1][j+1] == p1 and Grid[i + 2][j+2] == p1 and Grid[i - 1][j-1] == p2 and Grid[i + 3][j+3] == ".":
                count += 1
            if Grid[i + 1][j+1] == p1 and Grid[i + 2][j+2] == p1 and Grid[i - 1][j-1] == "." and Grid[i + 3][j+3] == p2:
                count += 1
        if i - 3 >= 0 and j - 1 >= 0 and i + 1 <= 14 and j + 3 <= 14:
            if Grid[i - 1][j+1] == p1 and Grid[i - 2][j+2] == p1 and Grid[i - 3][j+3] == p2 and Grid[i + 1][j-1] == ".":
                count += 1
            if Grid[i - 1][j + 1] == p1 and Grid[i - 2][j + 2] == p1 and Grid[i - 3][j + 3] == "." and Grid[i + 1][
                        j - 1] == p2:
                count += 1
        if i - 1 >= 0 and j - 3 >= 0 and i + 3 <= 14 and j + 1 <= 14:
            if Grid[i + 1][j-1] == p1 and Grid[i + 2][j-2] == p1 and Grid[i - 1][j+1] == p2 and Grid[i + 3][j-3] == ".":
                count += 1
            if Grid[i + 1][j-1] == p1 and Grid[i + 2][j-2] == p1 and Grid[i - 1][j-1] == "." and Grid[i + 3][j-3] == p2:
                count += 1

    return count


def createOpenTwo_func(i,j,p1,p2,Grid):
    count = 0
    if Grid[i][j] ==".":
        if j - 2 >= 0 and j + 1 <= 14:
            if Grid[i][j-1] == p1 and Grid[i][j-2] == "." and Grid[i][j+1] == ".":
                count += 1
        if j - 1 >= 0 and j + 2 <= 14:
            if Grid[i][j+1] == p1 and Grid[i][j+2] == "." and Grid[i][j-1] == ".":
                count += 1
        if i - 2 >= 0 and i + 1 <= 14:
            if Grid[i-1][j] == p1 and Grid[i-2][j] == "." and Grid[i+1][j] == ".":
                count += 1
        if i - 1 >= 0 and i + 2 <= 14:
            if Grid[i+1][j] == p1 and Grid[i+2][j] == "." and Grid[i-1][j] == ".":
                count += 1
        if i - 2 >= 0 and j - 2 >= 0 and i + 1 <= 14 and j + 1 <= 14:
            if Grid[i - 1][j-1] == p1 and Grid[i - 2][j-2] == "." and Grid[i + 1][j+1] == ".":
                count += 1
        if i - 1 >= 0 and j - 1 >= 0 and i + 2 <= 14 and j + 2 <= 14:
            if Grid[i + 1][j+1] == p1 and Grid[i + 2][j+2] == "." and Grid[i - 1][j-1] == ".":
                count += 1
        if i - 2 >= 0 and j - 1 >= 0 and i + 1 <= 14 and j + 2 <= 14:
            if Grid[i - 1][j+1] == p1 and Grid[i - 2][j+2] == "." and Grid[i + 1][j-1] == ".":
                count += 1
        if i - 1 >= 0 and j - 2 >= 0 and i + 2 <= 14 and j + 1 <= 14:
            if Grid[i + 1][j-1] == p1 and Grid[i + 2][j-2] == "." and Grid[i - 1][j+1] == ".":
                count += 1

    return count

def createClosedTwo_func(i,j,p1,p2,Grid):
    count = 0
    if Grid[i][j] ==".":
        if j-2>=0 and j+1<=14:
            if Grid[i][j-1] == p1 and Grid[i][j-2] == p2 and Grid[i][j+1] == ".":
                count += 1
            if Grid[i][j - 1] == p1 and Grid[i][j - 2] == "." and Grid[i][j + 1] == p2:
                count += 1
        if j - 1 >= 0 and j + 2 <= 14:
            if Grid[i][j + 1] == p1 and Grid[i][j - 1] == p2 and Grid[i][j + 2] == ".":
                count += 1
            if Grid[i][j + 1] == p1 and Grid[i][j - 1] == "." and Grid[i][j + 2] == p2:
                count += 1
        if i-2>=0 and i+1<=14:
            if Grid[i-1][j] == p1 and Grid[i-2][j] == p2 and Grid[i+1][j] == ".":
                count += 1
            if Grid[i - 1][j] == p1 and Grid[i - 2][j] == "." and Grid[i + 1][j] == p2:
                count += 1
        if i-1>=0 and i+2<=14:
            if Grid[i+1][j] == p1 and Grid[i-1][j] == p2 and Grid[i+2][j] == ".":
                count += 1
            if Grid[i+1][j] == p1 and Grid[i-1][j] == "." and Grid[i+2][j] == p2:
                count += 1
        if i-2>=0 and j-2>=0 and i+1<=14 and j+1<=14:
            if Grid[i - 1][j-1] == p1 and Grid[i - 2][j-2] == p2 and Grid[i + 1][j+1] == ".":
                count += 1
            if Grid[i - 1][j - 1] == p1 and Grid[i - 2][j - 2] == "." and Grid[i + 1][j + 1] == p2:
                count += 1
        if i - 1 >= 0 and j - 1 >= 0 and i + 2 <= 14 and j + 2 <= 14:
            if Grid[i + 1][j+1] == p1 and Grid[i - 1][j-1] == p2 and Grid[i + 2][j+2] == ".":
                count += 1
            if Grid[i + 1][j+1] == p1 and Grid[i - 1][j-1] == "." and Grid[i + 2][j+2] == p2:
                count += 1
        if i - 2 >= 0 and j - 1 >= 0 and i + 1 <= 14 and j + 2 <= 14:
            if Grid[i - 1][j+1] == p1 and Grid[i - 2][j+2] == p2 and Grid[i + 1][j-1] == ".":
                count += 1
        if i - 1 >= 0 and j - 2 >= 0 and i + 2 <= 14 and j + 1 <= 14:
            if Grid[i + 1][j-1] == p1 and Grid[i - 1][j+1] == p2 and Grid[i + 2][j-2] == ".":
                count += 1
            if Grid[i + 1][j - 1] == p1 and Grid[i - 1][j + 1] == "." and Grid[i + 2][j - 2] == p2:
                count += 1
        if i - 2 >= 0 and j - 1 >= 0 and i + 1 <= 14 and j + 2 <= 14:
            if Grid[i - 1][j+1] == p1 and Grid[i - 2][j+2] == "." and Grid[i + 1][j-1] == p2:
                count += 1
    return count


def identifyAlgorithm():
    fr = open(sys.argv[1], 'r')
    allFile = fr.readlines()
    taskType = allFile[0]
    return int(taskType)

def readInputFile():
    fr = open(sys.argv[1], 'r')
    allFile = fr.readlines()
    taskType = allFile[0]
    player = int(allFile[1])
    cutOff = int(allFile[2])
    boardSize = int(allFile[3])
    boardState = allFile[4]

    for i in range(boardSize):
        currentStateList = list(allFile[4])
        map(str.strip, currentStateList)


    for j in range(4, (boardSize + 4)):
        row = list(allFile[j])
        map(str.strip, row)
        Grid.append(row)

    return(taskType, player, cutOff, boardSize, boardState)

def eval_func(i,j,p1,p2,Grid):
    total_count = 0
    if isValidMoves(i,j,Grid):
        val1 = win_func(i,j,p1,p2,Grid)
        if val1 != 0:
            total_count = (total_count)+(val1*50000)
        val2 = blockClosedFour_func(i,j,p1,p2,Grid)
        if val2 != 0:
            total_count = (total_count)+(val2*10000)
        val3 = createOpenFour_func(i, j, p1, p2,Grid)
        if val3 != 0 :
            total_count = (total_count)+(val3*5000)
        val4 = createClosedFour_func(i, j, p1, p2,Grid)
        if val4 != 0 :
            total_count = (total_count)+(val4*1000)
        val5 = blockOpenThree_func(i, j, p1, p2,Grid)
        if val5 != 0:
            total_count = (total_count)+(val5*500)
        val6 = blockClosedThree_func(i, j, p1, p2,Grid)
        if val6 != 0:
            total_count = (total_count)+(val6*100)
        val7 = createOpenThree_func(i, j, p1, p2,Grid)
        if val7 != 0:
            total_count = (total_count)+(val7*50)
        val8 = createClosedThree_func(i, j, p1, p2,Grid)
        if val8 !=0:
            total_count = (total_count)+(val8*10)
        val9 = createOpenTwo_func(i, j, p1, p2,Grid)
        if val9 != 0:
            total_count = (total_count)+(val9*5)
        val10 = createClosedTwo_func(i, j, p1, p2,Grid)
        if val10 != 0:
            total_count = (total_count)+(val10*1)

    return total_count

def doGreedy(p1,p2):

    final_val = []

    for x in range(len(validGrid)):
        i, j = validGrid[x].split(";")
        if isValidMoves(int(i),int(j),Grid):
            max_val = eval_func(int(i),int(j),p1,p2,Grid)
            final_val.append(str(max_val)+":"+i+","+j)

    temp_list = []
    idx_list = []
    for k in range(len(final_val)):
        max_eval, idx = final_val[k].split(":")
        temp_list.append(int(max_eval))
        idx_list.append(idx)

    index_of_temp = temp_list.index(max(temp_list))


    for k in range(len(final_val)):
        if k == index_of_temp:
            final_i, final_j = idx_list[k].split(",")

    return (int(final_i), int(final_j))


def doMiniMax(latest_value,p1,p2,freshGrid,depth, cutOff, MaximizingPlayer):
    if cutOff == depth:
        if not MaximizingPlayer:
            bestvalue = neg_inf
        else:
            bestvalue = pos_inf
        for i in range(len(freshGrid)-1, 0, -1):
            for j in range(len(freshGrid[i])):
                if isValidMoves(i, j, freshGrid) is True:
                    value = int((eval_func(i,j,p1,p2,freshGrid)))
                    #print value
                    if not MaximizingPlayer:
                        if value > bestvalue:
                            bestvalue = int(value)
                            best_i = i
                            best_j = j
                    else:
                        value-=2*value
                        if value < bestvalue:
                            bestvalue = int(value)
                            best_i = i
                            best_j = j


        final_value = int(latest_value)+int(bestvalue)
        #print("leaf")

        return final_value ,best_i,best_j

    else:
        best_i = -1
        best_j = -1
        if MaximizingPlayer:
            bestvalue = neg_inf
            for i in range(len(freshGrid)-1, 0, -1):
                 for j in range(len(freshGrid[i])):
                     if isValidMoves(i,j,freshGrid) is True:
                         latest_value1 = 0
                         latest_value1 = int(latest_value) + int(eval_func(i, j, p1, p2, freshGrid))
                         resulting_Grid = makeAMove(i,j,p1,freshGrid)
                         temp, temp_i, temp_j = doMiniMax(latest_value1,p1,p2,resulting_Grid,depth+1,cutOff, False)
                         if temp > bestvalue:
                             bestvalue = int(temp)
                             best_i = i
                             best_j = j

            return bestvalue,best_i,best_j

        else:
            bestvalue = pos_inf
            for i in range (len(freshGrid)-1, 0, -1):
                for j in range(len(freshGrid[i])):
                    if isValidMoves(i, j, freshGrid) is True:
                            latest_value2 = 0
                            latest_value2 = int(latest_value) - int(eval_func(i, j, p2, p1, freshGrid))
                            resulting_Grid = makeAMove(i, j, p2, freshGrid)
                            #print(latest_value2)
                            temp, temp_i, temp_j = doMiniMax(latest_value2,p1,p2,resulting_Grid,depth+1, cutOff, True)
                            if temp < bestvalue:
                                bestvalue = int(temp)
                                best_i = j
                                best_j = i
            return bestvalue,best_i,best_j

def isStarting(depth):
    if depth % 2 == 0:
        return True

def findMax(arrayGrid):
    temp = -60000
    new_i = 0
    new_j = 0
    for i in range(len(arrayGrid)):
        for j in range(len(arrayGrid[i])):
            if arrayGrid[i][j] > temp:
                temp = arrayGrid[i][j]
                new_i = i
                new_j = j

    return temp

def findMin(arrayGrid):
    temp = 60000
    new_i = 0
    new_j = 0
    for i in range(len(arrayGrid)):
        for j in range(len(arrayGrid[i])):
            if arrayGrid[i][j] < temp:
                temp = arrayGrid[i][j]
                new_i = i
                new_j = j

    return temp


def makeAMove(x,y,player,TempGrid):
    newTempGrid = copy.deepcopy(TempGrid)
    newTempGrid[int(x)][int(y)] = player
    return newTempGrid

def swap_players(p1,p2):
    new1,new2 = p2,p1
    return new1,new2

def swap_player_minmaxGrid(playerOne_Grid,playerTwo_Grid,boardSize):
    new_MaxGrid = [[0] * boardSize] * boardSize
    new_MinGrid = [[0] * boardSize] * boardSize
    for i in range (len(new_MaxGrid)):
        for j in range (len(new_MaxGrid[i])):
            new_MaxGrid[i][j] = playerTwo_Grid[i][j]

    for i in range(len(new_MinGrid)):
        for j in range(len(new_MinGrid[i])):
            new_MinGrid[i][j] = playerOne_Grid[i][j]
    return new_MaxGrid,new_MinGrid


def isValidMoves(i,j,newGrid):
    if newGrid[i][j] == ".":
        if i == 0:
            if j == 0:
                if newGrid[i + 1][j] != "." or newGrid[i][j + 1] != "." or \
                                newGrid[i + 1][j + 1] != ".":
                    return True

            elif j == 14:
                if newGrid[i + 1][j] != "." or newGrid[i][j - 1] != "." or newGrid[i + 1][j - 1] != ".":
                    return True

            else:
                if newGrid[i + 1][j] != "." or newGrid[i][j + 1] != "." or newGrid[i][j - 1] != "." or \
                                newGrid[i + 1][j + 1] != "." or newGrid[i + 1][j - 1] != ".":
                    return True

        elif j == 0:
            if i == 14:
                if newGrid[i - 1][j] != "." or newGrid[i][j + 1] != "." or newGrid[i - 1][j + 1] != ".":
                    return True

            else:
                if newGrid[i + 1][j] != "." or newGrid[i - 1][j] != "." or newGrid[i][j + 1] != "." or \
                            newGrid[i + 1][j + 1] != "." or \
                            newGrid[i - 1][j + 1] != ".":
                    return True

        elif i == 14:
            if j ==14:
                if newGrid[i - 1][j] != "."or newGrid[i][
                            j - 1] != "." or \
                                 newGrid[i - 1][j - 1] != ".":
                    return True
            elif j== 0:
                if newGrid[i - 1][j] != "." or newGrid[i][j + 1] != "." or newGrid[i - 1][j + 1] != ".":
                    return True
            else:
                if newGrid[i - 1][j] != "." or newGrid[i][j + 1] != "." or newGrid[i][
                            j - 1] != "." or \
                                newGrid[i - 1][j - 1] != "." or \
                                newGrid[i - 1][j + 1] != ".":
                    return True
        elif j == 14:
            if newGrid[i + 1][j] != "." or newGrid[i - 1][j] != "." or newGrid[i][j - 1] != "." or \
                            newGrid[i - 1][j - 1] != "." or newGrid[i + 1][j - 1] != ".":
                return True

        else:
            if newGrid[i + 1][j] != "." or newGrid[i - 1][j] != "." or newGrid[i][j + 1] != "." or newGrid[i][j - 1] != "." or \
                            newGrid[i + 1][j + 1] != "." or newGrid[i - 1][j - 1] != "." or newGrid[i + 1][j - 1] != "." or \
                            newGrid[i - 1][j + 1] != ".":
                return True

    return False


def isPlayable():
    for i in range((len(Grid)-1),0,-1):
        for j in range(0, len(Grid[i]) - 1):
            if i + 1 <= 14 and j + 2 <= 14 and j + 3 <= 14 and i + 2 <= 14 and i + 3 <= 14 and j + 1 <= 14 and i - 1 >= 0 and j - 1 >= 0 and i - 2 >= 0 and j - 2 >= 0 and j - 3 >= 0 and i - 3 >= 0:
                if Grid[i + 1][j] != "." or Grid[i - 1][j] != "." or Grid[i][j + 1] != "." or Grid[i][j - 1] != "." or \
                                Grid[i + 1][j + 1] != "." or Grid[i - 1][j - 1] != "." or Grid[i + 1][j - 1] != "." or \
                                Grid[i - 1][j + 1] != ".":
                    validGrid.append(str(i) + ";" + str(j))

            else:
                pass
                # print "this is it"
                # print i,j
    return True

def drawBoard(i,j,finalBoard,player):
    finalBoard[i][j] = player
    result = ""
    for x in range(len(finalBoard)):
        for y in range(len(finalBoard[x])):
            result += finalBoard[x][y]
    return result


def main():

    (task, player, cutOff, boardSize, boardState) = readInputFile()

    if player == 1:
        p1 = "b"
        p2 = "w"
    elif player == 2:
        p1 = "w"
        p2 = "b"


    task = identifyAlgorithm()
    checkPlayable = isPlayable()
    if checkPlayable is True:
        if task == 1:
            i,j = doGreedy(p1,p2)
            print ("GREEDY-> Next Move: ")
            result = drawBoard(i, j, Grid, p1)
            text_file.write(result)
            text_file.close()
            print (result)
        elif task == 2:
            result2 = ""
            playerOne_Grid = [[0] * boardSize] * boardSize
            currentDepth = 1
            initialGrid = list(Grid)
            newBoard = []
            newBoard = [list(i) for i in (zip(*initialGrid[::-1]))]
            evalValue = 10
            count1 = 0
            MaximizingPlayer = True
            val, i, j = doMiniMax(0, p1, p2, initialGrid, 0, cutOff, MaximizingPlayer)
            print(val, i, j)
            result2 += drawBoard(i, j, Grid, p1)
            text_file.write(result2)
            text_file.close()


        elif task == 3:
            print("AlphaBeta")
            result2 = ""
            playerOne_Grid = [[0] * boardSize] * boardSize
            currentDepth = 1
            initialGrid = list(Grid)
            newBoard = []
            newBoard = [list(i) for i in (zip(*initialGrid[::-1]))]
            evalValue = 10
            count1 = 0
            MaximizingPlayer = True
            val, i, j = doMiniMax(0, p1, p2, initialGrid, 0, cutOff, MaximizingPlayer)
            print(val, i, j)
            result2 += drawBoard(i, j, Grid, p1)
            text_file.write(result2)
            text_file.close()
    else:
        print("Not playable")



main()
