#include "serializationUtils.h"


std::vector<std::pair<size_t, size_t>> getOutermostBlocks(std::string data){
    std::vector<std::pair<size_t, size_t>> retVal;

    if(data.find("}") != std::string::npos && data.find("{") != std::string::npos){

    }else{
        return retVal;
    }



    std::string cutData = data;
    size_t tempStartPos;
    size_t startPos;
    size_t endPos;
    int blockStartCounter = 0;

    size_t tempPos;
    while(cutData.find("}") != std::string::npos){
        tempPos = cutData.find("}");
        cutData[tempPos] = '!';
        if(cutData.find("}") != std::string::npos){

        }
    }

    cutData = data;
    cutData[cutData.find("{")] = '!';
    cutData[tempPos] = '!';

    while(cutData.find("}") != std::string::npos){

        //while(true){

        //}


        if(cutData.find("{") < cutData.find("}")){//if start block is before end block; adds to the start block counter
            tempStartPos = cutData.find("{");
            if(blockStartCounter == 0){
                startPos = tempStartPos;
            }
            cutData[tempStartPos] = '!';
            blockStartCounter++;
            //std::cout << "startBlock" << std::endl;

        }else if(cutData.find("}") != std::string::npos){//if endblock is before next start block, and is ending a previously detected start block
            blockStartCounter--;
            endPos = cutData.find("}");
            cutData[endPos] = '!';
            //std::cout << "endBlock" << std::endl;

        }
        if(blockStartCounter == 0){//if start blocks match end blocks
            retVal.push_back(std::pair<size_t, size_t>(startPos, endPos));
            //std::cout << "block added!" << std::endl;

        }


    }

    return retVal;
}


std::vector<std::string> processBlock(std::string data){
    std::vector<std::string> retVal;
    std::vector<std::pair<size_t, size_t>> outermostBlocks = getOutermostBlocks(data);
    if(outermostBlocks.size() >= 1){
        retVal.push_back(data.substr(data.find("{") + 1, (outermostBlocks[0].first - data.find("{")) - 1 ));
    }else{
        retVal.push_back(data.substr(data.find("{") + 1, (data.find("}") - data.find("{")) - 1 ));
    }


    for(int i = 0; i < outermostBlocks.size(); i++){
        retVal.push_back(data.substr(outermostBlocks[i].first, (outermostBlocks[i].second - outermostBlocks[i].first)+1));
    }
    return retVal;
//    std::vector<std::string> retVal;
//    size_t pos = data.find("{");
//    if(data.find("{") <  data.find("}")){
//        retVal.push_back(data.substr(pos, data.find("{")));
//
//    }else{
//        retVal.push_back(data.substr(pos, data.find("}")));
//    }
//
//
//    while(blockStartCounter>0){
//        cutData[cutData.find("}")] = '!';
//    }
//
//
//    return retVal;
}


std::vector<char const *> processBlockRetChar(std::string data){
    std::vector<char const *> retVal;
    std::vector<std::pair<size_t, size_t>> outermostBlocks = getOutermostBlocks(data);
    if(outermostBlocks.size() >= 1){
        retVal.push_back(data.substr(data.find("{") + 1, (outermostBlocks[0].first - data.find("{")) - 1 ).c_str());
    }else{
        retVal.push_back(data.substr(data.find("{") + 1, (data.find("}") - data.find("{")) - 1 ).c_str());
    }


    for(int i = 0; i < outermostBlocks.size(); i++){
        retVal.push_back(data.substr(outermostBlocks[i].first, (outermostBlocks[i].second - outermostBlocks[i].first)+1).c_str());
    }
    return retVal;
}

boost::python::list processBlockForPython(char const* data){
    //std::vector<char const *> temp = processBlockRetChar(data);
    std::vector<std::string> temp = processBlock(data);
    return std_vector_to_py_list(temp);
    //return boost::python::list();
}


std::vector<std::string> tokenizeDecomposedDataData(std::string data, std::string delimiter){
    std::vector<std::string> tokens;

    size_t pos = 0;
    pos = data.find(delimiter);
    while (pos != std::string::npos) {
        tokens.push_back(data.substr(0, pos));
        data.erase(0, pos + delimiter.length());
        pos = data.find(delimiter);
    }
    return tokens;
}

boost::python::list tokenizeDecomposedDataDataForPython(char const* data, char const* delimiter){
    return std_vector_to_py_list(tokenizeDecomposedDataData(data, delimiter));
}

BOOST_PYTHON_MODULE(gameSerializationUtils){
    using namespace boost::python;
    def("processBlock", processBlockForPython);
    def("tokenizeDecomposedDataData", tokenizeDecomposedDataDataForPython);
}

