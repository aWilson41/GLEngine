#include "PolyDataIntersects2D.h"
#include "ContactManifold.h"
#include "PolyData.h"

static void computeContacts(
	glm::vec3* ptsA, glm::vec3* ptsB,
	UINT* indicesA, UINT* indicesB,
	UINT indexCountA, UINT indexCountB,
	std::shared_ptr<ContactManifold2D> manifold)
{
	// Test every edge 1 of polygon A(1) with B(2)
	for (UINT a1 = 0; a1 < indexCountA; a1 += 2)
	{
		const glm::vec3& ptA1 = ptsA[indicesA[a1]];
		const glm::vec3& ptA2 = ptsA[indicesA[a1 + 1]];

		for (UINT b1 = 0; b1 < indexCountB; b1 += 2)
		{
			const glm::vec3& ptB1 = ptsB[indicesB[b1]];
			const glm::vec3& ptB2 = ptsB[indicesB[b1 + 1]];

			glm::vec2 iPt;
			if (MathHelp::intersectSegmentSegment(ptA1, ptA2, ptB1, ptB2, iPt, false))
			{
				Contact2D& contact = manifold->contacts[manifold->numberOfContacts];
				contact.pt = iPt;
				// Direction to move 1 out of 2
				contact.n = glm::normalize(MathHelp::perp(ptA2 - ptA1));
				contact.segmentIndex[0] = a1;
				contact.segmentIndex[1] = b1;
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
		printf("hit\n");
		return;
	}
	// If 2 contacts then we must compute the depth to resolve the intersection
	else
	{
		const UINT vertexCount1 = inputData1->getPointCount();
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
		GLfloat intersectionPt0Projected = glm::dot(contactManifold->contacts[0].pt, contactManifold->contacts[0].n);
		GLfloat intersectionPt1Projected = glm::dot(contactManifold->contacts[1].pt, contactManifold->contacts[1].n);
		while (currVertexIndex != endVertexIndex)
		{
			// Project this vertex onto the contact normals
			GLfloat ptN1 = intersectionPt0Projected - glm::dot(contactManifold->contacts[0].n, glm::vec2(pts2[currVertexIndex]));
			GLfloat ptN2 = intersectionPt1Projected - glm::dot(contactManifold->contacts[1].n, glm::vec2(pts2[currVertexIndex]));

			contactManifold->contacts[0].depth = std::max(ptN1, contactManifold->contacts[0].depth);
			contactManifold->contacts[1].depth = std::max(ptN2, contactManifold->contacts[1].depth);

			currVertexIndex += iter;
			if (currVertexIndex < 0)
				currVertexIndex = vertexCount1 - 1;
			if (currVertexIndex >= vertexCount1)
				currVertexIndex = 0;
		}
		printf("hit\n");
	}
}