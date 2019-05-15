#ifndef SERIALIZATIONUTILS_H
#define SERIALIZATIONUTILS_H
#include <memory>
#include <iostream>
#include <boost/python.hpp>
#include<vector>

#include "boost/shared_ptr.hpp"
#include "boost/python/stl_iterator.hpp"

using namespace std;
template <class T>
inline
boost::python::list std_vector_to_py_list(std::vector<T> vector) {
    typename std::vector<T>::iterator iter;
    boost::python::list list;
    for (iter = vector.begin(); iter != vector.end(); ++iter) {
        list.append(*iter);
    }
    return list;
}

std::vector<std::pair<size_t, size_t>> getOutermostBlocks(std::string);
std::vector<std::string> processBlock(std::string);
std::vector<char const *> processBlockRetChar(std::string);
boost::python::list processBlockForPython(char const*);
std::vector<std::string> tokenizeDecomposedDataData(std::string, std::string delimiter = ",");
boost::python::list tokenizeDecomposedDataDataForPython(char const*, char const*);


#endif // SERIALIZATIONUTILS_H
