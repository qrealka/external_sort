/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date Mar 2015
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 * @package Mphf
 * 
 * @date Jun 2015
 * @fix compilation error by Dmitry Loginov <qrealka@gmail.com>
 * @migrate to std::vector uniue_ptr by Dmitry Loginov <qrealka@gmail.com>
 */

#include "Graph.hpp"
#include <iostream>

namespace NMphf
{

Graph::Graph(unsigned nodesCount) throw()
    : mCreatedNodesCount(0)
	, mNodes(nodesCount) // () means create null pointers.
{
}


unsigned Graph::getNodesCount() const
{
	return mNodes.size();
}

unsigned Graph::getEdgesCount() const
{
    return mEdgesCount;
}

bool Graph::connect(
    unsigned firstNodeIndex,
    unsigned secondNodeIndex
    )
{
	if (firstNodeIndex >= mNodes.size() ||
		secondNodeIndex >= mNodes.size())
    {
        return false;
    }

	if (mNodes[firstNodeIndex] == nullptr)
    {
		mNodes[firstNodeIndex].reset(new Node);
        ++mCreatedNodesCount;
    }
	if (mNodes[secondNodeIndex] == nullptr)
    {
		mNodes[secondNodeIndex].reset(new Node);
        ++mCreatedNodesCount;
    }

	mNodes[firstNodeIndex]->connectTo(secondNodeIndex);
	mNodes[secondNodeIndex]->connectTo(firstNodeIndex);

    ++mEdgesCount;

    return true;
}

std::unique_ptr<Node>& Graph::getNode(unsigned index)
{
    return mNodes[index];
}

const std::unique_ptr<Node>& Graph::getNode(unsigned index) const
{
    return mNodes[index];
}

bool Graph::isCyclic() const
{
    // If empty graph means it is acyclic.
    if (mCreatedNodesCount == 0)
    {
        return false;
    }

	BoolArray visitedNodes(mNodes.size(), false);
    
    // Total deleted leaf nodes.
    unsigned deletedNodesCount = 0;
    for (unsigned i = 0; i < mNodes.size(); ++i)
    {
        deleteLeafNodes(i, visitedNodes, deletedNodesCount);
    }

    return mCreatedNodesCount > deletedNodesCount;
}

bool Graph::deleteLeafNodes(
    unsigned index,
	BoolArray& visitedNodes,
    unsigned& deletedNodesCount
    ) const
{
	const auto& node = getNode(index);
    if (node == nullptr || visitedNodes[index])
    {
        return false;
    }

    visitedNodes[index] = true;

    // Calculates how many related nodes have been removed.
    unsigned deletedEdgesCount = 0;
    for (unsigned i = 0; i < node->getEdgesCount(); ++i)
    {
        unsigned nextNodeIndex = node->getNodeIndex(i);
        if (visitedNodes[nextNodeIndex])
        {
            continue;
        }

		if (getNode(nextNodeIndex) == nullptr ||
            deleteLeafNodes(nextNodeIndex, visitedNodes, deletedNodesCount))
        {
            ++deletedEdgesCount;
        }
    }

    // If at least one edge left need to remove the current node
    // because it is already a leaf.
    if (deletedEdgesCount + 1 >= node->getEdgesCount())
    {
        ++deletedNodesCount;

        return true;
    }

    return false;
}

void Graph::calculateNodeValues()
{
	BoolArray visitedNodes(mNodes.size(), false);

    // Each edge will have an unigue id to avoid collisions.
    unsigned edgeId = 0;
    for (unsigned i = 0; i < getNodesCount(); ++i)
    {
		if (getNode(i) != nullptr && !visitedNodes[i])
        {
            traverse(i, visitedNodes, edgeId);
        }
    }
}

void Graph::traverse(
    unsigned index,
	BoolArray& visitedNodes,
    unsigned& edgeId
    )
{
    visitedNodes[index] = true;

	auto& node = getNode(index);
    if (node == nullptr)
    {
        return;
    }

    for (unsigned i = 0; i < node->getEdgesCount(); ++i)
    {
        unsigned nextNodeIndex = node->getNodeIndex(i);

	    auto& nextNode = getNode(nextNodeIndex);
        if (nextNode == nullptr || visitedNodes[nextNodeIndex])
        {
            continue;
        }

        nextNode->setValue(edgeId - node->getValue());
        ++edgeId;
        traverse(nextNodeIndex, visitedNodes, edgeId);
    }
}

bool Graph::getEdgeId(
    unsigned firstNodeIndex,
    unsigned secondNodeIndex,
    unsigned& result)
{
	auto firstNode = firstNodeIndex < mNodes.size() ? getNode(firstNodeIndex).get() : nullptr;
	auto secondNode = secondNodeIndex < mNodes.size() ? getNode(secondNodeIndex).get() : nullptr;
    if (firstNode == nullptr || secondNode == nullptr)
    {
        return false;
    }

    result = (firstNode->getValue() + secondNode->getValue()) % mEdgesCount;

    return true;
}

} // namespace NMphf
