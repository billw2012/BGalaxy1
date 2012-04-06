
#include "BHTree.h"
#include <functional>
#include <algorithm>
#include <list>
#include "math/vector3.hpp"


void BHTree::evaluate_node_mass( const BHNodePtr& node, 
	const opencl::OpenCLBuffer<cl_real3>::vector_type& positions, 
	const opencl::OpenCLBuffer<cl_real>::vector_type& masses )
{
	using namespace math;

	Vector3d mrsum = Vector3d::Zero;
	double msum = 0.0;
	//pf_real averageFac = 1.0 / (pf_real)node->tempIndexes.size();
	std::for_each(node->tempIndexes.begin(), node->tempIndexes.end(), [&](size_t idx)
	{
		mrsum += (math::Vector3d(positions[idx].s[0], positions[idx].s[1], positions[idx].s[2]) * (double)masses[idx]);
		msum += (double)masses[idx];
	});

	if(msum > 0.0)
		node->massCenter = pf_Vector3(mrsum / msum);
	node->mass = (float)msum;

	//node->tempIndexes.clear();
	//node->tempIndexes.shrink_to_fit();

	//// center of mass R = 1/M * sum(mi*ri), where M = sum(mi), mi = mass for particle i and ri = position for particle i
	//std::for_each(node->children.begin(), node->children.end(), [&](const BHNodePtr& childNode)
	//{
	//	evaluate_node_mass(childNode, positions, masses);
	//	//mrsum += childNode->massCenter * childNode->mass;
	//	//msum += childNode->mass;
	//});
}

void BHTree::init()
{
	nodes_children.		init(OpenCLBufferI::Flags::KERNEL_READ);
	nodes_massCenter.	init(OpenCLBufferD3::Flags::KERNEL_READ);
	nodes_mass.			init(OpenCLBufferD ::Flags::KERNEL_READ);
	nodes_nnn.			init(OpenCLBufferD3::Flags::KERNEL_READ);
	nodes_ppp.			init(OpenCLBufferD3::Flags::KERNEL_READ);
}

