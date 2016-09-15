import copy
import sys

sys.stdout = open("output.txt", "w")

def readInputFile():
    inputFile = open(sys.argv[1], "r")
    myFile = inputFile.readlines()
    totalQuery = int(myFile[0])

    bayesnet = []
    for i in range(totalQuery + 1, len(myFile)):
        eachLine = myFile[i].strip("\n")
        bayesnet.append(eachLine)

    queries = []
    for j in range(1, totalQuery + 1):
        eachQuery = myFile[j].strip("\n")
        queries.append(eachQuery)
    return totalQuery, queries, bayesnet


def makeBayesNet(inputBN):
    nodes = []
    for i in range(len(inputBN)):
        if inputBN[i] != "***" and "." not in inputBN[i]:
            nodes.append(inputBN[i])

    keyNames = []
    tempParent = []
    for j in range(len(nodes)):
        if "|" in nodes[j]:
            temp = nodes[j].split("|")
            child = temp[0].rstrip(" ")
            parent = temp[1].lstrip(" ")
            tempParent.append(parent)
            keyNames.append(child)
        else:
            keyNames.append(nodes[j])
            tempParent.append([])

    valueArray = []
    for x in range(len(tempParent)):
        if " " in tempParent[x]:
            temp1 = tempParent[x].split(" ")
            valueArray.append(temp1)
        elif str(tempParent[x]).isalpha():
            singleValue = tempParent[x].split()
            valueArray.append(singleValue)
        else:
            valueArray.append(tempParent[x])

    probArray = []
    tempProbVal = []
    for y in range(len(inputBN)):
        if "." in inputBN[y]:
            tempProbVal.append(inputBN[y])
        if "***" == inputBN[y] or y == len(inputBN) - 1:
            probArray.append(tempProbVal)
            tempProbVal = []
    return keyNames, valueArray, probArray


def splitProbability(val):
    finalVal = {}
    parray = []
    if len(val) == 1:
        finalVal[None] = float(val[0])
    else:
        for i in range(len(val)):
            parray.append(val[i].split(" "))
        for z in range(len(parray)):
            str = ()
            for j in range(1, len(parray[z])):
                if parray[z][j] == "+":
                    str += ("+",)
                else:
                    str += ("-",)
            finalVal[str] = float(parray[z][0])
    return finalVal


def constructBayesNet(key, parent, prob):
    newBN = {}
    for x in range(len(key)):
        newBN[key[x]] = [parent[x], splitProbability(prob[x])]
    return newBN


def isEmptyNode(arr):
    if len(arr) == 0:
        return True


def constructQueries(queries):
    qArray = []
    for x in range(len(queries)):
        eName = []
        eSign = []
        evidence = {}
        subquery = queries[x].split("(")
        query = subquery[1].strip(")")
        if "|" in query:
            temp = query.split(" | ")
            for q in range(0, len(temp)):
                if q != 0:
                    rhs = temp[q].split(", ")
                    for r in range(len(rhs)):
                        temp1 = rhs[r].split(" ")
                        if temp1[2] == "+":
                            evidence[temp1[0]] = "+"
                        else:
                            evidence[temp1[0]] = "-"

                elif q == 0:
                    lhs = temp[q].split(", ")
                    for l in range(len(lhs)):
                        temp1 = lhs[l].split(" ")
                        eName.append(temp1[0])
                        if temp1[2] == "+":
                            eSign.append("+")
                        else:
                            eSign.append("-")

        else:
            if "," in query:
                temp = query.split(", ")
                for q in range(len(temp)):
                    temp1 = temp[q].split(" ")
                    eName.append(temp1[0])
                    if temp1[2] == "+":
                        eSign.append("+")
                    else:
                        eSign.append("-")
            else:
                temp = query.split(" ")
                eName.append(temp[0])
                if temp[2] == "+":
                    eSign.append("+")
                else:
                    eSign.append("-")

        qDict = makeDict(eName, eSign, evidence)
        qArray.append(qDict)
    return qArray


def reverseList(list):
    newList = []
    for i in reversed(list):
        newList.append(i)
    return newList


def enumAsk(X, e, bn, rNodes):
    total = 0.0
    queryDistribution = {}
    for j in ["-", "+"]:
        e[X] = j
        queryDistribution[j] = enumAll(rNodes, e, bn)
        del e[X]
    for value in queryDistribution.values():
        total = total + value
    for key in queryDistribution.keys():
        queryDistribution[key] = queryDistribution[key] / total
    return queryDistribution


def makeDict(name, sign, evidenceQuery):
    pDict = {}
    pDict["node"] = name
    pDict["sign"] = sign
    pDict["evidence"] = evidenceQuery
    return pDict


def enumAll(rNodes, e, BN):
    if isEmptyNode(rNodes):
        return 1.0
    Y = rNodes.pop()
    if Y not in e:
        total = 0
        for j in ["+", "-"]:
            e[Y] = j
            total += totalProb(Y, e[Y], e, BN, rNodes)
        del e[Y]
        rNodes.append(Y)
        return total
    else:
        pVal = totalProb(Y, e[Y], e, BN, rNodes)
        rNodes.append(Y)
        return pVal


def ProbOfParent(node, val, e, BN):
    parents = BN[node][0]
    if len(parents) != 0:
        tempVal = [e[parent] for parent in parents]
        pr = BN[node][1][tuple(tempVal)]
    elif len(parents) == 0:
        pr = BN[node][1][None]
    if val == "-":
        return 1.0 - pr
    else:
        return pr


def totalProb(Y, sign, e, BN, rNodes):
    return ProbOfParent(Y, sign, e, BN) * enumAll(rNodes, e, BN)


def main():
    totalQueries, inputQuery, inputBN = readInputFile()
    keyArr, parentArr, probArr = makeBayesNet(inputBN)
    BN = constructBayesNet(keyArr, parentArr, probArr)
    allQuerries = constructQueries(inputQuery)
    rNodes = reverseList(keyArr)
    for query in allQuerries:
        final_result = 1
        arr = []
        for eachNode in query["node"]:
            evidence = {}
            if bool(query["evidence"]) is False:
                for z in range(0, len(query["node"])):
                    if query["node"][z] not in BN[eachNode][0]:
                        pass
                    else:
                        evidence[query["node"][z]] = query["sign"][z]
            else:
                evidence = copy.deepcopy(query["evidence"])
            value = enumAsk(eachNode, evidence, BN, rNodes)
            arr.append(value)
        for i in range(len(query["node"])):
            final_result *= arr[i][query["sign"][i]]
        print(round(final_result, 2))


main()