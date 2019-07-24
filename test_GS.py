#import multiprocessing
#import shared_data
import decomposedData_python
import random




def gameScript(askingForName, messageType, senderID, intendedReceiverID, paramList, IDDictionary, selfID, metaData):
    if senderID == selfID:
        if intendedReceiverID == selfID:
            return ("NULL",0, "asd")
        return ("NULL",0, "")
  
    gameData = decomposedData_python.decomposedData()
    gameData.createFrom(metaData)


    if messageType == "playerData":
        inDData0 = decomposedData_python.decomposedData()
        inDData0.createFrom(paramList[0])


    #used for creating custom messages
    tempDData = decomposedData_python.decomposedData()
    tempDData.data.append("starting_door")
    tempDData.data.append("0")

    

    tempDData2 = decomposedData_python.decomposedData()
    tempDData2.data.append("perm_playerTex")
    tempDData2.data.append("")
    tempDData2.data.append("0")
    tempDData2.data.append("")
    tempDData2.data.append("")

 
    print(decomposedData_python.makeDDataMessage("messages", selfID,0,decomposedData_python.makeDDataMessage("editNamedWall", selfID,0, tempDData), decomposedData_python.makeDDataMessage("editNamedAnimatorSprite", selfID,0, tempDData2)).serialize())

    #shm = shared_memory.SharedMemory(memoryBlockName)
    if messageType == "onProgramEnd":
        #shm.unlink()
        return("NULL", 0, "")

    if messageType == "onProgramStart":
        #shm.buf[1] = 69
        return ("NULL",0,"")


    if messageType == "interactionKeyPressed":
        return ("NULL",0,"")

    retDData = decomposedData_python.decomposedData()
    retDData.type = "AnimatorSprite"
    retDData.name = "displayedSpriteName"
    retDData.data.append("blood_01.png") #the name of the image
    tempChildren = decomposedData_python.decomposedData()
    tempChildren.data.append(str(random.randint(250,500))) #the x and y positions of the image
    tempChildren.data.append(str(random.randint(250,500)))
    retDData.childrenObjects.append(tempChildren)
    retDData.data.append("2") #this is the scale of the image
    retDData.data.append("3") #this is how much time the image is displayed for
    retDData.data.append(str(random.randint(0,360))) #this is the rotation of the image in degrees

    #shm.close()

    return ("messages", 0, decomposedData_python.formatToSend(decomposedData_python.makeDDataMessage("displayDecal", selfID, 0, retDData), decomposedData_python.makeDDataMessage("editGameData", selfID, 0, gameData)))

