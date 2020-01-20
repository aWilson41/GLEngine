#include "PolyDataIntersects2D.h"
#include "ContactManifold.h"
#include "PolyData.h"

PolyDataIntersects2D::PolyDataIntersects2D() { }
PolyDataIntersects2D::~PolyDataIntersects2D() { }

static void computeContacts(
	glm::vec3* pts1, glm::vec3* pts2,
	UINT* indices1, UINT* indices2,
	UINT indexCount1, UINT indexCount2,
	std::shared_ptr<ContactManifold2D> manifold)
{
	// Test every edge 1 of polygon 1 with 2
	for (UINT i1 = 0; i1 < indexCount1; i1++)
	{
		const glm::vec3& poly1P1 = pts1[i1];
		UINT i2 = i1 + 1;
		if (i2 > indexCount1)
			i2 = 0;
		const glm::vec3& poly1P2 = pts1[i2];

		for (UINT j1 = 0; j1 < indexCount2; j1++)
		{
			const glm::vec3& poly2P1 = pts2[i1];
			UINT j2 = j1 + 1;
			if (j2 > indexCount2)
				j2 = 0;
			const glm::vec3& poly2P2 = pts2[j2];
			// Also compute the normal
			const glm::vec2 n2 = MathHelp::perp(poly2P2 - poly2P1);

			glm::vec2 iPt;
			if (MathHelp::intersectSegmentSegment(poly1P1, poly1P2, poly2P1, poly2P2, iPt, false))
			{
				Contact2D& contact = manifold->contacts[manifold->numberOfContacts];
				contact.pt = iPt;
				// Direction to move 1 out of 2
				contact.n = glm::normalize(n2);
				contact.segmentIndex[0] = i1;
				contact.segmentIndex[1] = j1;
				manifold->numberOfContacts++;
				// We really just want 0, 1, or 2 contacts. # contacts > 2 becomes ambiguous without more information.
				if (manifold->numberOfContacts == 2)
					return;
			}
		}
	}
}

void PolyDataIntersects2D::update()
{
	// Input 1
	glm::vec3* pts1 = reinterpret_cast<glm::vec3*>(inputData1->getVertexData());
	UINT* indices1 = inputData1->getIndexData();
	const UINT indexCount1 = inputData1->getIndexCount();

	// Input 2
	glm::vec3* pts2 = reinterpret_cast<glm::vec3*>(inputData2->getVertexData());
	UINT* indices2 = inputData2->getIndexData();
	const UINT indexCount2 = inputData2->getIndexCount();
	const UINT vertexCount = inputData2->getPointCount();

	contactManifold = std::make_shared<ContactManifold2D>();
	computeContacts(pts1, pts2, indices1, indices2, indexCount1, indexCount2, contactManifold);
	
	// If no contacts were found then the objects aren't intersecting
	if (contactManifold->numberOfContacts == 0)
	{
		contactManifold = nullptr;
		return;
	}
	// If just one then the point must be on the surface with 0 penetration depth
	else if (contactManifold->numberOfContacts == 1)
	{
		contactManifold->contacts[0].depth = 0.0f;
		return;
	}
	// If 2 contacts then we must compute the depth to resolve the intersection
	else
	{
		// Get the two edge indices (left vertex indices)
		// These are in order of occurence on 1 given that we looped over 1 edges checking with 2
		const UINT polygon1EdgeIndex1 = contactManifold->contacts[0].segmentIndex[0];
		const UINT polygon1EdgeIndex2 = contactManifold->contacts[1].segmentIndex[0];
		// The two edge indices on polygon 2 are not garunteed to be in order (this is good)
		const UINT polygon2EdgeIndex1 = contactManifold->contacts[0].segmentIndex[1];
		const UINT polygon2EdgeIndex2 = contactManifold->contacts[1].segmentIndex[1];
		
		// To then find the interior edges we just need to find if vertex 0 is between
		// [polygon2EdgeIndex1, polygon2EdgeIndex2] or (polygon2EdgeIndex2, polygon2EdgeIndex1)
		int iter = 1;
		if (polygon2EdgeIndex1 < polygon1EdgeIndex2)
			iter = -1;

		// Now we will iterate either CCW (++) or CW (--) to get from one vertex index
		// This is done via projection of all the points onto each contact normal
		// We don't have to project every point though, just those inside the polygon
		// This means we loop from one contact point to the other (this allows convex shapes)
		UINT currVertexIndex = polygon2EdgeIndex1;
		const UINT endVertexIndex = polygon2EdgeIndex2;
		while (currVertexIndex != endVertexIndex)
		{
			// Project this vertex onto the contact normals
			GLfloat ptN1 = glm::dot(contactManifold->contacts[0].n, glm::vec2(pts1[currVertexIndex]));
			GLfloat ptN2 = glm::dot(contactManifold->contacts[1].n, glm::vec2(pts1[currVertexIndex]));

			contactManifold->contacts[0].depth = std::max(ptN1, contactManifold->contacts[0].depth);
			contactManifold->contacts[1].depth = std::max(ptN2, contactManifold->contacts[1].depth);

			currVertexIndex += iter;
			if (currVertexIndex < 0)
				currVertexIndex = vertexCount - 1;
			if (currVertexIndex >= vertexCount)
				currVertexIndex = 0;
		}
	}
}