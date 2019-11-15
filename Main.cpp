
#include <SFML/Graphics.hpp>
#include <iostream>
#include <limits>
#include "unit.h"
#include "Map.h"
#include "Bullet.h"
#include "Weapon.h"
#include "unitManager.h"
#include "EnemyAI.h"
#include "Gear.h"
#include "GameMain.h"
#include <stdlib.h>
#include "Serializable.h"
#include "Animator.h"
#include "cryoscom_defsAndUtils.h"
#include "LevelMaker.h"
#include "ToolTip.h"

//messaging test
#include "MessageBus.h"
#include "MessagingComponent.h"

int factorial(int n)
{
    if(n > 1)
        return n * factorial(n - 1);
    else
        return 1;
}


//messaging test

//python test

#include <Python.h>
#include <boost/python.hpp>
#include <cstdlib>

template<typename T>
inline
std::vector< T > py_list_to_std_vector( const boost::python::object& iterable )
{
    return std::vector< T >( boost::python::stl_input_iterator< T >( iterable ),
                             boost::python::stl_input_iterator< T >( ) );
}

int setenv(const char* name, const char* value, int overwrite)
{
	int errcode = 0;
	if (!overwrite) {
		size_t envsize = 0;
		errcode = getenv_s(&envsize, NULL, 0, name);
		if (errcode || envsize) return errcode;
	}
	return _putenv_s(name, value);
}

//python test

int main()
{


    // Allow Python to load modules from the current directory.
    setenv("PYTHONPATH", ".", 1);
    // Initialize Python.
    Py_Initialize();

    //processBlockForPython("");
//    //serialization example:
//    std::vector<decomposedData> message;
//    decomposedData sData;
//    sData.type = "testType";
//    sData.name = "testType_001";
//    decomposedData sDataChildObject;
//    sDataChildObject.type = "asdType";
//    sDataChildObject.name = "asd";
//    sData.childrenObjects.push_back(sDataChildObject);
//    //std::cout << sData.serialize() << std::endl;
//    message.push_back(sData);
//
//
//
//    //deserialization example:
//    std::string data = "{r{ asdasd,.,.,**+++$%{}{{}{{}{{}{}}}}}{hello=/%&${{}{{}}}**+ }{}}";
//    data = "{rocket,main rocket,{cone,topCone,asd,{tip,coneTip,}{ring,cone ring,}}{fuselage,mainFuse,}}";
//    //data = sData.serialize();
//    //std::cout << data << std::endl;
//    decomposedData tempDData;
//    tempDData.createFrom(data);
//    message.push_back(tempDData);
//    std::string tempComposedString = composeString(message);
//    //std::cout << tempComposedString << std::endl;
//    //std::cout << decomposeString(tempComposedString)[1].type << std::endl;
//
//    while(true){
//
//    }
//
//    //std::cout << tempDData.serialize() << std::endl;
//    tempDData.createFrom(tempDData.serialize());
//    //std::cout << tempDData.serialize() << std::endl;
//    //std::cout << std::endl << "print: " << std::endl;
//    //std::cout << tempDData.type << std::endl;
//    //std::cout << tempDData.name << std::endl;
//    //std::cout << tempDData.childrenObjects[0].type << std::endl;
//    //std::cout << tempDData.childrenObjects[0].name << std::endl;
//    //std::cout << tempDData.childrenObjects[0].data[0] << std::endl;
//    //std::cout << std::endl << "rocket childrenObject, cone childrenObject, tip object: " << std::endl;
//    //std::cout << tempDData.childrenObjects[0].childrenObjects[0].type << std::endl;
//    //std::cout << tempDData.childrenObjects[0].childrenObjects[0].name << std::endl;
//
//    //std::cout << tempDData.childrenObjects[0].childrenObjects[1].type << std::endl;
//    //std::cout << tempDData.childrenObjects[0].childrenObjects[1].name << std::endl;
//
//    //std::cout << std::endl;
//    //std::cout << tempDData.childrenObjects[1].type << std::endl;
//    //std::cout << tempDData.childrenObjects[1].name << std::endl;
////    std::vector<std::string> result = processBlock(data);
////    //std::cout << data << std::endl;
////    size_t blocks = 0;
////    for(size_t i = 0; i < result.size(); i++){
////        blocks++;
////        //std::cout << result[i] << std::endl;
////    }
////    //std::cout << blocks << std::endl;
//
//
//
//    while(true){
//
//    }
    //messaging test
    //MessageBus onlyBus;
    //Interactable *tempInteractable = new Interactable();
    //tempInteractable->createFromFile("test_GS");
    //onlyBus.addMessagingComponent(tempInteractable);
    //MessageData *MData = new MessageData();
    //MData->senderID = 0;
    //MData->intendedReceiverID = 0;
    //MData->messageContents.push_back(decomposedData().setType("printable").setName("hello_printable").addData("hello").addData("0"));
    //onlyBus.addMessage(MData);

    //while(true){
    //    onlyBus.notify();
    //}

    //messaging test
    //python test
//
//
//    namespace python = boost::python;
//    try
//    {
//    // >>> import MyPythonClass
//    python::object my_python_class_module = python::import("MyPythonClass");
//
//    // >>> dog = MyPythonClass.Dog()
//    python::object dog = my_python_class_module.attr("Dog")();
//
//    python::object func = dog.attr("bark");
//
//
//
//    // >>> dog.bark("woof");
//    boost::python::list tempList;
//    tempList.append("lolxd");
//    tempList.append("roflcopter");
//
//    python::object retVal = func(tempList);
//    //std::cout << boost::python::len(retVal[2]) << std::endl;
//
//    tempList = python::extract<boost::python::list>(retVal[2]);
//
//
//    std::string realRetVal = python::extract<std::string>(retVal[1]);
//
//    boost::python::list nestedList = python::extract<boost::python::list>(tempList.pop(0));
//
//
//
//    //std::cout << realRetVal << std::endl;
//    //std::cout << python::extract<int>(nestedList[0]) << std::endl;
//    //std::cout << python::extract<int>(tempList.pop(0) << std::endl;
//    }
//    catch (const python::error_already_set&)
//    {
//    PyErr_Print();
//    return 1;
//    }
//
//    while(false){
//
//    }

    //python test
	
	sf::err().rdbuf(NULL);
	
	//memory test

	//template test

	//std::cout << ma_deserialize<float>("27.03") << std::endl;
	//return 0;


	//template test


#if MAKING_LEVELS
    LevelMaker levelMaker;
	levelMaker.levelMakerLoop("newFile.txt", "cryo_area_1.png");
	return 0;
#endif
	Animator::getInstance().loadTexturesFromFile("loaded_textures.txt");
	GameMain gameMain("master.txt");
	gameMain.spawnWindow();
	gameMain.createUIFromFile("mainUI.txt");
	gameMain.setProgressionFile("progression.txt");
	gameMain.startLevel();
	gameMain.onProgramStart();
	gameMain.gameLoop();
	gameMain.onProgramEnd();
	
	//_CrtDumpMemoryLeaks();

	return 0;

}