void BHTree::build( const opencl::OpenCLBuffer<cl_real3>::vector_type& positions, 
	const opencl::OpenCLBuffer<cl_real>::vector_type& masses, 
	const pf_Vector3& nnn, const pf_Vector3& ppp )
{
	using namespace math;
	//BHNodePtr root(new BHNode(math::AABB<pf_real>(nnn - pf_Vector3::One, ppp + pf_Vector3::One)));
	if(!root)
		root.reset(new BHNode(math::AABB<pf_real>(nnn - pf_Vector3::One, ppp + pf_Vector3::One)));
	else
		root->reset(math::AABB<pf_real>(nnn - pf_Vector3::One, ppp + pf_Vector3::One));
	root->tempIndexes.resize(positions.size());
	for(size_t idx = 0; idx < positions.size(); ++idx)
		root->tempIndexes[idx] = idx;

	// used to determine the dimensions of child nodes relative to parent
	pf_Vector3 childPartitonsd[8][2] =
	{
		{ pf_Vector3(0.5, 0.5, 0.5), pf_Vector3(1.0, 1.0, 1.0) }, // PPP
		{ pf_Vector3(0.5, 0.5, 0.0), pf_Vector3(1.0, 1.0, 0.5) }, // PPN
		{ pf_Vector3(0.5, 0.0, 0.5), pf_Vector3(1.0, 0.5, 1.0) }, // PNP
		{ pf_Vector3(0.5, 0.0, 0.0), pf_Vector3(1.0, 0.5, 0.5) }, // PNN
		{ pf_Vector3(0.0, 0.5, 0.5), pf_Vector3(0.5, 1.0, 1.0) }, // NPP
		{ pf_Vector3(0.0, 0.5, 0.0), pf_Vector3(0.5, 1.0, 0.5) }, // NPN
		{ pf_Vector3(0.0, 0.0, 0.5), pf_Vector3(0.5, 0.5, 1.0) }, // NNP
		{ pf_Vector3(0.0, 0.0, 0.0), pf_Vector3(0.5, 0.5, 0.5) }  // NNN 
	};

	size_t nodeCount = 1;
	std::list<BHNodePtr> nodeStack;
	// build the tree
	nodeStack.push_back(root);
	while(!nodeStack.empty())
	{
		BHNodePtr currNode = nodeStack.back(); nodeStack.pop_back();
		if(currNode->tempIndexes.size() > 1)
		{
			// split the node
			pf_Vector3 parentSize = currNode->bbox.extents();
			for(size_t childIdx = 0; childIdx < 8; ++ childIdx)
			{
				BHNodePtr newChild;
				if(currNode->children.size() <= currNode->usedChildren)
				{
					newChild.reset(new BHNode(math::AABB<pf_real>(currNode->bbox.min() + parentSize * childPartitonsd[childIdx][0],
						currNode->bbox.min() + parentSize * childPartitonsd[childIdx][1])));
					currNode->children.push_back(newChild);
				}
				else
				{
					newChild = currNode->children[currNode->usedChildren];
					newChild->reset(math::AABB<pf_real>(currNode->bbox.min() + parentSize * childPartitonsd[childIdx][0],
						currNode->bbox.min() + parentSize * childPartitonsd[childIdx][1]));
				}
				std::for_each(currNode->tempIndexes.begin(), currNode->tempIndexes.end(), [&newChild, &positions](size_t idx) {
					const cl_real3& currPos = positions[idx];
					if(newChild->bbox.contains_conservative(currPos.s[0], currPos.s[1], currPos.s[2]))
					{
						newChild->tempIndexes.push_back(idx);
					}
				});
				if(!newChild->tempIndexes.empty())
				{
					++currNode->usedChildren;
					//currNode->children.push_back(newChild);
					nodeStack.push_back(newChild);
					++ nodeCount;
				}
			}
			//currNode->tempIndexes.clear();
			//currNode->tempIndexes.shrink_to_fit();
		}
		evaluate_node_mass(currNode, positions, masses);
	}

	// evaluate the masses
	//evaluate_node_mass(root, positions, masses);

	// convert tree into linear vector
	nodeStack.push_back(root);
	std::map<BHNodePtr, size_t> linearIdxMap;
	linearTree.clear();
	linearTree.reserve(nodeCount);
	while(!nodeStack.empty())
	{
		BHNodePtr currNode = nodeStack.front(); nodeStack.pop_front();
		linearIdxMap[currNode] = linearTree.size();
		linearTree.push_back(currNode);
		for(size_t idx = 0; idx < currNode->usedChildren; ++idx)
			nodeStack.push_back(currNode->children[idx]);
	}

	// copy data to the cl structures
	nodes_children	.resize(linearTree.size() * 8);
	nodes_massCenter.resize(linearTree.size());
	nodes_mass		.resize(linearTree.size());
	nodes_nnn		.resize(linearTree.size());
	nodes_ppp		.resize(linearTree.size());
	for(size_t idx = 0; idx < linearTree.size(); ++idx)
	{
		BHNodePtr currNode = linearTree[idx];
		nodes_mass[idx]			= currNode->mass;
		nodes_massCenter[idx]	= convert_my_vec3(currNode->massCenter);
		nodes_nnn[idx]			= convert_my_vec3(currNode->bbox.min());
		nodes_ppp[idx]			= convert_my_vec3(currNode->bbox.max());
		size_t cidx = 0;
		for(; cidx < currNode->usedChildren; ++cidx)
		{
			nodes_children[idx*8+cidx] = linearIdxMap[currNode->children[cidx]];
		}
		for(; cidx < 8; ++cidx)
			nodes_children[idx*8+cidx] = -1;
	}
}

void BHTree::enqueue_write(const opencl::CLDevice& device)
{
	nodes_children.enqueue_write(device);
	nodes_massCenter.enqueue_write(device);
	nodes_mass.enqueue_write(device);
	nodes_nnn.enqueue_write(device);
	nodes_ppp.enqueue_write(device);
}

void BHTree::bind_as_parameters(opencl::CLProgram& program, const std::string& kernel, size_t paramIdxStart)
{
	program.bind_parameter(kernel, paramIdxStart + 0, nodes_children);
	program.bind_parameter(kernel, paramIdxStart + 1, nodes_massCenter);
	program.bind_parameter(kernel, paramIdxStart + 2, nodes_mass);
	program.bind_parameter(kernel, paramIdxStart + 3, nodes_nnn);
	program.bind_parameter(kernel, paramIdxStart + 4, nodes_ppp);
}