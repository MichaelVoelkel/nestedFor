# nestedFor
Loop through all permutations of some container.

For example, if you have a `vector<vector<int>>` as `{{1, 2}, {3, 4}, {}, {5, 6}}`, `nestedFor` iterates all 8 combinations.

# Sample code:

    #include "nestedFor.h"
    #include <iostream>
    #include <vector>

    using namespace std;

    int main()
    {
        const std::vector<std::vector<int>>::size_type OUTER_VECTOR_SIZE = 4;
        const std::vector<int>::size_type INNER_VECTOR_SIZE = 2;

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

creates the output:

    {0, 1, }{}{3, 4, }{5, 6, }
    0, 3, 5, 
    1, 3, 5, 
    0, 4, 5, 
    1, 4, 5, 
    0, 3, 6, 
    1, 3, 6, 
    0, 4, 6, 
    1, 4, 6, 

# Notes
* C++11 required
* empty inner vectors are allowed, the corresponding index is -1, then
* you can easily modify the code to get iterators instead of indices and/or values in your callback function
* although you can also use leave out the `const` qualifiers in your callback, changing values has usually no reasonable effect
* although we use this function heavily, performance is not yet tested; especially due to indices and values given, a bare loop might be faster for some instances, you might need to test for yourself it is performance-critical
