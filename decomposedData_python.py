import gameSerializationUtils

class decomposedData:
    def __init__(self):
        self.typeName = ""
        self.name = ""
        self.data = []
        self.childrenObjects = []

    def __m_parseData(self, passedData):
        tempData = gameSerializationUtils.tokenizeDecomposedDataData(passedData, ",")
        self.typeName = tempData[0]
        self.name = tempData[1]
        self.data = tempData[2:]

    def getChildByName(self, childName):
        for i in self.childrenObjects:
            if i.name == childName:
                return i
            else:
                searchChildResult = i.getChildByName(childName)
                if searchChildResult != None:
                    return searchChildResult
        return None

    def getChildByPos(self, *pos):
        childIndex = pos[0]
        if len(pos) == 1:
            return self.childrenObjects[childIndex]
        nextPos = pos[1:]
        return self.childrenObjects[childIndex].getChildByPos(*nextPos)

    def getChildPosByName(self, childName):
        retVal = []
        for index, item in enumerate(self.childrenObjects):
            if item.name == childName:
                retVal.append(index)
                return retVal
            else:
                tempVal = item.getChildPosByName(childName)
                if len(tempVal) > 0:
                    retVal.append(index)
                    retVal.extend(tempVal)
                    return retVal

        return retVal


    def serialize(self):
        retVal = []
        retVal = ["{", self.typeName, ",", self.name, ","]
        for i in self.data:
            retVal.append(i)
            retVal.append(",")
        for o in self.childrenObjects:
            retVal.append(o.serialize())
        retVal.append("}")
        return "".join(retVal)

    def createFrom(self, passedData):
        allData = gameSerializationUtils.processBlock(passedData)
        self.__m_parseData(allData[0])
        self.childrenObjects = []
        allData = allData[1:]
        for i in allData:
            tempObj = decomposedData()
            tempObj.createFrom(i)
            self.childrenObjects.append(tempObj)
        return self


def makeDDataMessage(messageType, senderID, intendedReceiverID, *DData):
    retDDataMessage = decomposedData()
    retDDataMessage.data.append(messageType)
    retDDataMessage.data.append(str(senderID))
    retDDataMessage.data.append(str(intendedReceiverID))
    for i in DData:
       retDDataMessage.childrenObjects.append(i)
    return retDDataMessage


def formatToSend(*DData):
    retStr = ""
    for i in DData:
        retStr = retStr +  i.serialize() + ";"
    return retStr