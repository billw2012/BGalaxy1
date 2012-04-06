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
		BHNode(const math::AABB<pf_real>& bbox_)
			: bbox(bbox_), usedChildren(0) {}

		void reset(const math::AABB<pf_real>& bbox_)
		{
			usedChildren = 0;
			tempIndexes.clear();
			bbox = bbox_;
			mass = 0;
			massCenter = pf_Vector3::Zero;
		}

		std::vector<BHNodePtr> children;
		size_t usedChildren;
		pf_Vector3 massCenter;
		pf_real mass;
		math::AABB<pf_real> bbox;
		std::vector<size_t> tempIndexes;
	};

	//struct BHNode_CL
	//{
	//	cl_int children[8];
	//	cl_real4 massCenter;
	//	cl_real mass;
	//	cl_real4 ppp;
	//	cl_real4 nnn;
	//};

	typedef opencl::OpenCLBuffer<cl_int> OpenCLBufferI;
	typedef opencl::OpenCLBuffer<cl_real3> OpenCLBufferD3;
	typedef opencl::OpenCLBuffer<cl_real> OpenCLBufferD;

	OpenCLBufferI	nodes_children;
	OpenCLBufferD3	nodes_massCenter;
	OpenCLBufferD	nodes_mass;
	OpenCLBufferD3	nodes_nnn;
	OpenCLBufferD3	nodes_ppp;

	BHNodePtr root;
	std::vector<BHNodePtr> linearTree;

	static void evaluate_node_mass(const BHNodePtr& node, 
		const opencl::OpenCLBuffer<cl_real3>::vector_type& positions, 
		const opencl::OpenCLBuffer<cl_real>::vector_type& masses);

	void init();

	void build(const opencl::OpenCLBuffer<cl_real3>::vector_type& positions, 
		const opencl::OpenCLBuffer<cl_real>::vector_type& masses,
		const pf_Vector3& nnn,
		const pf_Vector3& ppp);

	void enqueue_write(const opencl::CLDevice& device);
	void bind_as_parameters(opencl::CLProgram& program, const std::string& kernel, size_t paramIdxStart);
};

#endif // BHTree_h__
