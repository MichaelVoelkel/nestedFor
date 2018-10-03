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

#ifndef MV_NESTED_FOR_
#define MV_NESTED_FOR_

#include <iterator>
#include <vector>

namespace nestedFor
{
    // helper to check if an iterator is const
    template<typename Iterator>
    struct isConstIterator
    {
        typedef typename std::iterator_traits<Iterator>::pointer pointer;
        static const bool value =
        std::is_const<typename std::remove_pointer<pointer>::type>::value;
    };
    
    // helper to get const_iterator or iterator, whatever applicable, for nested type
    template<typename OuterItT, typename=void>
    struct retrieveConstCorrectIterator
    {
        using vt  = typename OuterItT::value_type;
        using iterator = typename vt::iterator;
    };
    
    template<typename OuterItT>
    struct retrieveConstCorrectIterator<OuterItT, std::enable_if_t<isConstIterator<OuterItT>::value>>
    {
        using iterator = typename OuterItT::value_type::const_iterator;
    };
    
    // given a container<container<T>>, loops all combinations of inner vector;
    // Func should be of type:
    // void func(const std::vector<int>& indices, std::vector<T>& values);
    // in each iteration, func is called with indices showing the position
    // and respective values;
    // note that empty inner vectors are allowed, the respective index is -1, then
    template<typename OuterItT, typename Func,
        typename InnerItT = typename OuterItT::value_type::const_iterator>
    void run(OuterItT begin, OuterItT end, Func func)
    {
        using ItVecT = std::vector<InnerItT>;
        using ItVecItT = typename ItVecT::iterator;
        
        // idx -1 means that there is no valid entry
        using IdxVecT = std::vector<int>;
        using ValueVecT = std::vector<typename InnerItT::value_type>;
        using IdxVecItT = typename IdxVecT::iterator;
        using ValueVecItT = typename ValueVecT::iterator;
        
        const typename std::iterator_traits<OuterItT>::difference_type numInnerVecs =
        std::distance(begin, end);
        
        IdxVecT idxVec(numInnerVecs, -1);
        ValueVecT valueVec(numInnerVecs);
        
        ItVecT runIterators(numInnerVecs), startIterators(numInnerVecs), endIterators(numInnerVecs);
        ItVecItT runItVecIt = runIterators.begin(),
            startItVecIt = startIterators.begin(),
            endItVecIt = endIterators.begin();
        IdxVecItT runIdxIterators = idxVec.begin();
        ValueVecItT runValueIterators = valueVec.begin();
        
        for(OuterItT it = begin; it != end; ++it, ++runItVecIt, ++startItVecIt, ++endItVecIt,
            ++runIdxIterators, ++runValueIterators)
        {
            *runItVecIt = it->begin();
            *startItVecIt = it->begin();
            *endItVecIt = it->end();
            
            if(it->begin() == it->end())
            {
                *runIdxIterators = -1;
                // value idx undefined then
            }
            else
            {
                *runIdxIterators = 0;
                *runValueIterators = *(*startItVecIt);
            }
        }
        
        while(true)
        {
            func(idxVec, valueVec);
            
            ItVecItT itToMove = runIterators.begin(),
            itToMoveStart = startIterators.begin(),
            itToMoveEnd = endIterators.begin();
            
            IdxVecItT idxIt = idxVec.begin();
            ValueVecItT valueIt = valueVec.begin();
            
            bool bigBreak = false;
            ++*itToMove;
            
            if(*itToMove != *itToMoveEnd)
            {
                ++*idxIt;
                *valueIt = **itToMove;
            }
            
            while(*itToMove == *itToMoveEnd)
            {
                *itToMove = *itToMoveStart;
                
                if(*itToMove != *itToMoveEnd)
                {
                    *idxIt = 0;
                    *valueIt = **itToMoveStart;
                }
                
                ++itToMove;
                ++itToMoveStart;
                ++itToMoveEnd;
                ++idxIt;
                ++valueIt;
                
                if(itToMove == runIterators.end())
                {
                    bigBreak = true;
                    break;
                }
                
                if(*itToMove != *itToMoveEnd)
                {
                    ++*itToMove;
                    ++*idxIt;
                    *valueIt = **itToMove;
                }
            };
            
            if(bigBreak)
                break;
            
        }
    }
}

#endif
