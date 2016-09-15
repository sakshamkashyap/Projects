import copy
import sys
sys.stdout = open("output.txt", "w")

def readInputFile():
    inputFile = open(sys.argv[1], "r")
    myFile = inputFile.readlines()
    totalClauses = int(myFile[1])
    query = myFile[0].rstrip("\n")

    allClauses= []
    for i in range(2, totalClauses + 2):
        eachClause = myFile[i].strip("\n")
        allClauses.append(eachClause)

    return (query, totalClauses, allClauses)

def isFact(clause):
    if "=>" in clause:
        return False
    else:
        return True

def splitQuery(query):
    clauses = []
    if "=>" in query:
        if "&" in query:
            string = query.split("=>")
            rhs = string[1]
            first = string[0]
            lhs = first.split("&")
            for i in range(len(lhs)):
                clauses.append(lhs[i])
            clauses.append(rhs)
        else:
            string = query.split("=>")
            rhs = string[1]
            lhs = string[0]
            clauses.append(lhs)
            clauses.append(rhs)
        return clauses
    else:
        clauses.append(query)
        return clauses

class Predicate:
    name = ""
    args = []

    def __init__(self, string1):
        temp = string1.split("(")
        self.name = temp[0]
        self.args = temp[1].strip(")").split(",")
        self.query = string1

#Check the whether non-x arguments are same
def checkArgs(goal, rhs):
    for i in range(len(goal)):
        if goal[i] != "x":
            if rhs[i]!="x":
                if goal[i] != rhs[i]:
                    return False
    return True

def UnifyFact(goal, kb, thetax):
    thetay=0
    for i in range(len(goal)):
        if goal[i] != kb[i]:
            thetay = kb[i]
            #goal[i] = kb[i]
    if(thetay==0):
        return thetax
    return thetay

def updateArgs(conjunct, goalArgs):
    conjunct_args = conjunct.args
    conjunct_name = conjunct.name

    for i in range(len(goalArgs)):
        if conjunct_args[i] == "x":
            conjunct_args[i] = goalArgs[i]
    return conjunct

def modifyTheta(old_theta, new_theta):
    if isEmpty(new_theta) is True:
        alag_theta = []
    else:
        if len(old_theta) == 0:
            alag_theta = list(new_theta)
        elif len(new_theta) > 0:
            alag_theta = set(old_theta) & set(new_theta)
    return list(alag_theta)

def isEmpty(theta):
    for x in range(len(theta)):
        if len(theta[x]) == 0:
            return True
        else:
            return False

def BackwardChaining(KB, goal, theta3):

    bakchodtheta=[]
    theta1 =[]
    theta_to_kb = []
    nayatheta=[]
    jack = 0
    # if(len(theta3)>0):
    #     for i in range(len(theta3)):
    #         theta1.append(theta3[i])
    theta1 = copy.deepcopy(theta3)

    for j in range(len(goal)):
        theta = []
        goal_name = goal[j].name
        goal_args = goal[j].args
        print("Query: " + goal[j].query)

        for i in range(len(KB)):
            arr = splitQuery(KB[i])
            clauses = []
            conjuncts = []
            for x in range(len(arr)):
                clauses.append(Predicate(arr[x]))

            for y in range(len(clauses)-1):
                conjuncts.append(Predicate(clauses[y].query))

            if isFact(KB[i]) is False:
                rhs_name = clauses[len(clauses) - 1].name
                rhs_args = clauses[len(clauses) - 1].args

                if goal_name == rhs_name:
                        if checkArgs(goal_args, rhs_args) is True:
                            if sameIndex(goal_args, rhs_args) is True:
                                print("Query: " + KB[i])
                                for i in range(len(conjuncts)):
                                    conjuncts[i] = updateArgs(conjuncts[i], goal_args)
                                theta = BackwardChaining(KB, conjuncts, theta1)

                            elif sameIndex(goal_args, rhs_args) is False:
                                list_of_arr = list()
                                jack = 1
                                theta_final, theta_to_kb = secondTheta(goal_args, rhs_args)
                                list_of_arr.append(theta_final)
                                list_of_arr.append(theta_to_kb)
                                print("Query: " + KB[i])
                                for i in range(len(conjuncts)):
                                    conjuncts[i] = updateThetainArgs(conjuncts[i], theta_final)
                                theta = BackwardChaining(KB, conjuncts, theta_final)
                                list_of_arr = theta
                                list_of_arr.append(theta_to_kb)


            elif isFact(KB[i]) is True:
                kb_name = Predicate(KB[i]).name
                kb_args = Predicate(KB[i]).args

                if goal_name == kb_name:
                    if checkArgs(goal_args, kb_args) is True:
                        theta.append(UnifyFact(goal_args, kb_args, theta1))
                        #theta1.append(UnifyFact(goal_args, kb_args, theta1))

        if len(theta) > 0:
            if isEmpty(theta) is True:
                    print(goal[j].query + ": True")
            elif jack == 1:
                n = list_of_arr[len(list_of_arr) - 1]
                print(goal[j].query + ": True: ['" + str(n) + "']")
            else:
                theta = list(set(theta))
                theta.sort()
                print(goal[j].query + ": True: " + str(theta))
        else:
            print(goal[j].query + ": False")
        bakchodtheta.append(theta)
    if(len(bakchodtheta)) > 0:
        for x in bakchodtheta:
            if len(x) > 0:
                nayatheta = modifyTheta(nayatheta, x)
    if isEmpty(nayatheta) is False:
        nayatheta = list(set(nayatheta))
    return nayatheta

