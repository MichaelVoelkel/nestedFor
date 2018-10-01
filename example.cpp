// Copyright 2018 Michael Voelkel
// https://github.com/Elypson/nestedFor
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "nestedFor.h"
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    const std::vector<std::vector<int>>::size_type OUTER_VECTOR_SIZE = 5;
    const std::vector<int>::size_type INNER_VECTOR_SIZE = 5;
    
    vector<vector<int>> vec(OUTER_VECTOR_SIZE);
    
    int n = 0;
    
    for(auto& innerVec : vec)
    {
        // deliberately create a gap
        if(n == INNER_VECTOR_SIZE)
        {
            ++n;
            continue;
        }
        
        for(int m = 0; m < INNER_VECTOR_SIZE; ++m)
            innerVec.push_back(n++);
    }
    
    // output data structure JSON-like before
    for(auto& innerVec : vec)
    {
        std::cout << "{";
        for(auto& value : innerVec)
        {
            std::cout << value << ", ";
        }
        std::cout << "}";
    }
    
    std::cout << std::endl;
    
    // use nested for to create output
    nestedFor::run(vec.begin(), vec.end(),
                       [](const std::vector<int>& indices, const std::vector<int>& values){
                           for(int n = 0; n < values.size(); ++n)
                           {
                               if(indices[n] != -1)
                               {
                                   std::cout << values[n] << ", ";
                               }
                           }
                           
                           std::cout << std::endl;
                       });
    
    return 0;
}
