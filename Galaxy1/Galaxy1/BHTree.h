#ifndef BHTree_h__
#define BHTree_h__

#include <memory>
#include "Math/vector3.hpp"
#include "Math/vector4.hpp"
#include "Math/matrix3.hpp"
#include "Math/misc.hpp"
#include "Math/aabb.hpp"
#include "OpenCLWrappers.h"

struct BHTree
{
	struct BHNode;
	typedef std::shared_ptr<BHNode> BHNodePtr;
	struct BHNode
	{
		BHNode(BHNode* parent_, const math::AABBd& bbox_)
			: parent(parent_), bbox(bbox_) {}

		std::vector<BHNodePtr> children;
		BHNode* parent;
		math::Vector3d massCenter;
		double mass;
		math::AABBd bbox;
		std::vector<size_t> tempIndexes;
	};

	//struct BHNode_CL
	//{
	//	cl_int children[8];
	//	cl_double4 massCenter;
	//	cl_double mass;
	//	cl_double4 ppp;
	//	cl_double4 nnn;
	//};

	typedef opencl::OpenCLBuffer<cl_int> OpenCLBufferI;
	typedef opencl::OpenCLBuffer<cl_double3> OpenCLBufferD3;
	typedef opencl::OpenCLBuffer<cl_double> OpenCLBufferD;

	OpenCLBufferI	nodes_children;
	OpenCLBufferD3	nodes_massCenter;
	OpenCLBufferD	nodes_mass;
	OpenCLBufferD3	nodes_nnn;
	OpenCLBufferD3	nodes_ppp;

	static void evaluate_node_mass(const BHNodePtr& node, 
		const opencl::OpenCLBuffer<cl_double3>::vector_type& positions, 
		const opencl::OpenCLBuffer<cl_double>::vector_type& masses);

	void init();

	void build(const opencl::OpenCLBuffer<cl_double3>::vector_type& positions, 
		const opencl::OpenCLBuffer<cl_double>::vector_type& masses,
		const math::Vector3d& nnn,
		const math::Vector3d& ppp);

	void enqueue_write(const opencl::CLDevice& device);
	void bind_as_parameters(opencl::CLProgram& program, const std::string& kernel, size_t paramIdxStart);
};

#endif // BHTree_h__
