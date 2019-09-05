#include "LootTable.h"

void LootTable::pv_parseStep(std::vector<std::string> tokens)
{
	if (tokens[0] == "gearPiece") {
		GearPiece tempGearPiece;
		tempGearPiece.createFrom(decomposedData().createFrom(tokens[1]));
		addLootEntry(tempGearPiece, ma_deserialize_uint(tokens[2]));
	}
}

void LootTable::addLootEntry(const GearPiece& piece, unsigned int incidence)
{
	GearPiece *tempGearPiece = new GearPiece(piece);
	for (size_t i = 0; i < incidence; i++)
	{
		m_lootEntries.push_back(tempGearPiece);
	}
	m_toDeleteGearPieces.push_back(tempGearPiece);
	m_isEmpty = false;
}

bool LootTable::isEmpty() const
{
	return m_isEmpty;
}

void LootTable::mergeLootTable(const LootTable& lootTable)
{

}

GearPiece LootTable::getRandomGearPiece()
{
	return *m_lootEntries[rand() % m_lootEntries.size()];
}

LootTable::~LootTable()
{
	for (size_t i = 0; i < m_toDeleteGearPieces.size(); i++)
	{
		delete m_toDeleteGearPieces[i];
	}
}
