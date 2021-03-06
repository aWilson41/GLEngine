#pragma once
#include <bitset>
#include <map>

// A map of properties implemented as a bitset for use as a unique key
// Good way to do branching based off properties
template<size_t N>
class PropertyMap
{
public:
	std::bitset<N> getPropertyBits() const { return propertyBits; }
	std::map<std::string, size_t> getIndexMap() const { return indexMap; }
	bool getProperty(std::string key) { return propertyBits[indexMap[key]]; }
	bool isOutOfDate() const { return outOfDate; }

	// Returns assigned index in bitset
	size_t addProperty(std::string key, bool initialState = false)
	{
		indexMap[key] = incrementalKey;
		propertyBits.set(incrementalKey, initialState);
		incrementalKey++;
		return incrementalKey - 1;
	}
	void setProperty(std::string key, bool state)
	{
		if (!outOfDate && indexMap.find(key) != indexMap.end() && propertyBits[indexMap[key]] != state)
			outOfDate = true;
		propertyBits.set(indexMap[key], state);
	}

	void makeCurrent() { outOfDate = false; }
	void clear()
	{
		propertyBits.reset();
		indexMap.clear();
		incrementalKey = 0;
	}

private:
	std::bitset<N> propertyBits;
	std::map<std::string, size_t> indexMap;
	int incrementalKey = 0;
	bool outOfDate = true;
};