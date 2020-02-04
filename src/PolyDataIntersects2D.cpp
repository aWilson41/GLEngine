#include "PolyDataIntersects2D.h"
#include "PolyData.h"

static void computeContacts(
	glm::vec3* ptsA, glm::vec3* ptsB,
	UINT* indicesA, UINT* indicesB,
	UINT indexCountA, UINT indexCountB,
	std::shared_ptr<ContactList> contacts,
	std::shared_ptr<ContactList> contacts1,
	std::shared_ptr<ContactList> contacts2)
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
				const glm::vec2 nA = glm::normalize(MathHelp::perp(ptA1 - ptA2));
				const glm::vec2 nB = -glm::normalize(MathHelp::perp(ptB1 - ptB2));
				std::shared_ptr<Contact2D> contact1 = std::make_shared<Contact2D>(iPt, nA);
				std::shared_ptr<Contact2D> contact2 = std::make_shared<Contact2D>(iPt, nB);

				contacts->push_back(contact1);
				contacts1->push_back(contact1);
				contacts->push_back(contact2);
				contacts2->push_back(contact2);
			}
		}
	}
}

std::shared_ptr<Contact2D> PolyDataIntersects2D::getMinContact() const
{
	GLfloat minDepth = FLOAT_MAX;
	std::shared_ptr<Contact2D> minContact = nullptr;
	glm::vec2 pt = glm::vec2(0.0f);
	for (ContactList::iterator iter = contacts->begin(); iter != contacts->end(); iter++)
	{
		std::shared_ptr<Contact2D> contact = *iter;
		pt += contact->pt;
		if (std::abs(contact->depth) < minDepth)
		{
			minContact = contact;
			minDepth = std::abs(contact->depth);
		}
	}
	return minContact;
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

	contacts = std::make_shared<ContactList>();
	contacts1 = std::make_shared<ContactList>();
	contacts2 = std::make_shared<ContactList>();
	computeContacts(pts1, pts2, indices1, indices2, indexCount1, indexCount2, contacts, contacts1, contacts2);
	
	// If no contacts were found then the objects aren't intersecting
	if (contacts->size() == 0)
		contacts = nullptr;
	// If just one then the point must be on the surface with 0 penetration depth
	else if (contacts->size() == 1)
		(*(*contacts).begin())->depth = 0.0f;
	// If 2 contacts then we must compute the depth to resolve the intersection
	else
	{
		//const UINT vertexCount2 = inputData2->getVertexCount();
		//// Get the two edge indices (left vertex indices)
		//// These are in order of occurence on 1 given that we looped over 1 edges checking with 2
		//const UINT polygon1EdgeIndex1 = contactManifold->contacts[0].segmentIndex[0];
		//const UINT polygon1EdgeIndex2 = contactManifold->contacts[1].segmentIndex[0];
		//// The two edge indices on polygon 2 are not garunteed to be in order (this is good)
		//const UINT polygon2EdgeIndex1 = contactManifold->contacts[0].segmentIndex[1];
		//const UINT polygon2EdgeIndex2 = contactManifold->contacts[1].segmentIndex[1];
		//
		//// To then find the interior edges we just need to find if vertex 0 is between
		//// [polygon2EdgeIndex1, polygon2EdgeIndex2] or (polygon2EdgeIndex2, polygon2EdgeIndex1)
		//int iter = 1;
		//if (polygon2EdgeIndex1 < polygon1EdgeIndex2)
		//	iter = -1;

		//// Now we will iterate either CCW (++) or CW (--) to get from one vertex index
		//// This is done via projection of all the points onto each contact normal
		//// We don't have to project every point though, just those inside the polygon
		//// This means we loop from one contact point to the other (this allows convex shapes)
		//UINT currVertexIndex = polygon2EdgeIndex1;
		//const UINT endVertexIndex = polygon2EdgeIndex2;
		//GLfloat intersectionPt0Projected = glm::dot(contactManifold->contacts[0].pt, contactManifold->contacts[0].n);
		//GLfloat intersectionPt1Projected = glm::dot(contactManifold->contacts[1].pt, contactManifold->contacts[1].n);
		//while (currVertexIndex != endVertexIndex)
		//{
		//	// Project this vertex onto the contact normals
		//	GLfloat ptN1 = intersectionPt0Projected - glm::dot(contactManifold->contacts[0].n, glm::vec2(pts2[currVertexIndex]));
		//	GLfloat ptN2 = intersectionPt1Projected - glm::dot(contactManifold->contacts[1].n, glm::vec2(pts2[currVertexIndex]));

		//	contactManifold->contacts[0].depth = std::max(ptN1, contactManifold->contacts[0].depth);
		//	contactManifold->contacts[1].depth = std::max(ptN2, contactManifold->contacts[1].depth);

		//	currVertexIndex += iter;
		//	if (currVertexIndex < 0)
		//		currVertexIndex = vertexCount2 - 1;
		//	if (currVertexIndex >= vertexCount2)
		//		currVertexIndex = 0;
		//}


		// Project all of the intersection points onto the contact normals
		std::vector<GLfloat> iPtProjs1 = std::vector<GLfloat>();
		iPtProjs1.reserve(contacts1->size());
		for (ContactList::iterator iter = contacts1->begin(); iter != contacts1->end(); iter++)
		{
			std::shared_ptr<Contact2D> contact = *iter;
			iPtProjs1.push_back(glm::dot(contact->pt, contact->n));
		}

		// Find the "deepest point" along the contact normal
		for (UINT i = 0; i < inputData2->getVertexCount(); i++)
		{
			UINT j = 0;
			for (ContactList::iterator iter = contacts1->begin(); iter != contacts1->end(); iter++)
			{
				std::shared_ptr<Contact2D> contact = *iter;
				contact->depth = std::max(iPtProjs1[j] - glm::dot(contact->n, glm::vec2(pts2[i])), contact->depth);
				j++;
			}
		}

		std::vector<GLfloat> iPtProjs2 = std::vector<GLfloat>();
		iPtProjs2.reserve(contacts2->size());
		for (ContactList::iterator iter = contacts2->begin(); iter != contacts2->end(); iter++)
		{
			std::shared_ptr<Contact2D> contact = *iter;
			iPtProjs2.push_back(glm::dot(contact->pt, contact->n));
		}

		for (UINT i = 0; i < inputData1->getVertexCount(); i++)
		{
			UINT j = 0;
			for (ContactList::iterator iter = contacts2->begin(); iter != contacts2->end(); iter++)
			{
				std::shared_ptr<Contact2D> contact = *iter;
				contact->depth = std::max(glm::dot(contact->n, glm::vec2(pts1[i])) - iPtProjs2[j], contact->depth);
				j++;
			}
		}
	}
}