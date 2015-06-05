/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date Mar 2015
 * @copyright Zbigniew Czech, George Havas, Bohdan S. Majewski, VaL Doroshchuk
 * @license GNU GPL v2
 * @package Mphf
 */

#include "Chm.hpp"
#include "mphf/jenkins.hpp"
#include <math.h> // ceil()
#include <cstdlib> // rand()

namespace NMphf
{

Chm::Chm(double sizeFactor) throw()
    : mNodesCountFactor(sizeFactor)
    , mGraph(nullptr)
{
    mHashSeeds[0] = mHashSeeds[1] = 0;
}

Chm::~Chm()
{
    delete mGraph;
}

bool Chm::reset(unsigned len)
{
    if (mNodesCountFactor < 2.0)
    {
        mNodesCountFactor = 2.09;
    }

    delete mGraph;
    mGraph = new Graph(ceil(mNodesCountFactor * len));

    return mGraph != nullptr;
}

void Chm::getNodeIndices(
	const external_sort::RangeConstChar& key,
    unsigned& firstNodeIndex,
    unsigned& secondNodeIndex
    ) const
{
    firstNodeIndex = NHash::jenkins(mHashSeeds[0], key.begin(), key.size()) % mGraph->getNodesCount();
    secondNodeIndex = NHash::jenkins(mHashSeeds[1], key.begin(), key.size()) % mGraph->getNodesCount();

    if (firstNodeIndex == secondNodeIndex &&
        ++secondNodeIndex >= mGraph->getNodesCount())
    {
        secondNodeIndex = 0;
    }
}

bool Chm::generate(const external_sort::RangeLines& keys)
{
    bool result = false;
    for (unsigned attempt = 0; !result && reset(keys.size()) && attempt < 20; ++attempt)
    {
        mHashSeeds[0] = rand() % mGraph->getNodesCount();
        mHashSeeds[1] = rand() % mGraph->getNodesCount();
        for (unsigned i = 0; i < keys.size(); ++i)
        {
            unsigned firstNodeIndex, secondNodeIndex;
			getNodeIndices(keys[i], firstNodeIndex, secondNodeIndex);
            mGraph->connect(firstNodeIndex, secondNodeIndex);
        }

        result = !mGraph->isCyclic();
    }

    if (result)
    {
        mGraph->calculateNodeValues();
    }

    return result;
}

bool Chm::search(const external_sort::RangeConstChar& key, unsigned& result) const
{
    unsigned firstNodeIndex, secondNodeIndex;
    getNodeIndices(key, firstNodeIndex, secondNodeIndex);

    return mGraph->getEdgeId(firstNodeIndex, secondNodeIndex, result);
}

} // namespace NMphf