def BC_Constant(KB, goal, theta):

    for x in range(len(goal)):
        goal_name = goal[x].name
        goal_args = goal[x].args
        goal_query = goal[x].query
        print("Query: " + goal_query)

        for i in range(len(KB)):
            var = 0
            arr = splitQuery(KB[i])
            clauses = []
            conjuncts = []
            for k in range(len(arr)):
                clauses.append(Predicate(arr[k]))

            for y in range(len(clauses)-1):
                conjuncts.append(Predicate(clauses[y].query))

            if isFact(KB[i]) is False:
                rhs_name = clauses[len(clauses) - 1].name
                rhs_args = clauses[len(clauses) - 1].args
                rhs_query = clauses[len(clauses) - 1].query
                for z in range(len(goal_args)):
                    if goal_args[z] != rhs_args[z] and rhs_args[z] == "x":
                        theta = goal_args[z]
                        rhs_args[z] = theta
                if goal_name == rhs_name and goal_args == rhs_args:
                    print("Query: " + KB[i])
                    for i in range(len(conjuncts)):
                        conjuncts[i].args = updateConstArgs(conjuncts[i], theta)
                    var = BC_Constant(KB, conjuncts, theta)
                    if var == 1:
                        break

            elif isFact(KB[i]) is True:
                kb_name = Predicate(KB[i]).name
                kb_args = Predicate(KB[i]).args
                kb_query = Predicate(KB[i]).query

                if kb_name == goal_name and kb_args == goal_args:
                    var = 1
                    break
                else:
                    var = 0
        if var == 1:
            print(goal_query + ": True")
        else:
            print(goal_query + ": False")
    return var

def allSame(goal, kb):
    for i in range(len(goal)):
        if goal[i] == kb[i]:
            return True
    return False

def sameIndex(goal, kb):
    for x in range(len(goal)):
        if goal[x] == "x":
            if kb[x] == "x":
                return True
    return False

def secondTheta(goal, kb):
    temp_theta1 = []
    temp_theta2 = []
    for i in range(len(kb)):
        if kb[i] == "x":
            temp_theta1 = goal[i]
        if goal[i] == "x":
            temp_theta2 = kb[i]
    return temp_theta1, temp_theta2

def updateThetainArgs(clause1, theta):
    clause1_name = clause1.name
    clause1_args = clause1.args

    for i in range(len(clause1_args)):
        if clause1_args[i] == "x":
            clause1_args[i] = theta
    return clause1

def break_KB(rules):
    facts = []
    implications = []
    for i in range(len(rules)):
        if isFact(rules[i]) is True:
            facts.append(rules[i])
        elif isFact(rules[i]) is False:
            implications.append(rules[i])
    return facts, implications

def updateConstArgs(conjunct, theta):
    conjunct_args = conjunct.args
    conjunct_name = conjunct.name

    for i in range(len(conjunct_args)):
        if conjunct_args[i] == "x":
            conjunct_args[i] = theta
    return conjunct_args


def main():
    query, totalClauses, KB = readInputFile()
    initial_goal = []
    list_of_predicates = []
    var_arr = []
    const_arr = []
    king = 0
    new_KB = []
    final_theta = []
    final_var = 0
    queen = 0

    facts, implications = break_KB(KB)
    for i in range(len(facts)):
        new_KB.append(facts[i])
    for j in range(len(implications)):
        new_KB.append(implications[j])

    if "&" in query:
        initial_goal = query.split("&")
        for x in range(len(initial_goal)):
            list_of_predicates.append(Predicate(initial_goal[x]))
        for i in range (len(list_of_predicates)):
            for j in range(len(list_of_predicates[i].args)):
                if list_of_predicates[i].args[j].islower():
                    var_arr.append(list_of_predicates[i]) #variable - List of all clauses
                    king = 3
        # for i in range(len(list_of_predicates)):
        #     for j in range(len(list_of_predicates[i].args)):
        #         if not list_of_predicates[i].args[j].islower() and not list_of_predicates[i].args[j].isupper():
        #             const_arr.append(list_of_predicates[i]) #Const - List of all clauses
        #             king = 4
        #             print(list_of_predicates[i].query)

        const_arr = list(set(list_of_predicates) - set(var_arr))
        if len(const_arr) > 0:
            queen = 1

        if king == 3:
            final_theta = BackwardChaining(KB, var_arr, [])
            final_theta.sort()
        if queen == 1:
            final_var = BC_Constant(new_KB, const_arr, "")

        if len(var_arr) > 0 and len(const_arr) > 0:
            if len(final_theta) > 0 and final_var == 1:
                print(query + ": True: " + str(final_theta))
        elif len(var_arr) > 0 and len(const_arr) == 0:
            print(query + ": True: " + str(final_theta))
        elif len(const_arr) > 0 and len(var_arr) == 0 and final_var == 1:
            print(query + ": True")
        else:
            print(query + ": False")
    else:
        temp = Predicate(query)
        for x in range(len(temp.args)):
            if temp.args[x].islower():
                king = 1 #Contains x
                break
            else:
                king = 2 #Constains constant
        initial_goal.append(temp)
        if king == 1:
            BackwardChaining(KB, initial_goal, [])

        elif king == 2:
            BC_Constant(new_KB, initial_goal, "")

main()